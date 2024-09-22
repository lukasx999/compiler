# TODO
- [ ] tests
- [ ] add preprocessor for macros and removing comments
- [ ] add comments to lexer
- [ ] fix this: if1 {} == if 1 {} or ret1 == ret 1
- [ ] make assignment an expression
- [ ] `nil` vs `void`
- [ ] else if
- [ ] prevent `ret` in global scope
- [ ] operator ")("
- [ ] evaluating expressions at compile time?
- [ ] Automatic type deduction (foo := 3;)
- [ ] find a name for the language
- [ ] sophisticated Nvim plugin for syntax highlighting
- [x] refactor repr.c
- [x] inline tokenizer repr list
- [x] synchronize parser for better error handling (not the best, but it kinda works)
- [x] fix whitespace issues (eg: letfoo)
- [x] unite all print functions in one file
- [x] add static type checking (eg: let foo'int = 5;)

# NOTES
- which backend? (llvm-ir, qbe, nasm)
