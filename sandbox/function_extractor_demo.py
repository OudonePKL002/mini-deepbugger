import subprocess
import re


BINARY_PATH = "sandbox/hello"


def disassemble_binary(binary_path):
    result = subprocess.run(
        ["otool", "-tvV", binary_path],
        capture_output=True,
        text=True,
        check=True,
    )

    return result.stdout


def extract_function(disassembly, function_name):

    lines = disassembly.splitlines()

    target = f"_{function_name}:"

    instructions = []

    inside_function = False

    for line in lines:

        stripped = line.strip()

        # Start of requested function
        if stripped == target:
            inside_function = True
            continue

        if inside_function:

            # Next symbol means current function ended
            if stripped.endswith(":") and not re.match(
                r"^[0-9a-fA-F]+:",
                stripped,
            ):
                break

            # Match an instruction line
            match = re.match(
                r"^[0-9a-fA-F]+\s+(.+)$",
                stripped,
            )

            if match:
                instruction = match.group(1)
                instructions.append(instruction)

    return instructions


disassembly = disassemble_binary(BINARY_PATH)


functions = [
    "add",
    "add_similar",
    "multiply",
]


print("=== Mini-DeepBugger Automatic Function Extraction ===")


for function_name in functions:

    instructions = extract_function(
        disassembly,
        function_name,
    )

    print(f"\nFunction: {function_name}")
    print("-" * 40)

    for instruction in instructions:
        print(instruction)

    print(
        "Instruction count:",
        len(instructions),
    )