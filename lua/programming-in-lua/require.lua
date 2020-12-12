local m = require("math")
print(m.abs(-100)) -->100
print(m.huge) -->inf

local abs = require("math").abs
print(abs(-100)) -->100 可以单独导入函数

local inf = require("math").huge
print(inf) -->inf

local M = {} -- the module

-- creates a new complex number
local function new (r, i)
    return {r=r, i=i}
end

M.new = new -- add 'new' to the module

-- constant 'i'
M.i = new(0, 1)

function M.add (c1, c2)
    return new(c1.r + c2.r, c1.i + c2.i)
end

function M.tostring (c)
    return string.format("(%g,%g)", c.r, c.i)
end

return M