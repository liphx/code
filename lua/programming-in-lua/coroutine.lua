co = coroutine.create(function () print("coroutine") end)
print(type(co))             --> thread
print(coroutine.status(co)) --> suspended
--从挂起到运行
coroutine.resume(co)        --> coroutine
--死亡
print(coroutine.status(co)) --> dead


co = coroutine.create(function ()
    for i = 1, 3 do
        print("co", i)
        coroutine.yield()
    end
end)

coroutine.resume(co)        -->co      1
print(coroutine.status(co)) --> suspended
coroutine.resume(co)        -->co      2
coroutine.resume(co)        -->co      3

coroutine.resume(co)        -- 无打印，结束最后的循环

------------消费者-生产者-----------------
function receive (prod)
    local status, value = coroutine.resume(prod)
    return value
end
   
function send (x)
    coroutine.yield(x)
end
   
function producer ()
    return coroutine.create(function ()
        while true do
            local x = io.read() -- produce new value
            send(x)
        end
    end)
end
   
function filter (prod)
    return coroutine.create(function ()
        for line = 1, math.huge do
            local x = receive(prod) -- get new value
            x = string.format("%5d %s", line, x)
            send(x) -- send it to consumer
        end
    end)
end
   
function consumer (prod)
    while true do
        local x = receive(prod) -- get new value
        io.write(x, "\n") -- consume new value
    end
end
   
-- consumer(filter(producer()))