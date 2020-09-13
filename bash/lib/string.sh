################################################################
# 常用字符串函数，参考
# <https://github.com/dylanaraps/pure-bash-bible>
################################################################


string_trim() {
    # Usage: trim_string "   example   string    "
    : "${1#"${1%%[![:space:]]*}"}"
    : "${_%"${_##*[![:space:]]}"}"
    printf '%s\n' "$_"
}


test() {
    string_trim '  helo asdf  '

}

test