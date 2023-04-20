-- $HOME/.config/nvim/init.lua

-- UI
vim.opt.number = true
vim.opt.list = true
vim.opt.listchars = { tab = '→ ', trail = '·' }
vim.opt.cursorline = true
vim.opt.showmode = true
vim.opt.showcmd = true
vim.cmd("syntax enable")
vim.cmd("filetype plugin indent on")

-- tab page map
vim.keymap.set('n', '<leader><Tab>', 'gt<CR>')
vim.keymap.set('n', '<leader>1', '1gt<CR>')
vim.keymap.set('n', '<leader>2', '2gt<CR>')
vim.keymap.set('n', '<leader>3', '3gt<CR>')
vim.keymap.set('n', '<leader>4', '4gt<CR>')
vim.keymap.set('n', '<leader>5', '5gt<CR>')
vim.keymap.set('n', '<leader>6', '6gt<CR>')
vim.keymap.set('n', '<leader>7', '7gt<CR>')
vim.keymap.set('n', '<leader>8', '8gt<CR>')
vim.keymap.set('n', '<leader>9', '9gt<CR>')
vim.keymap.set('n', '<leader>t', ':tabnew<CR>')

-- paste
-- vim.opt.paste = true
vim.opt.clipboard:append { 'unnamedplus' }

-- tab and indent
vim.opt.tabstop = 4
vim.opt.softtabstop = 4
vim.opt.shiftwidth = 4
vim.opt.expandtab = true
vim.opt.autoindent = true
vim.opt.smartindent = true

-- search
vim.opt.showmatch = true
vim.opt.incsearch = true
vim.opt.hlsearch = true
vim.opt.ignorecase = true

-- file and encoding
vim.opt.fileformat = "unix"
vim.opt.encoding = "utf-8"
vim.opt.fileencoding = "utf-8"
vim.opt.fileencodings = "ucs-bom,utf-8,cp936,gb18030,big5,euc-jp,euc-kr,latin1"

-- tags
vim.opt.tags = "./tags;,tags"

-- save and quit
vim.keymap.set('n', '<leader>w', ':w!<CR>')
vim.keymap.set('n', '<leader>W', ':wall!<CR>')
vim.keymap.set('n', '<leader>q', ':q<CR>')
vim.keymap.set('n', '<leader>Q', ':qall<CR>')
