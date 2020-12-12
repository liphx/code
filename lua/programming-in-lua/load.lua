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