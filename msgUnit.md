## 通信数据单元内容规范
以下所有的{}代表填充内容

### 登陆
1. 登陆请求
    ```
    email:{}\r\n
    passwd:{}\r\n
    ```
2. 登陆响应
    * 成功  `success\r\nid:{}\r\ninfo:{}\r\n` 
    * 失败  `failure\r\ninfo:{}\r\n`


### 注销
1. 注销请求
	```
	id:{}\r\n	
	```

### 查找用户（根据id或邮箱查找）
1. 查找用户请求
    ```
    key:{}\r\n
    ```
2. 查找用户响应
    * 有该用户 `success\r\nemail:{}\r\ninfo:{}\r\n` 
    * 没有该用户或出错 `failure\r\ninfo:{}\r\n`

### 好友申请
1. 添加好友请求
    ```
    from:{}\r\n
    to:{}\r\n
    ```

2. 添加好友响应
    * 成功添加请求：`success\r\ninfo:{}\r\n`
    * 重复的请求：`conflict\r\ninfo:{}\r\n`
    * 失败：`failure\r\ninfo:{}\r\n`
