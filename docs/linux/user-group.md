# 用户和组

## `/etc/passwd`

```
liph:x:1000:1000:liph,,,:/home/liph:/bin/bash
1    2 3    4    5       6          7
1: login name
2: encrypted password, /etc/shadow
3: user id(uid), root is 0
4: group id(gid)
5: comment
6: home directory
7: login shell
```

## `/etc/shadow`

```
liph:xxxxx:19171:0:99999:7:::
1    2     3     4 5     6789
1: login name
2: encrypted password
3: date of last password change, expressed as the number of days since Jan 1, 1970.
4: minimum password age
5: maximum password age
6: password warning period
7: password inactivity period
8: account expiration date
9: reserved field
```

## `/etc/group`

```
liph:x:1000:abc,bcd
1    2 3    4
1: group name
2: encrypted password, /etc/gshadow
3: group id(gid), root is 0
4: user list, user name list(not uid), many login names can have the same uid
```

## `函数 getpwnam(), getpwuid(), getgrnam(), getgrgid`

```c
#include <pwd.h>

struct passwd *getpwnam(const char *name);
struct passwd *getpwuid(uid_t uid);

struct passwd {
    char   *pw_name;       /* username */
    char   *pw_passwd;     /* user password */
    uid_t   pw_uid;        /* user ID */
    gid_t   pw_gid;        /* group ID */
    char   *pw_gecos;      /* user information */
    char   *pw_dir;        /* home directory */
    char   *pw_shell;      /* shell program */
};

#include <grp.h>

struct group *getgrnam(const char *name);
struct group *getgrgid(gid_t gid);

struct group {
    char   *gr_name;        /* group name */
    char   *gr_passwd;      /* group password */
    gid_t   gr_gid;         /* group ID */
    char  **gr_mem;         /* NULL-terminated array of pointers to names of group members */
};
```

## 进程凭证

```shell
$ ls -l /bin/sudo
-rwsr-xr-x 1 root root 244632  3月 23 17:38 /bin/sudo
    |
 set-UID
```

当运行 set-user-ID 程序（即通过调用 exec()将 set-user-ID 程序载入进程的内存中）时，内核会将进程的有效用户 ID 设置为可执行文件的用户 ID。

在 Linux 系统中，set-user-ID 和 set-group-ID 权限位对 shell脚本无效。

```c
#include <unistd.h>
uid_t getuid(void);         // return uid
uid_t geteuid(void);        // return effective uid
gid_t getgid(void);         // return gid
gid_t getegid(void);        // return effective gid

int setuid(uid_t uid);
int setgid(gid_t gid);
int seteuid(uid_t euid);
int setegid(gid_t egid);
int setreuid(uid_t ruid, uid_t euid);
int setregid(gid_t rgid, gid_t egid);
// set ((real) effective) uid/gid, 成功返回0，否则-1

// Linux
int getresuid(uid_t *ruid, uid_t *euid, uid_t *suid);
int getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid);
int setresuid(uid_t ruid, uid_t euid, uid_t suid);
int setresgid(gid_t rgid, gid_t egid, gid_t sgid);
// get/set real, effective, saved set uid/gid, 成功返回0，否则-1
```
