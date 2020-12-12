function traceback ()
    for level = 1, math.huge do
        local info = debug.getinfo(level, "Sl")
        if not info then break end
        if info.what == "C" then -- is a C function?
            print(string.format("%d\tC function", level))
        else -- a Lua function
            print(string.format("%d\t[%s]:%d", level,
            info.short_src, info.currentline))
        end
    end
end

traceback()

function foo (a, b)
    local x
    do local c = a - b end
    local a = 1
    while true do
    local name, value = debug.getlocal(1, a)
    if not name then break end
        print(name, value)
        a = a + 1
    end
end

foo(10, 20)