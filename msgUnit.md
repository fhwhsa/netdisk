## 通信数据单元内容规范
以下所有的{}代表填充内容

### 登陆
1. 请求
    ```
    email:{}\r\n
    passwd:{}\r\n
    ```
2. 响应
    * 成功  `success\r\nid:{}\r\nstatus:{}\r\n` 
    * 失败  `failure\r\nstatus:{}\r\n`

---

### 注销
1. 请求
	```
	id:{}\r\n	
	```

---

### 查找用户（根据id或邮箱查找）
1. 请求
    ```
    key:{id/邮箱}\r\n
    ```
2. 响应
    * 有该用户 `success\r\nid:{}\r\nemail:{}\r\nstatus:{}\r\n` 
    * 没有该用户或出错 `failure\r\nstatus:{}\r\n`

---

### 好友申请
1. 请求
    ```
    from:{id}\r\n
    to:{id}\r\n
    ```

2. 响应
    * 成功添加请求：`success\r\nstatus:{}\r\n`
    * 重复的请求：`conflict\r\nstatus:{}\r\n`
    * 失败：`failure\r\nstatus:{}\r\n`

---

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

    * 失败：`failure\r\nstatus:{}\r\n`

---

### 验证好友申请
1. 请求
    ```
    [accept/refuse]\r\n
    {申请记录id}\r\n
    ```
2. 响应
    * 成功 `success\r\nstatus:{}\r\n`
    * 失败 `failure\r\nstatus:{}\r\n`

---

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
    * 失败：`failure\r\nstatus:{}\r\n`

---

### 获取文件夹内容
1. 请求
    ```
    path:{路径}\r\n
    ```
2. 响应
    * 成功：
        ```
        {文件名}|{文件类型，0代表文件夹，1代表其它（0/1）}|{字节数}
        ...
        ```
    * 失败：`failure\r\nstatus:{}\r\n`

---

### 创建文件夹
1. 请求
    ```
    path:{}\r\n
    name:{}\r\n
    ```
2. 响应
    * 成功返回文件列表
        ```
        {文件名}|{文件类型，0代表文件夹，1代表其它（0/1）}|{字节数}
        ...
        ```
    * 失败 `failure\r\nstatus:{}\r\n`

---

### 重命名文件/文件夹
1. 请求
    ```
    path:{}\r\n
    newName:{}\r\n
    ```
2. 响应
    * 成功 `success\r\nstatus:{}\r\n`
    * 失败 `failure\r\nstatus:{}\r\n`

---

### 删除文件/文件夹
1. 请求
    ```
    {path}\r\n
    ...
    ```
2. 响应
    * 成功 `success\r\n`
    * 否则返回删除失败的文件名
        ```
        {name}\r\n
        ...
        ```

---

### 创建文件上传任务
1. 请求
    ```
    {文件名}\r\n
    {网盘存储路径}\r\n
    ```
2. 响应
    * 成功 `recv\r\n0\r\n`
    * 失败 `failure\r\nstatus:{}\r\n`
---

### 上传文件数据
1. 请求 `{文件数据}`
2. 响应 
    * 成功 `recv\r\n{读取的字节数}\r\n`
    * 失败 `failure\r\nstatus:{}\r\n`

---

### 上传完成
1. 请求 ``
2. 响应 
    * 成功 `recv\r\n`
    * 失败 `failure\r\nstatus:{}\r\n`

---

### 取消上传
1. 请求 ``
2. 响应
    * 成功 `recv\r\n`
    * 失败 `failure\r\nstatus:{}\r\n`

---

### 暂停上传
1. 请求 ``
2. 响应
    * 成功 `recv\r\n`
    * 失败 `failure\r\nstatus:{}\r\n`

---

### 继续上传
1. 请求 
    ```
    {文件名}\r\n
    {网盘存储路径}\r\n
    ```
2. 响应
    * 成功 `recv\r\n{已上传的字节数}\r\n`
    * 失败 `failure\r\nstatus:{}\r\n`

### 创建文件下载任务
1. 请求 `{文件路径}\r\n`
2. 响应
    ```
    recv\r\n
    {文件大小}\r\n
    ```
---

### 下载文件数据
1. 请求
    ```
    {客户端已接收的数据大小}\r\n
    ```
2. 响应 `{文件数据}`
---

### 取消下载
1. 请求 ``
2. 响应
    * 成功 `recv\r\n`
    * 失败 `failure\r\nstatus:{}\r\n`

---

### 文件下载错误
1. 响应 `status:{}\r\n`
