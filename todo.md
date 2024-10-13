# TODO
- [ ] refactor parser macros into real functions
- [ ] :InspectTree

- [ ] shell cmd interpolation
- [ ] nvim bufferline
- [ ] convert grammar to real BNF
- [ ] Better error handling with tokens
- [ ] tests
- [ ] free astnodes
- [ ] preprocessor for macros and removing comments
- [ ] xor operator: )( | mod operator: %
- [ ] implicit tokens?
- [ ] implicit void returntype (defun foo() {})
- [ ] seperate ast node for logical operators?
- [ ] references/pointers (let foo'&int;)
- [ ] arrays (let foo'int\[5\];)
- [ ] object access (object:method(), object:member)
- [ ] else if
- [x] **should idtypepair really be an astnode?** (make it a helper function)
- [x] function calls
- [x] **add keyword tokens to every astnode**
- [x] make assignment an expression
- [x] fix this: if1 {} == if 1 {} or ret1 == ret 1
- [x] function declarations (NULL pointer)
- [x] refactor repr.c
- [x] inline tokenizer repr list
- [x] synchronize parser for better error handling (not the best, but it kinda works)
- [x] fix whitespace issues (eg: letfoo)
- [x] unite all print functions in one file
- [x] add static type checking (eg: let foo'int = 5;)
- [x] generic dynamic array
- [x] Symbol Table datastructure
- [x] move error handling to seperate module
- [x] add a license
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


- **Semantic analysis**
 - prevent usage of `ret` in global scope
 - enforce correct usage of `ret` in function
 - type checking
 - declaring existing variable more than once
 - double assignment to const
 - enforce `$` for global variables (or warning)


