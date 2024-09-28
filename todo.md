# TODO
- [ ] generic dynamic array
- [ ] Better error handling with tokens
- [ ] Remove Tokens from ast nodes?
- [ ] tests
- [ ] add preprocessor for macros and removing comments
- [ ] string interpolation
- [ ] inline assembly
- [ ] add comments to lexer
- [ ] type checking pass
- [ ] xor operator: )(
- [ ] warning if not using `$` for globals
- [ ] implicit tokens?
- [ ] implicit void returntype (defun foo() {})
- [ ] seperate ast node for logical operators?
- [ ] dollar as part of identifier of global variables
- [ ] function: extra node for function parameters
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
 - goddamn js frameworks
- [ ] sophisticated Nvim plugin for syntax highlighting
- [x] make assignment an expression
- [x] fix this: if1 {} == if 1 {} or ret1 == ret 1
- [x] function declarations (NULL pointer)
- [x] refactor repr.c
- [x] inline tokenizer repr list
- [x] synchronize parser for better error handling (not the best, but it kinda works)
- [x] fix whitespace issues (eg: letfoo)
- [x] unite all print functions in one file
- [x] add static type checking (eg: let foo'int = 5;)
- [x] Symbol Table datastructure
 - stack of lists (temporary)
 - stack w/ pushcount (temporary)
 - **tree of lists** (every node has a pointer to the parent)

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
