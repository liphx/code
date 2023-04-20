```
do
end -- 相当于c中的代码段{}，有局部变量作用域
```


```
if .. then .. elseif .. then .. else .. end
while .. do .. end
repeat .. until ..
for var = exp1, exp2, exp3 do .. end -- exp3是步长，不提供则为1，三个表达式都只计算一次，var是局部变量
```


```
goto name
::name::
```

```
模式匹配
.   所有字符
%a  字母
%c  控制字符
%d  数字
%g  除空格外的可打印字符
%l  小写字母
%p  标点字符
%s  空白字符
%u  大写字母
%w  字母数字字符
%x  十六进制数字
字母取大写则表示取反 %A 匹配除字母以外的字符

[] 里面是字符集 character class
[0-7] 01234567
[a-zA-Z] 字母
[01] 二进制
[^\n] 除换行符
[%A] = [^%a]
+ 1个或多个
* 0个或多个 最长匹配
- 0个或多个 最短匹配，例如消除C语言注释'/%*.-%*/'
? 0或1

%a+ 一个单词
%d+ 一个整数
[+-]?%d+ 正负数，且符号可选

模式以^开头，匹配的子串是字符串的开头
模式以$结尾，匹配的子串是字符串的结尾

'%bxy' 匹配以x开头以y结尾的字符串
```

```
Precompiled Code
$ luac -o prog.lc prog.lua
$ lua prog.lc
```


## assert-error.lua

```lua
local b = 1 < 0
-- assert(b, "false")
--[[
local file, msg
repeat
    print "enter a file name:"
    local name = io.read()
    if not name then return end -- no input
    file, msg = io.open(name, "r")
    if not file then print(msg) end
until file

local name = io.read()
file = assert(io.open(name, "r"))
 --> stdin:1: no-file: No such file or directory
]]--

function f (a, b)
    if b == 0 then
        error("wrong")
    else return a / b end
end

local ok, msg = pcall(function () f(1, 0) end)
print(ok, msg)      -->false   .\assert-error.lua:20: wrong
ok, msg = pcall(function () f(0, 1) end)
print(ok, msg)      -->true    nil
```

## bit-byte.lua

```lua
print(string.format("%x", 0xff & 0xabcd)) --> cd
print(string.format("%x", 0xff | 0xabcd)) --> abff
print(string.format("%x", 0xaaaa ~ -1)) --> ffffffffffff5555
print(string.format("%x", ~0)) --> ffffffffffffffff

print(0x7fffffffffffffff < 0x8000000000000000) --false
--unsigned less than
print(math.ult(0x7fffffffffffffff, 0x8000000000000000)) --true
mask = 0x8000000000000000
print((0x7fffffffffffffff ~ mask) < (0x8000000000000000 ~ mask)) -- true

s = string.pack("iii", 48, 49, 65)
print(s) --0   1   A
print(#s)
print(string.unpack("iii", s)) -->48      49      65      13
print(string.pack("bbb", 48, 49, 65)) --> 01A
```

## closures.lua

```lua
f = function ()
    print("func")
end
--等价于function f() print("func") end

f() -- func

t = { 1, 2, 3, 4 }
table.sort(t, function(x, y) return x > y end )
print(table.unpack(t)) --> 4 3 2 1

-- 求导函数
function derivative (f, delta)
    delta = delta or 1e-4
    return function (x)
        return (f(x + delta) - f(x))/delta
    end
end

cosx = derivative (math.sin)
print(math.cos(0.9), cosx(0.9))
-->0.62160996827066
-->0.62157080088943

function fc ()
    x = 20
    local y
    y = 10 --y is local
end

print(x, y) -- nil nil
fc()
print(x, y) -- 20 nil

local fact = function (n)
    if n == 0 then return 1
    else return n * fact(n-1) -- buggy
    end
end

-- print(fact(5)) error

local fact2
fact2 = function (n)
    if n == 0 then return 1
    else return n * fact2(n-1)
    end
end

print(fact2(5))  -- 120

function f1 ()
    function f2()
    end
end

-- f2() --error

-- 非局部变量 & 闭包
function newCounter ()
    local count = 0
    return function () --匿名函数
               count = count + 1
               return count
           end
end

c1 = newCounter()
c2= newCounter()
print(c1()) --> 1
print(c1()) --> 2
print(c2()) --> 1
print(c2()) --> 2
print(c1()) --> 3
```

## coroutine.lua

