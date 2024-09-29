import sys
import subprocess as s
from pprint import pprint


RECORD_NAME: str = "test.txt"


def get_compiler_output() -> list[str]:
    args: list[str] = ["./compiler", "file.script"]
    result = s.Popen(args, shell=True, stdout=s.PIPE)
    out: list[bytes] = result.stdout.readlines()
    return [line.decode("utf-8") for line in out]


def record() -> None:

    output: list[str] = get_compiler_output()

    global RECORD_NAME
    with open(RECORD_NAME, "w") as f:
        f.writelines(output)



def check() -> bool:

    global RECORD_NAME
    with open(RECORD_NAME, "r") as f:
        record: list[str] = f.readlines()

    output: list[str] = get_compiler_output()

    return record == output



def write_diff() -> None:

    output: list[str] = get_compiler_output()

    global RECORD_NAME
    args: list[str] = ["diff", RECORD_NAME, f"<(echo {output})"]
    result = s.Popen(args, shell=False, stdout=s.PIPE)

    string: list[str] = [line.decode("utf-8") for line in result.stdout.readlines()]

    with open("failure.diff", "w") as f:
        f.writelines(string)






def main() -> int:

    if check():
        print("tests passed!")
    else:
        write_diff()


    return 0




if __name__ == "__main__":
    sys.exit(main())
