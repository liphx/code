**tcmalloc**

```
#0  tcmalloc::SLL_Next (t=0x0) at src/linked_list.h:45
#1  tcmalloc::SLL_PopRange (end=<synthetic pointer>, start=<synthetic pointer>, N=32, head=0x3363920) at src/linked_list.h:88
#2  tcmalloc::ThreadCache::FreeList::PopRange (end=<synthetic pointer>, start=<synthetic pointer>, N=32, this=0x3363920)
    at src/thread_cache.h:238
#3  tcmalloc::ThreadCache::ReleaseToCentralCache (this=this@entry=0x3363880, src=src@entry=0x3363920, cl=cl@entry=5, N=3025)
    at src/thread_cache.cc:201
#4  0x00007f61a6413226 in tcmalloc::ThreadCache::Scavenge (this=0x3363880) at src/thread_cache.cc:224
#5  0x00007f619a8f3a35 in exit () from /lib64/libc.so.6
#6  0x00007f619a8dcd27 in __libc_start_main () from /lib64/libc.so.6
#7  0x00000000004f579d in _start ()
```

R: double free 或其他内存问题
