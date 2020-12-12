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