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
        {文件名}|{文件类型，0代表文件夹，1代表其它（0/1）}
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
        {文件名}|{文件类型，0代表文件夹，1代表其它（0/1）}
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

### 文件上传
1. 请求
    * start（开始上传）
        ```
        start\r\n
        {文件名}\r\n
        {保存路径}\r\n
        ```
    * next（上传下一个分块）
        ```
        next\r\n
        {数据}
        ```
    * pause（暂停上传）
        ```
        pause\r\n
        ```
    * cont（继续上传）
        ```
        cont\r\n
        {文件路径}
        ```
    * finsh（上传完成）
        ```
        finsh\r\n
        ```

2. 响应
    * start `ready\r\n`
    * next `recv\r\n`
    * pause `recv\r\n`
    * cont `ready\r\n`
    * finsh `recv\r\n`
    * 出错 `failure\r\nstatus:{}\r\n`