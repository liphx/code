function mkcd() {
    dir="$1";
    mkdir -p "${dir}";
    cd "${dir}";
}

function format-unix() {
    for file; do
        if [ -f $file ]; then
            dos2unix "$file"
            enca -L chinese -x utf-8 "$file"
        fi
    done
}

function find_command() {
    if command -v $1 > /dev/null 2>&1; then
        return 0
    fi
    return 1
}

function install() {
    if find_command brew; then
        brew install "$@"
    elif find_command apt; then
        sudo apt install -y "$@"
    elif find_command zypper; then
        sudo zypper install -y "$@"
    elif find_command yum; then
        sudo yum install -y "$@"
    fi
}

function update() {
    if find_command apt; then
        sudo apt update && sudo apt upgrade -y
    fi
}

function when() {
    while sleep 1; do
        tput sc
        tput cup 0 $(($(tput cols)-29))
        now
        tput rc
    done &
}
