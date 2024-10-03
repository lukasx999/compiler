" ftdetect/script.vim
" autocmd BufRead,BufNewFile *.script set filetype=script

" syntax/script.vim
syntax match Keyword /let/
syntax match Keyword /mut/
syntax match Keyword /defun/
syntax match Keyword /ret/
syntax match Keyword /if/
syntax match Keyword /else/
syntax match Keyword /elsif/
syntax match Keyword /loop/
syntax match Keyword /escape/
syntax match Keyword /container/
syntax match Keyword /create/
syntax match Keyword /demolish/
syntax match Keyword /echo/

syntax match Type /int/
syntax match Type /str/
syntax match Type /bool/
syntax match Type /float/
syntax match Type /void/

syntax match PunctMinor /;/
syntax match PunctMinor /+/
syntax match PunctMinor /-/
syntax match PunctMinor /\*/
syntax match PunctMinor /\//
syntax match PunctMinor /=/
syntax match PunctMinor /'/
syntax match PunctMinor /&/
syntax match PunctMinor /&&/
syntax match PunctMinor /||/
syntax match PunctMinor /!/
syntax match PunctMinor /==/
syntax match PunctMinor /!=/
syntax match PunctMinor /</
syntax match PunctMinor />/
syntax match PunctMinor />=/
syntax match PunctMinor /<=/

syntax match StringLiteral /".*"/

syntax match Literal /\d/
syntax match Literal /\d\.\d/
syntax match Literal /true/
syntax match Literal /false/
syntax match Literal /nil/

syntax match PunctParens /(/
syntax match PunctParens /)/
syntax match PunctParens /{/
syntax match PunctParens /}/
syntax match PunctParens /\[/
syntax match PunctParens /]/

syntax match Comments /#.*$/

syntax match Preprocessor /$define/
syntax match Preprocessor /$macro/
syntax match Preprocessor /$end/

highlight Keyword       ctermfg=blue  guifg=#bc72d4
highlight Type          ctermfg=blue  guifg=#7c91e6
highlight PunctMinor    ctermfg=grey  guifg=#8f96a1
highlight Literal       ctermfg=cyan  guifg=#75dfff
highlight StringLiteral ctermfg=green guifg=#7ce683
highlight PunctParens   ctermfg=red   guifg=#e05555
highlight Identifier    ctermfg=white guifg=#ede4e4
highlight Comments      ctermfg=grey  guifg=#64676b
highlight Preprocessor  ctermfg=grey  guifg=#803e46
