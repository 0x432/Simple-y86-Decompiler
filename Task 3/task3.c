#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

size_t util_load_file(const char* filename, unsigned char data[]) {
	FILE* file = fopen(filename, "rb");
	if (!file) {
		printf("[!] Failed to open file: %s.\n", filename);
		return 0;
	}

	fseek(file, 0L, SEEK_END);
	const long size = ftell(file);
	rewind(file);

	size_t bytes = fread(data, 1, size, file);

	fclose(file);

	if (bytes != size) {
		printf("[!] Failed to read entire file.\n");
		return 0;
	}

	return bytes;
}

const char* y86_register_mnemonics[] = {
		"%eax",
		"%ecx",
		"%edx",
		"%ebx",
		"%esp",
		"%ebp",
		"%esi",
		"%edi"
};

const char* y86_instruction_mnemonics[] = {
		"halt",
		"nop",
		"ret",
		"rrmovl",
		"cmovle",
		"cmovl",
		"cmove",
		"cmovne",
		"cmovge",
		"cmovg",
		"addl",
		"subl",
		"andl",
		"xorl",
		"pushl",
		"popl",
		"irmovl",
		"rmmovl",
		"mrmovl",
		"jmp",
		"jle",
		"jl",
		"je",
		"jne",
		"jge",
		"jg",
		"call"
};

enum e_instruction_type {
	no_operand,
	register_only,
	intermediate_value,
	unknown
};

enum e_instruction_type y86_get_instruction_type(unsigned char byte) {
	switch (byte) {
		case 0x00:
		case 0x10:
		case 0x90:
			return no_operand;
		case 0x20:
		case 0x21:
		case 0x22:
		case 0x23:
		case 0x24:
		case 0x25:
		case 0x26:
		case 0x60:
		case 0x61:
		case 0x62:
		case 0x63:
		case 0xA0:
		case 0xB0:
			return register_only;
		case 0x30:
		case 0x40:
		case 0x50:
		case 0x70:
		case 0x71:
		case 0x72:
		case 0x73:
		case 0x74:
		case 0x75:
		case 0x76:
		case 0x80:
			return intermediate_value;
		default:
			return unknown;
	}
}
const char* y86_get_instruction_name(unsigned char byte) {
	switch (byte) {
		case 0x00:
			return "halt";
		case 0x10:
			return "nop";
		case 0x90:
			return "ret";
		case 0x20:
			return "rrmovl";
		case 0x21:
			return "cmovle";
		case 0x22:
			return "cmovl";
		case 0x23:
			return "cmove";
		case 0x24:
			return "cmovne";
		case 0x25:
			return "cmovge";
		case 0x26:
			return "cmovg";
		case 0x60:
			return "addl";
		case 0x61:
			return "subl";
		case 0x62:
			return "andl";
		case 0x63:
			return "xorl";
		case 0xA0:
			return "pushl";
		case 0xB0:
			return "popl";
		case 0x30:
			return "irmovl";
		case 0x40:
			return "rmmovl";
		case 0x50:
			return "mrmovl";
		case 0x70:
			return "jmp";
		case 0x71:
			return "jle";
		case 0x72:
			return "jl";
		case 0x73:
			return "je";
		case 0x74:
			return "jne";
		case 0x75:
			return "jge";
		case 0x76:
			return "jg";
		case 0x80:
			return "call";
		default:
			// TODO Throw
			return "unknown";
	}
}

void y86_disassemble(const unsigned char data[], size_t bytes) {
	unsigned char chunk[6];
	size_t size = 0;

	size_t current_byte = 0;

	for (size_t i = 0; i < bytes; i++) {
		unsigned char byte = data[i];

		// Start of instruction
		if (current_byte == 0) {
			switch (y86_get_instruction_type(byte)) {
				case no_operand:
					printf("%s\n", y86_get_instruction_name(byte));
					size = 1;
					break;
				case register_only:
					size = 2;
					break;
				case intermediate_value:
					switch (byte) {
						case 0x30:
						case 0x40:
						case 0x50:
							size = 6;
							break;
						default:
							size = 5;
					}
					break;
				case unknown:
					//printf("[!] Unknown instruction received: %X.\n", byte);
					size = 1;
					//return EINVAL;
			}
		}

		chunk[current_byte] = byte;

		if (current_byte == size - 1) {
			/*printf("Instruction: ");
			for (size_t j = 0; j < size; j++) {
				printf("%02X ", chunk[j]);
			}
			printf("\n");*/

			const char* name = y86_get_instruction_name(chunk[0]); //= y86_instruction_mnemonics[chunk[0]];

			unsigned int divisor = 16;
			while (byte / divisor > divisor)
				divisor *= 16;

			const char* rA = y86_register_mnemonics[chunk[1] / divisor];
			const char* rB = y86_register_mnemonics[chunk[1] % divisor];

			switch (size) {
				case 2: {
					switch (chunk[0]) {
						case 0xA0:
						case 0xB0:
							printf("%s %s\n", name, rA);
							break;
						default:
							printf("%s %s, %s\n", name, rA, rB);
							break;
					}
					break;
				}
				case 5: {
					uint32_t dest = chunk[1] + (chunk[2] << 8) + (chunk[3] << 16) + (chunk[4] << 24);
					printf("%s %" PRIu32 "\n", name, dest);
					break;
				}
				case 6: {
					uint32_t value = chunk[2] + (chunk[3] << 8) + (chunk[4] << 16) + (chunk[5] << 24);
					switch (chunk[0]) {
						case 0x30:
							printf("%s $%" PRIu32 ", %s\n", name, value, rB);
							break;
						case 0x40:
							printf("%s %s, %" PRIu32 "(%s)\n", name, rA, value, rB);
							break;
						case 0x50:
							printf("%s %" PRIu32 "(%s), %s\n", name, value, rB, rA);
							break;
					}
					break;
				}
				default:
					break;
			}

			current_byte = 0;
			size = 0;

			memset(chunk, 0, sizeof chunk);
		}
		else if (size != 1)
			current_byte++;
	}
}

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("[!] Invalid usage.\n");
		return -1;
	}

	// TODO: Check input data source size in util_load_file.
	unsigned char data[4096];
	const size_t bytes = util_load_file(argv[1], data);

	if (bytes == 0) {
		printf("[!] Bytes read is 0.\n");
		return -1;
	}

	// Output file data
	/*printf("[?] File data\n");
	for (size_t i = 0; i < bytes; i++) {
		printf("%02X ", data[i]);
	}
	printf("\n\n");*/

	//printf("[?] Disassembler view\n");
	y86_disassemble(data, bytes);
}