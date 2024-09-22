# TODO
- [ ] tests
- [ ] synchronize parser for better error handling (slowly start collecting errors, sync parser up, then release all of them when parsing is done)
- [ ] add preprocessor for macros and removing comments
- [ ] add comments to lexer
- [ ] fix this: if1 {} == if 1 {} or ret1 == ret 1
- [ ] `nil` vs `void`
- [ ] else if
- [ ] prevent `ret` in global scope
- [ ] operator ")("
- [ ] evaluating expressions at compile time?
- [ ] inline tokenizer repr list
- [ ] Automatic type deduction (foo := 3;)
- [ ] find a name for the language
- [ ] refactor repr.c
- [ ] sophisticated Nvim plugin for syntax highlighting
- [x] fix whitespace issues (eg: letfoo)
- [x] unite all print functions in one file
- [x] add static type checking (eg: let foo'int = 5;)

# NOTES
- which backend? (llvm-ir, qbe, nasm)
