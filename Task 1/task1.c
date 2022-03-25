// Importing the required include statments needed for the process.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//This is an array of register mnemonics in y86
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
    pFile = fopen("./test1.txt", "r");
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
      //This unsigned char array stores an instruction read from the file
      //As the largest y86 instruction is 6 bytes, there are 6 unsigned char in the array where
      //each represents a byte.
      unsigned char instruction[6] = {0,0,0,0,0,0};
      convertStrToByteCode(buffer, instruction, 6);
        int i;
        for (i = 0; i < 27; ++i)
          {
            if(bits[i] == instruction[0]){
              printf("%s\n",op[i]);
            }
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