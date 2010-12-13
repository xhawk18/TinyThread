1. TinyThread特性
   多线程
      时间片轮转算法
      抢占式多任务
      Mutex能够克服优先级翻转
   线程特性
      定义线程数，无限制
      活动线程数，无限制
      用户优先级，无限制
      线程堆栈检测，支持
      上下文切换时间，
         不支持线程sleep函数和堆栈检测时，< 300 CPU周期
         支持线程sleep函数和堆栈检测时，< 400 CPU周期
   时间
      延迟，通过tt_sleep()支持
      微秒级延迟，通过tt_usleep()支持
   线程交互
      信号量（Semaphore），无个数限制
      锁（Mutex），无个数限制
      递归锁（Recursive Mutex)，无个数限制
      条件变量（Condition），无个数限制
      邮箱(Message)，无个数限制
   线程/中断交互
      信号量/锁/递归锁/条件变量，均可用在中断里。
      (有限制的支持，请参考Doc\TinyThread.doc)

2. 内存使用状况
   一个任务需要的RAM
      堆栈大小 + 68 Bytes(最大)
      堆栈大小 + 28 Bytes(最小)
   一个信号量需要的RAM, 8 Bytes
   一个锁需要的RAM Space for a Mutex, 8 Bytes
   一个递归锁需要的RAM Space for a Recursive Mutex, 16 Bytes
   一个条件变量需要的RAM Space for a Condition, 12 Bytes
   一个邮箱消息使用RAM：40字节 + 自定义消息缓存

3. 文档资料
   Doc\TinyThread.doc

4. 关于BSP（基本开发库）
   本软件库包含了部分 NUC100、M051 BSP的代码，
   如果您需要取得上述两个BSP库的完全版本，可以访问：
       http://www.nuvoton.com/NuMicro


