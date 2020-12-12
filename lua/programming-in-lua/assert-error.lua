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