# tmux

## tmux 的功能

1. 分屏: 多个窗口，窗口分屏
2. detach, attach
3. 高效操作文本
4. 日志记录
5. 屏幕共享

## 命令速记

$ for shell, ! for PREFIX(default CTRL-b), : for command mode

```
$ tmux -V                       # display version

# session
$ tmux new-session              # Creates a new session without a name. Can be shortened to tmux new or simply tmux.
$ tmux new -s <name>            # Create named session
$ tmux new -s <s> -n <w>        # Create named session and name first window
$ tmux new -s <name> -d         # create new session, detach
! d                             # detach session
$ tmux list-sessions            # list sessions, shortened to tmux ls
$ tmux attach                   # attach session, shortened to tmux at
$ tmux attach -t <session>      # attach session
$ tmux kill-session -t <s>      # kill session
! (                             # goto previous session
! )                             # goto next session
! s                             # list sessions

# window
! c                             # create new window
! ,                             # rename window
! .                             # move window
! n                             # next window
! p                             # previous window
! [0-9]                         # go to window
! w                             # select window
! f                             # find and select window
! &                             # close window
! !                             # Turning a Pane into a Window
: join-pane -s <src> -t <dst>   # take <src> join to <dst>, default is current window
                                # the form of <src> is `[session_name]:[window].[pane]`
: move-window -s <src> -t <dst> # move window

# pane
! %                             # vertically split
! "                             # horizontally split
! q                             # display pane number
! o                             # move to pane
! <UP> <DOWN> <LEFT> <RIGHT>    # move to pane
! <SPACE>                       # change pane layout
! z                             # Maximizing and Restoring Panes
! x                             # close pane, close window if no pane left
$ exit                          # close pane, close window if no pane left, close session if no window left

! :                             # command mode
: new-window -n <w> "proc"      # create named window run proc

! ?                             # help
```

## tmuxinator

```shell
$ tmuxinator open <name>
$ tmuxinator <name>
$ tmuxinator debug <name>
```

## Copy Mode

~ for copy mode

```
! [                             # copy mode
~ ENTER                         # quit copy mode
~ w b <C-f> <C-b> h j k l f F G / ? n N     # the same as vi, use `g` goto top
~ SPACE <select text> ENTER     # copy text
! ]                             # paste text
: capture-pane                  # capture pane into paste buffer
$ tmux show-buffer              # display buffer
$ tmux save-buffer <path>       # save buffer to file
$ tmux list-buffers             # list buffers
$ tmux choose-buffer            # choose buffer
! =                             # choose buffer
```

## Pair Programming

### Pairing with a Shared Account

1. `tmux new-session -s Pairing`
2. `tmux attach -t Pairing`

### Using a Shared Account and Grouped Sessions

1. `tmux new-session -s groupedsession`
2. `tmux new-session -t groupedsession -s mysession`

### [tmate](https://tmate.io/)

```
$ tmate
$ tmux show-messages        # Displays a log of messages in the current window
```

### Pairing with Separate Accounts and Sockets

1. `tmux -S <socket-path>`
2. `tmux -S <socket-path> attach`

```shell
groupadd tmux-shared
usermod -aG tmux-shared user1
usermod -aG tmux-shared user2

tmux -S /tmp/tmux-shared
chgrp tmux-shared /tmp/tmux-shared
tmux -S /tmp/tmux-shared attach
```
