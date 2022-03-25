// Importing the required include statments needed for the process.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// This is an array of register mnemonics in y86
const char* reg[] =
{
 "%eax",
 "%ecx",
 "%edx",
 "%ebx",
 "%esp",
 "%ebp",
 "%esi",
 "%edi",
 ""
};

// This is an array of oprands
const char* op[] =
{
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

// This is a list of bits
unsigned char bits[] = {0x00, 0x10, 0x90, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x60, 0x61, 0x62, 0x63, 0xA0, 0xB0, 0x30, 0x40, 0x50, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x80};

// Opens the .txt file and converst the strings to byte code so it can be worked on.
int convertStrToByteCode( const char *str, unsigned char inst[], int size );

int main ( int argc, char** argv )
{
  FILE *pFile = NULL;
  
  char buffer[15];

  if (argc < 2 )
  {
    pFile = fopen("./test3.txt", "r");
  }
  else
  {
    pFile = fopen(argv[1], "r");
  }
  
  if ( pFile == NULL )
  {
    printf("Error open test file, please make sure they exist.\n");
    return 0;
  }
  
  while( fgets(buffer, 15, pFile ) )
  {
      // This unsigned char array stores an instruction read from the file
      // As the largest y86 instruction is 6 bytes, there are 6 unsigned char in the array where
      // each represents a byte.
      unsigned char instruction[6] = {0,0,0,0,0,0};
      convertStrToByteCode(buffer, instruction, 6);
    
      // Gets 32 bit Int from bytes 1 to 4 using a bitshift
      uint32_t Bytes1to4 = instruction[1] + (instruction[2] << 8) + (instruction[3] << 16) + (instruction[4] << 24);

      // Gets 32 bit Int from bytes 2 to 5 using a bitshift
      uint32_t Bytes2to5 = instruction[2] + (instruction[3] << 8) + (instruction[4] << 16) + (instruction[5] << 24);

      // Splits the second byte into 2 ints in Base 16 (Hex)
      const unsigned int Base = 16;
      unsigned int divisor = Base;
      while ( instruction[1] / divisor > divisor ) divisor *= Base;
        int regA = instruction[1] / divisor;
        int regB = instruction[1] % divisor;
      
      // Loops throught the bits that are listed.
      // If matched to the given it will check what to do with the rest of the bits in the instruction
      // and outputs the assembly. 
      int i;
      for (i = 0; i < 27; ++i)
          if(bits[i] == instruction[0]){
            if(i <= 2)
              printf("%s\n",op[i]);
            else if(i >= 3 && i <= 13 )
                printf("%s %s, %s\n",op[i],reg[regA],reg[regB]);
            else if(i >=14 && i <=15)
                printf("%s %s\n",op[i],reg[regA]);
            else if(i == 16)
                printf("%s $%d, %s\n",op[i],Bytes2to5,reg[regB]);
            else if(i == 17)
                printf("%s %s, %d(%s)\n",op[i],reg[regA],Bytes2to5,reg[regB]);
            else if(i == 18)
                printf("%s %d(%s), %s\n",op[i],Bytes2to5,reg[regB],reg[regA]);
            else if(i >= 19 && i <= 27 )
                printf("%s %d\n",op[i],Bytes1to4);
          }    
  }
  fclose(pFile);
  return 0;
}

/****************************************************************************
This function converts a line of machine code read from the text file
into machine byte code.
The machine code is stored in an unsigned char array.
******************************************************************************/
int convertStrToByteCode( const char *str, unsigned char inst[], int size )
{
  int numHexDigits = 0;
  char *endstr;
  //Each instruction should consist of at most 12 hex digits
  numHexDigits = strlen(str) - 1;
  //Convert the string to integer, N.B. this integer is in decimal
  long long value = strtol(str, &endstr, 16);

  int numBytes = numHexDigits >> 1;
  int byteCount = numHexDigits >> 1;

  while ( byteCount > 0 )
  {
    unsigned long long mask = 0xFF;
    unsigned long shift = (numBytes - byteCount) << 3; 

    inst[byteCount - 1] = (value & (mask<<shift))>>shift;
    byteCount --;
  }

  //Return the size of the instruction in bytes
  return numHexDigits >> 1;
}