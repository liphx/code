s = "hello world"
i, j = string.find(s, "hello")
print(i, j) --> 1 5
print(string.sub(s, i, j)) --> hello

-- string.find("a [word]", "[")
-- error [在模式中有特殊意义
print(string.find("a [word]", "[", 1, true)) --> 3 3
-- 第三个参数是查找的起始下标，第四个参数是是否忽略模式

print(string.match("hello world", "hello")) --> hello
date = "Today is 17/7/1990"
d = string.match(date, "%d+/%d+/%d+")
print(d) --> 17/7/1990

s = string.gsub("Lua is cute", "cute", "great")
print(s) --> Lua is great
s = string.gsub("all lii", "l", "x")
print(s) --> axx xii
s = string.gsub("all lii", "l", "x", 1)
print(s) --> axl lii
-- 第四个参数限制替换的字符串数量

s = "some string"
words = {}
for w in string.gmatch(s, "%a+") do
    words[#words + 1] = w
end
print(table.unpack(words)) --some string

function expand (s)
    return (string.gsub(s, "$(%w+)", _G))
end
   
name = "Lua"; status = "great"
print(expand("$name is $status, isn't it?")) --> Lua is great, isn't it?