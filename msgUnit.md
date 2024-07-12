## 通信数据单元内容规范
以下所有的{}代表填充内容

### 登陆
1. 请求
    ```
    email:{}\r\n
    passwd:{}\r\n
    ```
2. 响应
    * 成功  `success\r\nid:{}\r\ninfo:{}\r\n` 
    * 失败  `failure\r\ninfo:{}\r\n`

<br><br>

### 注销
1. 请求
	```
	id:{}\r\n	
	```

<br><br>

### 查找用户（根据id或邮箱查找）
1. 请求
    ```
    key:{id/邮箱}\r\n
    ```
2. 响应
    * 有该用户 `success\r\nid:{}\r\nemail:{}\r\ninfo:{}\r\n` 
    * 没有该用户或出错 `failure\r\ninfo:{}\r\n`

<br><br>

### 好友申请
1. 请求
    ```
    from:{id}\r\n
    to:{id}\r\n
    ```

2. 响应
    * 成功添加请求：`success\r\ninfo:{}\r\n`
    * 重复的请求：`conflict\r\ninfo:{}\r\n`
    * 失败：`failure\r\ninfo:{}\r\n`

<br><br>

### 获取好友申请记录
1. 请求
    ```
    from:{id}\r\n
    ```

2. 响应
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
1. 请求
    ```
    [accept/refuse]\r\n
    {申请记录id}\r\n
    ```
2. 响应
    * 成功 `success\r\ninfo:{}\r\n`
    * 失败 `failure\r\ninfo:{}\r\n`

<br><br>

### 获取好友列表
1. 请求
    ```
    from:{id}\r\n
    ```
2. 响应
    * 成功
        ```
        {好友id}|{好友邮箱}\r\n
        ...
        ```
    * 失败：`failure\r\ninfo:{}\r\n`

<br><br>

### 获取文件夹内容
1. 请求
    ```
    path:{路径}\r\n
    ```
2. 响应
    * 成功：
        ```
        {文件名}|{文件类型，0代表文件夹，1代表其它（0/1）}
        ...
        ```
    * 失败：`failure\r\ninfo:{}\r\n`

<br><br>

### 创建文件夹
1. 请求
    ```
    path:{}\r\n
    name:{}\r\n
    ```
2. 响应
    * 成功 `success\r\ninfo:{}\r\n`
    * 失败 `failure\r\ninfo:{}\r\n`

<br><br>

### 重命名文件/文件夹
1. 请求
    ```
    path:{}\r\n
    newName:{}\r\n
    ```
2. 响应
    * 成功 `success\r\ninfo:{}\r\n`
    * 失败 `failure\r\ninfo:{}\r\n`

<br><br>

### 删除文件/文件夹
1. 请求
    ```
    path:{}\r\n
    ```
2. 响应
    * 成功 `success\r\ninfo:{}\r\n`
    * 失败 `failure\r\ninfo:{}\r\n`