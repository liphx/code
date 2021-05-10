set nocompatible            " 不兼容vi

" 这是注释

set nobackup                " 不创建交换文件
set wildmenu
set wildmode=longest:list,full
                            " 命令模式下，底部操作指令按下 Tab 键自动补全

" 颜色相关
colorscheme default         " 颜色主题，在$VIMRUNTIME/colors/*.vim文件
syntax enable               " 语法高亮
                            " syntax on 同样语法高亮，但会覆盖已定义的颜色规则

" tab和空格相关
set tabstop=4               " 文本中tab以多少空格显示
set softtabstop=4           " 编辑一个tab时等同的空格数，和tabstop有关
                            " 例如tabstop=4，softtabstop=4则插入一个tab，为6则插入1个tab2个空格，为8则是两个tab
set expandtab               " tab替换成相应数量的空格
" 进行了上面的设置后一次退格最多可删除4个空格

" 缩进相关
set autoindent              " 下一行缩进保持和上一行相同
set smartindent
set shiftwidth=4            " 缩进空格数

" 界面相关
set number                  " 设置显示行号
set showmode                " 在底部显示当前模式
set showcmd                 " 在右下角显示未完成的命令，例如下移20行 20j
set cursorline              " 高亮显示当前行
set laststatus=2            " 是否显示状态栏。0 表示不显示，1 表示只在多窗口时显示，2 表示显示
set listchars=tab:→\ ,space:·
                            " 特殊显示字符

" 搜索相关
set showmatch               " 高亮显示匹配的括号{[()]}
set incsearch               " 每输入一个字符都进行搜索
set hlsearch                " 高亮搜索到的内容
set ignorecase              " 忽略大小写搜索

" 编码相关
set encoding=utf-8          " 使用utf-8

" 单词检查
" set spell spelllang=en_us   " 英文单词拼写检查，中文文档下建议关闭

