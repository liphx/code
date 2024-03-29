# ls
alias ls='ls --color'
alias la='ls -a'
alias ll='ls -alh'

# cd
alias ..='cd ..'
alias 2..='cd ../..'
alias 3..='cd ../../..'
alias 4..='cd ../../../..'
alias 5..='cd ../../../../..'
alias -- -='cd -'

# interactive
alias cp='cp -i'
alias rm='rm -i'
alias mv='mv -i'

# terminal
alias c=clear

# history
alias h=history
alias r='fc -s'
alias hgrep='history | grep'

# alternative
alias diff='colordiff'
alias top='htop'
alias more='less'

# git
alias ga='git add'
alias gb='git branch'
alias gs='git status -s'
alias gr='git remote -v'
alias gc='git commit -m'
alias gc-now='git commit -m "`date \"+%F %T\"`"'
alias glog='git log --date=format:"%F %T"'
alias gl='git log --oneline -n6'
alias glast='git log -n1 --date=format:"%F %T"'
alias gd='git diff'
alias gdc='git diff --cached'
alias gdh='git diff HEAD~1'
alias gk='git checkout'
alias gmv='git mv'
alias clone='git clone'

# vim
alias vi=vim
alias vimrc='vim ~/.vim/vimrc'
alias vim-default='vim -u NONE -N'
alias vim-session='vim -S'
alias sudo-vim='sudo vim -u ~/.vim/vimrc'
alias neovim='nvim'

# shorter
alias pu=pushd
alias po=popd
alias t=tmux

# with args
alias ff='find . -name'
alias j='jobs -l'
alias wget='wget -c'
alias ping='ping -c 4'
alias now='date "+%F %T"'
alias tree='tree --charset=ansi'
alias remote='bash ~/.bash/ssh.sh ~/usr/password.remote'
alias grep='grep --exclude=tags --exclude-dir=.git --color'

# nginx
alias nginxreload='sudo nginx -s reload'
alias nginxtest='sudo nginx -t'

alias update='sudo apt update && sudo apt upgrade -y'
alias clear_file=': | tee'

# python3 module
alias json='python3 -m json.tool --no-ensure-ascii'
alias httpserver='python3 -m http.server'

alias bb='bazel build'
