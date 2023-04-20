- http://unpbook.com/
- https://github.com/unpbook/unpv13e.git

```
-       if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
+       if ( (sockfd = socket(9999, SOCK_STREAM, 0)) < 0)

Address family not supported by protocol
```

```diff
+++ intro/daytimetcpcli.c
+    int count = 0;
        while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
+        count++;
                recvline[n] = 0;        /* null terminate */
                if (fputs(recvline, stdout) == EOF)
                        err_sys("fputs error");
        }
+    printf("count = %d\n", count);
        if (n < 0)
                err_sys("read error");

+++ intro/daytimetcpsrv.c
         ticks = time(NULL);
         snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
-        Write(connfd, buff, strlen(buff));
+        int n = strlen(buff);
+        for (int i = 0; i < n; i++) {
+            Write(connfd, buff + i, 1);
+        }

                Close(connfd);
        }
```

```shell
$ ./daytimetcpcli 127.0.0.1
Sat Dec 24 22:45:52 2022
count = 1
```