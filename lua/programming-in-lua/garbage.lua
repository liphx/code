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