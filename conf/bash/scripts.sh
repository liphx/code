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

function install() {
    if command -v apt; then
        sudo apt install -y "$@"
    elif command -v zypper; then
        sudo zypper install -y "$@"
    elif command -v yum; then
        sudo yum install -y "$@"
    fi
}

function update() {
    if command -v apt; then
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
