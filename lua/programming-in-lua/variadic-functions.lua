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
