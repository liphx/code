# script scriptreplay

```shell
$ script -t 2> timing.log -a output.session
$ scriptreplay timing.log output.session
```

`script [options] [file]`

Make a typescript of a terminal session.

```
Options:
 -a, --append                  append the output
 -c, --command <command>       run command rather than interactive shell
 -e, --return                  return exit code of the child process
 -f, --flush                   run flush after each write
     --force                   use output file even when it is a link
 -o, --output-limit <size>     terminate if output files exceed size
 -q, --quiet                   be quiet
 -t[<file>], --timing[=<file>] output timing data to stderr or to FILE
 -h, --help                    display this help
 -V, --version                 display version
```

`scriptreplay [-t] timingfile [typescript] [divisor]`

Play back terminal typescripts, using timing information.

```
Options:
 -t, --timing <file>     script timing output file
 -s, --typescript <file> script terminal session output file
 -d, --divisor <num>     speed up or slow down execution with time divisor
 -m, --maxdelay <num>    wait at most this many seconds between updates
 -h, --help              display this help
 -V, --version           display version
```
