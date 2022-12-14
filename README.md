# Network Labs

## Lab1

完成本地的Server和Client之间的聊天程序。

## Lab2

使用Wireshark进行抓包，并分析TCP协议的“三次握手，四次挥手”过程。

## Lab3

基于UDP服务设计可靠传输协议并编程实现

### 实验3-1

利用数据报套接字在用户空间实现面向连接的可靠数据传输，功能包括：建立连接、差错检测、确认重传等。流量控制采用停等机制，完成给定测试文件的传输。

+ UDP
+ 建立连接：实现类似TCP的握手、挥手功能（次数不限定）
+ 差错检验：计算校验和
+ 确认重传：rdt2.0,rdt2.1,rdt3.0等，也可以自己设计协议
+ 单向传输：
  + 发送端：需要传图片和txt测试文件
  + 接收端：事先把接收端文件清空
+ 日志输出：log
  + 输入发送报文的长度，序列号seg,ACK,SYN,Fin,checksum 
  + 文件传输时间，t率

### 实验3-2

在实验3-1的基础上，将停等机制改成基于滑动窗口的流量控制机制，采用固定窗口大小，支持累积确认，完成给定测试文件的传输。

+ 多个序列号
+ 发送缓冲区、接收缓冲区
+ 日志输出：发送端接收端的窗口情况（多少发送了，多少没有发送）

+ 需要把ip写死（通过路由器进行转发）
+ 文件传输速率不计入最终成绩

### 实验3-3

在实验3-2的基础上，选择实现一种拥塞控制算法，也可以是改进的算法，完成给定测试文件的传输。

+ 推荐实现RENO算法
+ 也可以自己设计协议或实现拥塞控制算法
+ 给出实现的拥塞
+ 日志输出：需要输出拥塞窗口的大小

### 实验3-4

基于给定的实验测试环境，通过改变延迟时间和丢包率，完成下面3组性能对比实验：

1. 停等机制与滑动窗口机制性能对比
2. 滑动窗口机制中不同窗口大小对性能的影响；
3. 有拥塞控制和无拥塞控制的性能比较。

+ 控制变量法
+ visualize对比结果（bar graph等）