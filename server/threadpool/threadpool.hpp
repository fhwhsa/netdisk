#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include "../json/json.h"
#include "../log/log.h"

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <chrono>
#include <memory>
#include <unordered_set>
#include <fstream>
#include <atomic>
#include <assert.h>

class ThreadPool {
public:

    /// @brief 获取单例实例
    /// @return 
    static ThreadPool& getInstance() {
        static ThreadPool instance;
        return instance;
    }

    /// @brief 添加任务到线程池的任务队列，返回一个std::future对象
    /// @tparam F 
    /// @tparam ...Args 
    /// @param f 任务函数
    /// @param ...args 任务函数的参数
    /// @return std::future对象
    template<class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type> enqueue(F&& f, Args&&... args) {
        using return_type = typename std::result_of<F(Args...)>::type;  // 获取返回类型

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...) 
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    /// @brief 初始化线程池
    /// @param _maxIdleTime 最长空闲时间（ms）
    /// @param _maxThreads 最大工作线程数
    /// @param _minThreads 最小工作线程数
    /// @param _adjustAddThreads 单次调整增加工作线程数的最大值
    /// @param _adjustDuration 执行调整函数的间隔时间(ms)
    void init(ulong _maxIdleTime, int _maxThreads, int _minThreads, int _adjustAddThreads, int _adjustDuration)
    {
        maxIdleTime = _maxIdleTime;
        maxThreads = _maxThreads;
        minThreads = _minThreads;
        adjustAddThreads = _adjustAddThreads;
        adjustDuration = _adjustDuration;

        createThread();

        LogFunc::info("The thread pool initialization is complete.");
    }

    void initFromConfig(const Json::Value& root)
    {
        if (!root.isMember("thread-pool"))
            throw std::runtime_error("Profile is missing value 'thread-pool'!");
        
        auto check = [&](const std::string& key) {
            if (!root["thread-pool"].isMember(key))
                throw std::runtime_error("The thread-pool configuration is missing the value '" + key + "'");
        };
        
        for (const std::string& key : {"maxIdleTime", "maxThreads", "minThreads", "adjustAddThreads", "adjustDuration"})
            check(key);
        
        maxIdleTime = root["thread-pool"]["maxIdleTime"].asUInt() * 1000;
        maxThreads = root["thread-pool"]["maxThreads"].asInt();
        minThreads = root["thread-pool"]["minThreads"].asInt();
        adjustAddThreads = root["thread-pool"]["adjustAddThreads"].asInt();
        adjustDuration = root["thread-pool"]["adjustDuration"].asUInt() * 1000;

        createThread();   

        LogFunc::info("The thread pool initialization is complete.");
    }

    void close()
    {
        LogFunc::info("Start cleaning up thread pool resources.");
        stop = true;
        condition.notify_all();
        for (std::thread& worker : workers)
            if (worker.joinable())
                worker.join();
        if (adjustThread.joinable())
            adjustThread.join();
        LogFunc::info("Thread pool resource cleanup is complete.");
    }

private:
    ThreadPool(const ThreadPool&);
    ThreadPool& operator=(const ThreadPool&);

    ~ThreadPool() {
        if (!stop)
            close();
    }

    ThreadPool() : stop(false) {
        maxIdleTime = -1;
        maxThreads = -1;
        minThreads = -1;
        adjustAddThreads = -1;
        adjustDuration = -1;
    }

    void createThread()
    {
        assert(maxIdleTime > 0);
        assert(maxThreads > 0);
        assert(minThreads > 0);
        assert(adjustAddThreads > 0);
        assert(adjustDuration > 0);

        // 初始化工作线程数量
        for (int i = 0; i < minThreads; ++i)
            workers.emplace_back([this] { worker_thread(); });
        // 启动定时器动态调整工作线程数量
        adjustThread = std::thread([this] {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            while (!stop)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(adjustDuration));
                adjust_thread_count();
            }
        });
    }

    /// @brief 工作函数
    void worker_thread() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                std::thread::id tid = std::this_thread::get_id();
                condition.wait(lock, [this, &tid] { return stop || !tasks.empty() || shouldTerminate.find(tid) != shouldTerminate.end(); });
                if (stop) 
                    return;
                if (shouldTerminate.find(tid) != shouldTerminate.end())
                {
                    shouldTerminate.erase(tid);
                    return;
                }
                lastActive[tid] = std::chrono::steady_clock::now();
                task = std::move(tasks.front());
                tasks.pop();
            }
            task();
        }
    }

    /// @brief 动态调整线程池工作线程数量，根据设置的最小调整时间间隔决定是否执行
    void adjust_thread_count() {
        std::unique_lock<std::mutex> lock(queue_mutex);
        const auto now = std::chrono::steady_clock::now();

        // 增加工作线程
        if (tasks.size() * 2 > workers.size() && workers.size() < static_cast<size_t>(maxThreads)) {
            lock.unlock();  // 提前解锁，增加工作线程不影响现有工作线程
            int n = maxThreads - workers.size();
            n = n > adjustAddThreads ? adjustAddThreads : n;
            while (n--)
            {
                workers.emplace_back([this] { worker_thread(); });
            }
        }

        // 减少长时间空闲的工作线程
        else if (workers.size() > static_cast<size_t>(minThreads)) 
        {
            // lock.unlock(); 减少工作线程影响现有工作线程
            auto it = std::remove_if(workers.begin(), workers.end(), [&](std::thread& t) {
                std::chrono::milliseconds dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastActive[t.get_id()]);
                if (static_cast<ulong>(dur.count()) > maxIdleTime)
                {
                    t.detach();
                    shouldTerminate.emplace(t.get_id());
                    lastActive.erase(t.get_id());
                    return true;
                }
                return false;
            });
            workers.erase(it, workers.end());
            condition.notify_all();
        }

        LogFunc::info("Thread pool has adjust the number of work thread to %ld", workers.size());
    }

    /// @brief 工作线程列表
    std::vector<std::thread> workers;
    /// @brief 任务队列
    std::queue<std::function<void()>> tasks;
    /// @brief 任务队列锁
    std::mutex queue_mutex;
    /// @brief 任务队列条件变量
    std::condition_variable condition;
    /// @brief 线程池状态，true为运行中
    std::atomic<bool> stop;
    /// @brief 记录每个工作线程上一个活跃时间点，用于动态调整工作线程数量
    std::unordered_map<std::thread::id, std::chrono::steady_clock::time_point> lastActive;
    /// @brief 动态控制工作线程销毁
    std::unordered_set<std::thread::id> shouldTerminate;
    /// @brief 调整工作线程数量的线程
    std::thread adjustThread;

    /// @brief 最长空闲时间（ms）
    ulong maxIdleTime;
    /// @brief 最大工作线程数
    int maxThreads;
    /// @brief 最小工作线程数
    int minThreads;
    /// @brief 单次调整增加工作线程数的最大值
    int adjustAddThreads;
    /// @brief 执行调整函数的间隔时间(ms)
    ulong adjustDuration;
};

#endif
