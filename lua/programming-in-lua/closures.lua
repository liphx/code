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