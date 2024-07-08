# 数据库连接池
* MysqlConn：MySQL C API的C++封装
* ConnectionPool：数据库连接池实现
* 配置文件[config.json](server/config.json)：<br>
    读取路径为主函数所在目录下的config.json文件，文件中key值含义如下：<br>
    ```
    "ip": 数据库ip地址
    "port": 数据库端口
    "userName": 用户名
    "passwd": 用户密码
    "dbName": 数据库名称
    "minConnSize": 最小连接个数
    "maxConnSize": 最大连接个数
    "maxIdleTime": 连接最大空闲时长
    "maxWaitTime": 获取连接最大等待时长
    ```
