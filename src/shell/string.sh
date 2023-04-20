################################################################
# 常用字符串函数，参考
# <https://github.com/dylanaraps/pure-bash-bible>
################################################################

string_trim() {
    # : 会进行参数扩展且将临时变量存在 $_
    # % %% # ## 参考参数扩展
    : "${1#"${1%%[![:space:]]*}"}"
    : "${_%"${_##*[![:space:]]}"}"
    printf '%s\n' "$_"
}

string_trim_all() {
    set -f      # 禁用文件名扩展
    set -- $*   # 将 -- 后的内容设为当前参数
    printf '%s\n' "$*"
    set +f
}

string_trim_quotes() {
    : "${1//\'}"
    printf '%s\n' "${_//\"}"
}

string_regex() {
    [[ $1 =~ $2 ]] && printf '%s\n' "${BASH_REMATCH[1]}"
}

string_split() {
   # Usage: split "string" "delimiter"
   IFS=$'\n' read -d "" -ra arr <<< "${1//$2/$'\n'}"
   printf '%s\n' "${arr[@]}"
}

string_lower() {
    printf '%s\n' "${1,,}"
}

string_upper() {
    printf '%s\n' "${1^^}"
}

string_reverse_case() {
    printf '%s\n' "${1~~}"
}

string_strip() {
    printf '%s\n' "${1/$2}"
}

string_lstrip() {
    # Strip pattern from start of string
    printf '%s\n' "${1##$2}"
}

string_rstrip() {
    # Strip pattern from end of string
    printf '%s\n' "${1%%$2}"
}

string_strip_all() {
    printf '%s\n' "${1//$2}"
}

string_urlencode() {
    # Usage: urlencode "string"
    local LC_ALL=C
    for (( i = 0; i < ${#1}; i++ )); do
        : "${1:i:1}"
        case "$_" in
            [a-zA-Z0-9.~_-])
                printf '%s' "$_"
            ;;

            *)
                printf '%%%02X' "'$_"
            ;;
        esac
    done
    printf '\n'
}

string_urldecode() {
    # Usage: urldecode "string"
    : "${1//+/ }"
    printf '%b\n' "${_//%/\\x}"
}

_string_test() {
    string_trim '  hello   world '          # hello   world
    string_trim_all '  hello   world '      # hello world
    string_trim_quotes '"abc"'              # abc
    string_regex '100abc' '([0-9]+)'        # 100
    string_split '1.2.3.4' '.'              # 1 2 3 4
    string_lower 'Abc'                      # abc
    string_upper 'Abc'                      # ABC
    string_reverse_case 'Abc'               # aBC
    string_strip 'abc123abc' 'abc'          # 123abc
    string_strip_all 'abc123abc' 'abc'      # 123
}

# _string_test