```lua
co = coroutine.create(function () print("coroutine") end)
print(type(co))             --> thread
print(coroutine.status(co)) --> suspended
--从挂起到运行
coroutine.resume(co)        --> coroutine
--死亡
print(coroutine.status(co)) --> dead


co = coroutine.create(function ()
    for i = 1, 3 do
        print("co", i)
        coroutine.yield()
    end
end)

coroutine.resume(co)        -->co      1
print(coroutine.status(co)) --> suspended
coroutine.resume(co)        -->co      2
coroutine.resume(co)        -->co      3

coroutine.resume(co)        -- 无打印，结束最后的循环

------------消费者-生产者-----------------
function receive (prod)
    local status, value = coroutine.resume(prod)
    return value
end

function send (x)
    coroutine.yield(x)
end

function producer ()
    return coroutine.create(function ()
        while true do
            local x = io.read() -- produce new value
            send(x)
        end
    end)
end

function filter (prod)
    return coroutine.create(function ()
        for line = 1, math.huge do
            local x = receive(prod) -- get new value
            x = string.format("%5d %s", line, x)
            send(x) -- send it to consumer
        end
    end)
end

function consumer (prod)
    while true do
        local x = receive(prod) -- get new value
        io.write(x, "\n") -- consume new value
    end
end

-- consumer(filter(producer()))
```

## data-structure.lua

```lua
----------------- array --------------------
local array = {} -- new array
for i = 1, 1000 do
    array[i] = 0
end

print(#array) --> 1000

array = {}
for i = -5, 5 do
    array[i] = 0
end

print(#array) --> 5

squares = {1, 4, 9, 16, 25, 36, 49, 64, 81}
print(#squares) -->9

N = 10
M = 5
local mt = {} -- create the matrix
for i = 1, N do
    local row = {} -- create a new row
    mt[i] = row
    for j = 1, M do
        row[j] = 0
    end
end

---------------------linked list -------------------------
list = { val = 0, next = nil }
list2 = { val = 1, next = list }
local l = list2
while l do
    print(l.val)
    l = l.next
end

------------------------queue ---------------------------------
function listNew ()
    return {first = 0, last = -1}
end

function pushFirst (list, value)
    local first = list.first - 1
    list.first = first
    list[first] = value
end

function pushLast (list, value)
    local last = list.last + 1
    list.last = last
    list[last] = value
end

function popFirst (list)
    local first = list.first
    if first > list.last then error("list is empty") end
    local value = list[first]
    list[first] = nil -- to allow garbage collection
    list.first = first + 1
    return value
end

function popLast (list)
    local last = list.last
    if list.first > last then error("list is empty") end
    local value = list[last]
    list[last] = nil -- to allow garbage collection
    list.last = last - 1
    return value
end

--------------------string buffer -------------------------
local t = {}
for line in io.lines() do
    t[#t + 1] = line
end
s = table.concat(t, "\n") .. "\n"
```

## date-time.lua

```lua
print(os.time())                         -->1568699153
print(os.time({year = 2019, month = 9, day = 17, hour = 13, min = 45, sec = 0}))        -->1568699100

t = os.date("*t", os.time())
for k, v in pairs(t) do
    print(k, v)
end

--[[
isdst   false
min     52
yday    260
sec     29
wday    3
year    2019
month   9
day     17
hour    13
]]--

t = os.time()
print(os.date("%Y-%m-%d", t))  -->2019-09-17

t = os.date("*t") -- get current time
t1 = os.time(t)
t.day = t.day +1 -- 2019-09-18
t2 = os.time(t)
print(os.difftime(t2, t1)/(60 * 60)) --24.0

print(t.sec) -- 43
t.sec = t.sec + 120
print(t.sec) -- 163

--cpu time
local x = os.clock()
local s = 0
for i = 1, 100000000 do s = s + i end
print(string.format("elapsed time: %.2f\n", os.clock() - x)) --local x = os.clock()

```

## environment.lua

```lua
for n in pairs(_G) do
    print(n)
end

i = 1
name = "abc"
f = print

print(_G["i"], _G["name"], _G["f"])  -->1       abc     function: 00000000684989c0

local print, sin = print, math.sin
local env = _ENV
_ENV = nil
print(sin(0))           -->0.0
-- print(math.cos(0))   -->error
-- print(i)             -->error
-- print(_G["i"])       -->error

_ENV = env
print(_ENV.math.cos(0)) -->1.0
local i = 2
print(_ENV.i, _G.i, i)  -->1 1 2

local g = _G
_G = nil
print(_ENV.i)           -->1
-- print(_G.i)          -->error
j = 1000
_G = g
print(_G.j)             -->1000
print(env.j)            -->1000
```

