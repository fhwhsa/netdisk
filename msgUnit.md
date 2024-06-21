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