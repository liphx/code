GIT_ROOT_PATH=$(git rev-parse --show-toplevel)
GIT_HELPER_PATH=$GIT_ROOT_PATH/.git_helper
cd $GIT_ROOT_PATH

if [ ! -d $GIT_HELPER_PATH ]; then
    mkdir $GIT_HELPER_PATH
fi

git config color.status always

pids=$(jobs -p)
if [ -n "$pids" ]; then
    kill $pids
fi

nohup bash -c "while true; do                                       \
    date +%T > $GIT_HELPER_PATH/timestamp;                          \
    echo '========' >> $GIT_HELPER_PATH/timestamp;                  \
    git status -s > $GIT_HELPER_PATH/status;                        \
    git log --color=always --oneline -n 5 > $GIT_HELPER_PATH/log;   \
    git branch --color=always > $GIT_HELPER_PATH/branch;            \
    sleep 30;                                                       \
done" >/dev/null 2>&1 &

alias gs='cat $GIT_HELPER_PATH/timestamp $GIT_HELPER_PATH/status'
alias gl='cat $GIT_HELPER_PATH/timestamp $GIT_HELPER_PATH/log'
alias gb='cat $GIT_HELPER_PATH/timestamp $GIT_HELPER_PATH/branch'
