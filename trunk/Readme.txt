1. TinyThread Specifications
   Multitask
      Round-Robin
      Preemptive
      Can avoid priority inversion by mutex
   Task Specifications
      Define Tasks, Unlimited
      Active Tasks, Unlimited
      User Priorities, Unlimited
      Thread Stack Checking, Supported
      Context Switch Time,
         <300 cycles without thread sleeping and stack checking supported.
         <400 cycles with thread sleeping and stack checking supported.
   Time
      delay, supported by tt_sleep()
      micro delay, supported by tt_usleep()
   Inter-Task Communication
      Semaphores, Unlimited
      Mutexes, Unlimited
      Recursive mutexes, Unlimited
      Conditions, Unlimited
      Mailbox(Message), Unlimited
   Thread/Irq Communication
      Semaphores/Mutexes/Recursive Mutexed/Conditions can be used in Irq.
      (Some functions are limited, see Doc\TinyThread.doc)

2. Memory specifications
   RAM Space for a Task
      TaskStackSize + 68 Bytes(MAX)
      TaskStackSize + 28 Bytes(MIN)
   RAM Space for a Semaphore, 8 Bytes
   RAM Space for a Mutex, 8 Bytes
   RAM Space for a Recursive Mutex, 16 Bytes
   RAM Space for a Condition, 12 Bytes
   RAM Space for a Mailbox, 40 Bytes + user defined message buffer

3. Reference document
   Doc\TinyThread.doc

4. About the BSP
   This library contains part of NUC100 and M051 series BSP.
   To get full BSP libraries, please visit
       http://www.nuvoton.com/NuMicro


