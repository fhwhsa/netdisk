# Netdisk Server

## 目录
* [项目框架](#框架)
* [通信协议](msg/README.md)
* [JSON库](json/README.md)
* [包裹函数](wrap/README.md)
* [数据库连接池](mysql/README.md)
* [数据库交互接口](idatabase/README.md)
* [运行环境](#运行环境)
* [运行](#运行)

<span id="框架"></span>
## 框架

<span id="运行环境"></span>
## 运行环境
* 操作系统：Ubuntu 22.04.4 LTS
* 开发工具：Visual Studio Code, Qt Creator
* 依赖库：
    - Qt 6.2.3
    - libevent-2.1.12-stable
    - mysql  Ver 8.0.37-0ubuntu0.22.04.3 for Linux on x86_64 ((Ubuntu))

<span id="运行"></span>
## 运行
* 数据库配置：运行MySQL数据库脚本，修改数据库连接池配置文件
* 服务器运行：`cd server && make && ./run`
* 客户端运行：`cd client && make && ./client`