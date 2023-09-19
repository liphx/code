function mkcd() {
    dir="$1";
    mkdir -p "${dir}";
    cd "${dir}";
}

function mktmp() {
    mkcd "tmp-`date "+%Y%m%d%H%M%S"`"
}

function format_unix() {
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

function extension_name {
    name=$(basename $1)
    printf "%s\n" "${name##*.}"
}

function format() {
    for file; do
        extension=$(extension_name $file)
        case $extension in
            h|c|cc|cpp) clang-format -i $file;;
            py) autopep8 -i $file;;
            go) gofmt -w $file;;
            *) echo "no format tools for $file";;
        esac
    done
}

function compress() {
    pack=$1
    dir=$2
    case $pack in
        *.zip) zip -r $pack $dir;;
        *.tar.gz|*.tgz) tar cjvf $pack $dir;;
        *) echo "no compress tools for $pack";;
    esac
}

function uncompress() {
    file=$1
    case $file in
        *.zip) unzip $file;;
        *.tar.gz|*.tgz|*.tar.bz2) tar xvf $file;;
        *) echo "no uncompress tools for $file"
    esac
}

function terminfo() {
    echo "`tput cols`x`tput lines` `tput longname`"
}
