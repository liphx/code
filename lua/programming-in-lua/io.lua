-- stdout
io.write("hello, world\n")
name = "Abc"
age = 20
io.write(string.format("name is %s, age is %d. \n", name, age))
io.write(233) -- 转换成字符串
io.write('\n')

-- stdin
num = io.read("n") -- 读一个数
char = io.read(1) -- 读一个字符
line = io.read("l") -- 读下一行，并丢弃换行符，io.read()默认参数
Line = io.read("L") -- 读下一行，并保留换行符
all = io.read("a") -- 读全文


local f = assert(io.open("filename", "r"))
local t = f:read("a")
f:close()


local temp = io.input() -- 保存当前流
print(temp)         -- file
print(type(temp))   -- userdata
io.input("newinput")    -- 打开新的流
io.input():close()      -- 关闭当前流
io.input(temp)          -- 恢复之前的流

--[[ functions

print()
io.read()
io.write()
io.lines()
io.open(filename, mode)
io.tmpfile()
io.flush()
file:setvbuf()
file:seek()
os.rename()
os.remove()
os.exit()
os.getenv()
os.execute()
io.popen()

]]--