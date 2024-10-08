import sys
import re
import textwrap
from pprint import pprint

def main() -> int:

    with open("parser.c", "r") as file:
        lines: list[str] = file.readlines()

    grammar: list[str] = [line for line in lines if bool(re.search("// BNF: ", line))]

    grammar = [textwrap.dedent(line) for line in grammar] # Remove leading spaces (for some reason?)
    grammar = [line[8:] for line in grammar] # Remove `// BNF: ` at the beginning
    grammar.reverse()


    offset: int = 0 # Position of Arrow "->"
    for line in grammar:
        if (new := line.index("-")) > offset:
            offset = new

    grammaraligned: list[str] = []
    for line in grammar:
        new: list[str] = [] # String
        header = line.split(" ")[0]
        new.append(header)
        new.append(" " * (offset - len(header))) # Spaces
        new.append(" ".join(line.split(" ")[1:])) # Append the rest of the line
        grammaraligned.append("".join(new))



    filename: str = __file__.split("/")[-1]

    with open("grammar.txt", "w") as file:
        file.write(f"== THIS FILE IS GENERATED BY `{filename}` ==\n\n")
        file.writelines(grammaraligned)

    return 0


if __name__ == '__main__':
    sys.exit(main())
