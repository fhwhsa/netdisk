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

<br><br>

### 注销
1. 注销请求
	```
	id:{}\r\n	
	```

<br><br>

### 查找用户（根据id或邮箱查找）
1. 查找用户请求
    ```
    key:{id/邮箱}\r\n
    ```
2. 查找用户响应
    * 有该用户 `success\r\nid:{}\r\nemail:{}\r\ninfo:{}\r\n` 
    * 没有该用户或出错 `failure\r\ninfo:{}\r\n`

<br><br>

### 好友申请
1. 添加好友请求
    ```
    from:{id}\r\n
    to:{id}\r\n
    ```

2. 添加好友响应
    * 成功添加请求：`success\r\ninfo:{}\r\n`
    * 重复的请求：`conflict\r\ninfo:{}\r\n`
    * 失败：`failure\r\ninfo:{}\r\n`

<br><br>

### 好友申请记录
1. 获取好友申请记录请求
    ```
    from:{id}\r\n
    ```

2. 获取好友申请记录响应
    * 成功
        ```
        {申请记录id}|{邮箱}|{标志码}|{记录修改时间}\r\n
        ...
        ```
        标志码含义：
        * 1->请求添加对方为好友，等待验证
        * 2->请求添加对方为好友，已拒绝
        * 3->对方请求加为好友，等待验证
        * 4->对方请求加为好友，已拒绝

    * 失败：`failure\r\ninfo:{}\r\n`

<br><br>

### 验证好友申请
1. 同意/拒绝好友申请
    ```
    [accept/refuse]\r\n
    {申请记录id}
    ```
2. 响应
    * 成功 `success\r\ninfo:{}\r\n`
    * 失败 `failure\r\ninfo:{}\r\n`

<br><br>
