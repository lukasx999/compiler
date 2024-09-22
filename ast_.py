#!/usr/bin/env python3

import os
import sys
import ast
import astpretty

# Python script for querying the python ast

def main() -> None:


    # see _ast.pyi for all python ast nodes



    while True:
        query: str = input(">>> ")
        os.system("clear")
        print(f">>> {query}")
        try:
            astpretty.pprint(ast.parse(query).body[0], show_offsets=False)
        except SyntaxError:
            print("syntax error!")
            pass




if __name__ == '__main__':
    sys.exit(main())
