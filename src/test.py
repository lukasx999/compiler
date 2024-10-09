import sys
import subprocess as s
from pprint import pprint
import difflib


RECORD_NAME: str = "test_record.txt"
INPUT_FILE:  str = "test_all_the_features"
BIN:         str = "compiler"
DIFF_FILE:   str = "test_failure.diff"


COLOR_RED         : str = "\33[31m"
COLOR_BLUE        : str = "\33[34m"
COLOR_PURPLE      : str = "\33[35m"
COLOR_GRAY        : str = "\33[90m"
COLOR_CYAN        : str = "\33[36m"
COLOR_YELLOW      : str = "\33[33m"
COLOR_WHITE       : str = "\33[97m"
COLOR_GREEN       : str = "\33[32m"
COLOR_BOLD        : str = "\33[1m"
COLOR_UNDERLINE   : str = "\33[4m"
COLOR_END         : str = "\33[0m"



def get_compiler_output() -> list[str]:
    global INPUT_FILE
    global BIN
    args: str = f"./{BIN} {INPUT_FILE}"
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
    with open(RECORD_NAME, "r") as f:
        record: list[str] = f.readlines()


    diff = difflib.ndiff(record, output)
    diff = list(diff)

    with open(DIFF_FILE, "w") as f:
        f.writelines(diff)






def main() -> int:

    if len(sys.argv) == 2 and sys.argv[1] == "record":
        record()
        return 0


    if check():
        print(COLOR_GREEN, COLOR_BOLD, "tests passed!", COLOR_END, sep="")
    else:
        print(COLOR_RED, COLOR_BOLD, f"tests failed! - diff saved in `{DIFF_FILE}`", COLOR_END, sep="")
        write_diff()


    return 0




if __name__ == "__main__":
    sys.exit(main())
