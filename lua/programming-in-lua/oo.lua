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