## garbage.lua

```lua
-------------weak table --------------------
a = {}
mt = {__mode = "k"} --关键字是弱类型
setmetatable(a, mt) -- now 'a' has weak keys
key = {} -- creates first key
a[key] = 1
key = {} -- creates second key
a[key] = 2
collectgarbage() -- forces a garbage collection cycle
for k, v in pairs(a) do print(v) end --> 2
--string number boolean 是值不是对象

-------------gc ---------------------
o = {x = "hi"}
setmetatable(o, {__gc = function (o) print(o.x) end})
o = nil
collectgarbage() --> hi

A = {x = "this is A"}
B = {f = A}
setmetatable(B, {__gc = function (o) print(o.f.x) end})
A, B = nil
collectgarbage() --> this is A
```

## hello.lua

```lua
print("hello, world.")
```

## io.lua

```lua
-- stdout
io.write("hello, world\n")
name = "Abc"
age = 20
io.write(string.format("name is %s, age is %d. \n", name, age))
io.write(233) -- 转换成字符串
io.write('\n')

-- stdin
num = io.read("n") -- 读一个数
char = io.read(1) -- 读一个字符
line = io.read("l") -- 读下一行，并丢弃换行符，io.read()默认参数
Line = io.read("L") -- 读下一行，并保留换行符
all = io.read("a") -- 读全文


local f = assert(io.open("filename", "r"))
local t = f:read("a")
f:close()


local temp = io.input() -- 保存当前流
print(temp)         -- file
print(type(temp))   -- userdata
io.input("newinput")    -- 打开新的流
io.input():close()      -- 关闭当前流
io.input(temp)          -- 恢复之前的流

--[[ functions

print()
io.read()
io.write()
io.lines()
io.open(filename, mode)
io.tmpfile()
io.flush()
file:setvbuf()
file:seek()
os.rename()
os.remove()
os.exit()
os.getenv()
os.execute()
io.popen()

]]--
```

## iterator.lua

```lua
function values (t)
    local i = 0
    return function () i = i + 1; return t[i] end
end

a = { 1, 2, 3, key = 4 }
for i in values(a) do
    print(i)
end
---1 2 3

for k, v in pairs(a) do
    print(k, v)
end
---1 1 2 2 3 3 key 4

for i, v in ipairs(a) do
    print(i, v)
end
---1 1 2 2 3 3
```

## load.lua

```lua
i = 32
local i = 0
f = load("i = i + 1; print(i)")
g = function () i = i + 1; print(i) end
f() --> 33
g() --> 1

--[[ file 'foo.lua'
function foo (x)
    print(x)
end
]]--

f = loadfile("foo.lua")
print(foo) --> nil
f() -- run the chunk
foo("ok") --> ok
```

## metatable.lua

```lua
t = {}
print(getmetatable(t)) --> nil
t1 = {}
setmetatable(t, t1)
print(getmetatable(t) == t1) --> true
setmetatable(t, t)
print(getmetatable(t) == t) --> true

print(getmetatable("string"))   -->table: 000000000071aa40
print(getmetatable(1))          -->nil
print(getmetatable(print))      -->nil

a = {}
print(a)
--等价于
print(tostring(a))


function readOnly (t)
    local proxy = {}
    local mt = { -- create metatable
        __index = t,
        __newindex = function (t, k, v)
            error("attempt to update a read-only table", 2)
        end
    }
    setmetatable(proxy, mt)
    return proxy
end

b = readOnly({1, 2, 3, 4})
print(b[1])     -->1
b[2] = 20       -->error, attempt to update a read-only table
```

## most-frequent-words.lua

```lua
local counter = {}

for line in io.lines() do
    for word in string.gmatch(line, "%w+") do
        counter[word] = (counter[word] or 0) + 1
    end
end

local words = {} -- list of all words found in the text

for w in pairs(counter) do
    words[#words + 1] = w
end

table.sort(words, function (w1, w2)
        return counter[w1] > counter[w2] or
            counter[w1] == counter[w2] and w1 < w2
    end)

-- number of words to print
local n = math.min(tonumber(arg[1]) or math.huge, #words)

for i = 1, n do
    io.write(words[i], "\t", counter[words[i]], "\n")
end
```

