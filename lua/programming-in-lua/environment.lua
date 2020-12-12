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