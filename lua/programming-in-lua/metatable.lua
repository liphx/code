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