## oo.lua

```lua
A = {
    i = 100,
    f = function (self)
        print(self.i)
    end
}

A:f()   -->100
A.f(A)  -->100

function A:g(i)
    self.i = i
end

A:g(200)
A:f()   -->200

local mt = { __index = A }

--基于原型创建对象
function A.new(o)
    o = o or {} --如果不提供则创建一个空表
    setmetatable(o, mt)
    return o
end

a = A.new()
a:f()       -->200
b = A.new({i = 300})
b:f()       -->300
c = A.new()
getmetatable(c).__index:g(400)  --getmetatable(c).__index 得到 A
getmetatable(c).__index.f(c)    -->400

--不以mt作为o的metatable
function A:new(o)
    o = o or {}
    self.__index = self
    setmetatable(o, self)
    return o
end

a1 = A:new()
a1:f()  -->400
a1:g(500)
a1:f()  -->500
a2 = A:new()
a2:f()  -->400

--------------继承-----------------
aa = a:new()
aa:f()  -->400
----重写a的f()函数
function a:f()
    print(self.i + 100)
end
aa:f()  -->500
```

## pattern-matching.lua

```lua
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
```

## reflection.lua

```lua
function traceback ()
    for level = 1, math.huge do
        local info = debug.getinfo(level, "Sl")
        if not info then break end
        if info.what == "C" then -- is a C function?
            print(string.format("%d\tC function", level))
        else -- a Lua function
            print(string.format("%d\t[%s]:%d", level,
            info.short_src, info.currentline))
        end
    end
end

traceback()

function foo (a, b)
    local x
    do local c = a - b end
    local a = 1
    while true do
    local name, value = debug.getlocal(1, a)
    if not name then break end
        print(name, value)
        a = a + 1
    end
end

foo(10, 20)
```

## require.lua

```lua
local m = require("math")
print(m.abs(-100)) -->100
print(m.huge) -->inf

local abs = require("math").abs
print(abs(-100)) -->100 可以单独导入函数

local inf = require("math").huge
print(inf) -->inf

local M = {} -- the module

-- creates a new complex number
local function new (r, i)
    return {r=r, i=i}
end

M.new = new -- add 'new' to the module

-- constant 'i'
M.i = new(0, 1)

function M.add (c1, c2)
    return new(c1.r + c2.r, c1.i + c2.i)
end

function M.tostring (c)
    return string.format("(%g,%g)", c.r, c.i)
end

return M
```

## serialization.lua

```lua

function serialize (o)
    local t = type(o)
    if t == "number" or t == "string" or t == "boolean" or t == "nil" then
        io.write(string.format("%q", o))
    elseif t == "table" then
        io.write("{\n")
        for k,v in pairs(o) do
            io.write(" ", k, " = ")
            serialize(v)
            io.write(",\n")
        end
        io.write("}\n")
    else
        error("cannot serialize a " .. type(o))
    end
end

serialize{a=12, b='Lua', key='another "one"'}
```

## variadic-functions.lua

```lua
function add (...)
    local s = 0
    for _, v in ipairs{...} do
        s = s + v
    end
    return s
end

print(add(1, 2, 3)) --> 6
print(add(1, 2, nil))       -->3
print(add(nil, 1, 2))       -->0

function varf(...)
    print(...)
    print({...})
    print(type({...})) --> table
end

varf(1, 2, 3)

function nonils (...)
    local arg = table.pack(...)
    -- print(type(arg))     -->table
    for i = 1, arg.n do
        if arg[i] == nil then
            return false
        end
    end
    return true;
end

print(nonils(1, 1, nil))    -->false
print(nonils(not nil))      -->true

print(select(1, "a", "b", "c")) --> a b c
print(select(2, "a", "b", "c")) --> b c
print(select(3, "a", "b", "c")) --> c
print(select("#", "a", "b", "c")) --> 3

function add2 (...)
    local s = 0
    for i = 1, select("#", ...) do
        s = s + select(i, ...)
    end
    return s
end

print(add2(1, 2, 3))        -->6

print(table.unpack{10, 20, 30})         --> 10 20 30
-- a, b = table.unpack{10, 20, 30}      -- a=10, b=20, 30 is discarded
print(table.unpack({"Sun", "Mon", "Tue", "Wed"}, 2, 3)) -->Mon     Tue

```

