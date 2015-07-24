# CompressedDumpMaker
Library provides API to create compressed memory dump file. That should speed up time, required to gather memory dump file on large solutions.

Usually, the main bottleneck during memory dump file creation is hard drive. Idea is to use CPU resources and perform compression on the clients size, writting compressed data only.

How it works: 

1. Suspend the target process.
1. Call WinAPIto create dump file _without_ heap data (e.g. [MiniDumpWriteDump](https://msdn.microsoft.com/en-us/library/windows/desktop/ms680360(v=vs.85).aspx))
1. Immediately read the created memory dump file and look for memory regions that should be stored.
1. Creates one more file and store all memory regions to that file, compressing them before.
2. Resume the target process.

Later, library should be used to decompress dump file and construct a regular `*.dmp` file (with memory data stream) 
