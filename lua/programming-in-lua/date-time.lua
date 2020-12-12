print(os.time())                         -->1568699153
print(os.time({year = 2019, month = 9, day = 17, hour = 13, min = 45, sec = 0}))        -->1568699100

t = os.date("*t", os.time())
for k, v in pairs(t) do
    print(k, v)
end

--[[
isdst   false
min     52
yday    260
sec     29
wday    3
year    2019
month   9
day     17
hour    13
]]--

t = os.time()
print(os.date("%Y-%m-%d", t))  -->2019-09-17

t = os.date("*t") -- get current time
t1 = os.time(t)
t.day = t.day +1 -- 2019-09-18
t2 = os.time(t)
print(os.difftime(t2, t1)/(60 * 60)) --24.0

print(t.sec) -- 43
t.sec = t.sec + 120
print(t.sec) -- 163

--cpu time
local x = os.clock()
local s = 0
for i = 1, 100000000 do s = s + i end
print(string.format("elapsed time: %.2f\n", os.clock() - x)) --local x = os.clock()
