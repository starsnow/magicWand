# magicWand

代码是针对 arduino Micro 写的，Leonardo 也可以用，其它的根据情况要把 Serial1 改为 Serial 来使用，有两处，一处是 ino 文件的 setup() 中初始始化串口，另一处是 tuyaFunction.cpp 中 tuya_device 的定义处。加了条件编译，所以也可以不用管了，只是如果有没有考虑到的或者要改其它串口的，可以在这两处改。