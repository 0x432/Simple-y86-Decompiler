# Simple Y86 Disassembler

This project contains three C files (`task1.c`, `task2.c`, and `task3.c`) that collectively focus on reading, converting, and disassembling Y86 assembly language instructions. This is an assignment for the Computer Organisation and Architecture course at the University of Huddersfield.

## File Descriptions

### 1. `task1.c`
**Functionality:**
- This program is designed to read a list of Y86 instructions from a file and convert them into bytecode, subsequently printing the mnemonic of each instruction.

**Key Components:**
- **Header Inclusions:** Standard libraries such as `stdio.h`, `stdlib.h`, `string.h`, and `stdint.h`.
- **Instruction Mnemonics and Byte Codes:** Arrays `op` and `bits` store the mnemonics and corresponding bytecodes of Y86 instructions.
- **File Handling:** The program reads a file (`test1.txt` by default or specified via command-line argument) line by line.
- **Instruction Conversion:** A function `convertStrToByteCode` converts each line of the file into machine bytecode.
- **Instruction Matching:** The bytecode of each instruction is matched against the predefined `bits` array to print the corresponding mnemonic.

**Main Function Workflow:**
1. **File Opening:** Opens the specified file for reading.
2. **Reading and Conversion:** Reads each line, converts it to bytecode, and prints the corresponding mnemonic by matching the bytecode with predefined values.
3. **Closing File:** Closes the file after processing.

**Function `convertStrToByteCode`:**
- Converts a string of Y86 instruction into machine bytecode by parsing and storing each byte in an array.

### 2. `task2.c`
**Functionality:**
- This program disassembles Y86 binary instructions from a file and prints the human-readable assembly code.

**Key Components:**
- **Header Inclusions:** Standard libraries such as `stdio.h`, `stdlib.h`, `string.h`, and `stdint.h`.
- **Register Mnemonics:** Array `y86_register_mnemonics` stores the mnemonics for Y86 registers.
- **Utility Functions:** Functions `util_load_file` and `y86_disassemble`.

**Main Function Workflow:**
1. **File Loading:** Reads the binary data from the specified file into a buffer.
2. **Disassembly:** Uses the `y86_disassemble` function to parse the binary data and print the corresponding assembly instructions.

**Function `util_load_file`:**
- Loads the binary content of a file into a buffer and returns the number of bytes read.

**Function `y86_disassemble`:**
- Disassembles the binary data into Y86 assembly instructions.
- Iterates through the binary data, decodes each instruction, and prints the corresponding mnemonic and operands.

### 3. `task3.c`
**Functionality:**
- This program disassembles a Y86 binary file and prints each instruction in human-readable format.

**Key Components:**
- **Header Inclusions:** Standard libraries such as `stdio.h`, `stdlib.h`, `stdint.h`, and `inttypes.h`.
- **Instruction Mnemonics and Byte Sizes:** Arrays `y86_instructions` and `y86_instruction_size` store mnemonics and sizes for each Y86 instruction type.
- **Utility Functions:** Functions `util_load_file` and `y86_disassemble`.

**Main Function Workflow:**
1. **File Loading:** Reads the binary data from a specified file into a buffer.
2. **Disassembly:** The `y86_disassemble` function decodes the binary data and prints each instruction in a readable format.

**Function `util_load_file`:**
- Reads the entire content of a binary file into a buffer and returns the number of bytes read.

**Function `y86_disassemble`:**
- Disassembles the binary data and prints the Y86 assembly instructions.
- Utilizes predefined instruction sizes and mnemonics to decode each instruction and print its components.

## Summary
These C files are part of a Computer Organisation and Architecture assignment for the University of Huddersfield. They collectively focus on reading, converting, and disassembling Y86 assembly language instructions, translating binary data into human-readable assembly mnemonics. Each file handles different aspects of the disassembly process, including file reading, bytecode conversion, and detailed instruction decoding.
