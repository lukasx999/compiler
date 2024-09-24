# TODO
- [ ] tests
- [ ] add preprocessor for macros and removing comments
- [ ] string interpolation
- [ ] inline assembly
- [ ] add comments to lexer
- [ ] fix this: if1 {} == if 1 {} or ret1 == ret 1
- [ ] make assignment an expression
- [ ] type checking pass
- [ ] xor operator: )(
- [ ] implicit tokens?
- [ ] implicit void returntype (defun foo() {})
- [ ] dollar as part of identifier of global variables?
- [ ] function declarations (NULL pointer)
- [ ] add info calls to repr
- [ ] function calls
- [ ] productions with multiple tokens (operator precedence)
- [ ] references/pointers (let foo'&int;)
- [ ] lambda functions (like js)
- [ ] `nil` vs `void`
- [ ] else if
- [ ] prevent `ret` in global scope
- [ ] operator ")("
- [ ] evaluating expressions at compile time?
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

- Compilation stages

```
main (preprocessing)
(./pre)
  |
main (compilation)
(./compiler)
  |
out.s (assembly)
(nasm)
  |
out.o (linking)
(gcc)
  |
out (elf64 executable)
```
