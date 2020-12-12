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