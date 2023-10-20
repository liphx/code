" $HOME/.vim/plugin.vim
" $HOME/.vim/pack/bundle/start

" Nerdtree
"$ git clone https://github.com/preservim/nerdtree.git ~/.vim/pack/bundle/start/nerdtree
nnoremap <C-n> :NERDTree<CR>

" commentary
"$ git clone https://github.com/tpope/vim-commentary.git ~/.vim/pack/bundle/start/commentary
nnoremap <leader>\ :.Commentary<CR>
vnoremap <leader>\ :Commentary<CR>
autocmd FileType c setlocal commentstring=//\ %s
autocmd FileType cpp setlocal commentstring=//\ %s
autocmd FileType proto setlocal commentstring=//\ %s

" fzf
"$ sudo apt install fzf
"$ git clone https://github.com/junegunn/fzf.git ~/.vim/pack/bundle/start/fzf
nnoremap <C-p> :FZF<CR>

" clang-format
"$ git clone https://github.com/rhysd/vim-clang-format.git ~/.vim/pack/bundle/start/vim-clang-format
augroup cpp_filetype
    autocmd!
    autocmd FileType cpp set tags+=~/.vim/tags/stdcpp.tags
    autocmd FileType cpp map <F12> :ClangFormat<CR>
augroup END

augroup c_filetype
    autocmd!
    autocmd FileType c map <F12> :ClangFormat<CR>
augroup END

"$ git clone https://github.com/yianwillis/vimcdoc.git ~/.vim/pack/bundle/start/vimcdoc
"$ vim -u NONE -c "helptags ~/.vim/pack/bundle/start/nerdtree/doc" -c q
"$ git clone https://github.com/tpope/vim-dispatch.git ~/.vim/pack/bundle/start/dispatch
"$ git clone https://github.com/vim-airline/vim-airline.git ~/.vim/pack/bundle/start/airline
"$ git clone https://github.com/tpope/vim-surround.git ~/.vim/pack/bundle/start/surround

" :Git or :G
"$ git clone https://github.com/tpope/vim-fugitive.git ~/.vim/pack/bundle/start/fugitive

"$ git clone https://github.com/dense-analysis/ale.git ~/.vim/pack/bundle/start/ale
"$ git clone https://github.com/mhinz/vim-grepper.git ~/.vim/pack/bundle/start/grepper
"$ git clone https://github.com/vim-test/vim-test.git ~/.vim/pack/bundle/start/vim-test
"$ git clone https://github.com/tpope/vim-obsession.git ~/.vim/pack/bundle/start/obsession
"$ git clone https://github.com/junegunn/goyo.vim.git ~/.vim/pack/bundle/start/goyo
"$ git clone https://github.com/ycm-core/YouCompleteMe.git ~/.vim/pack/bundle/start/YouCompleteMe
"$ cd ~/.vim/pack/bundle/start/YouCompleteMe
"$ git submodule update --init --recursive
"$ python3 install.py --all
"$ python3 install.py --clangd-completer
"$ git clone git://github.com/godlygeek/tabular.git ~/.vim/pack/bundle/start/tabular
"$ git clone https://github.com/preservim/vim-markdown.git ~/.vim/pack/bundle/start/vim-markdown
" YouCompleteMe
" let g:ycm_key_invoke_completion = '<C-z>'
" set completeopt=menu,menuone
" let g:ycm_add_preview_to_completeopt = 0
"
"$ git clone https://github.com/vim-scripts/a.vim.git ~/.vim/pack/bundle/start/a.vim
"
" Go
"$ git clone https://github.com/fatih/vim-go.git ~/.vim/pack/plugins/start/vim-go
