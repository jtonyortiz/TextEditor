//=======================================================
//Author: James Anthony Ortiz
//File: kilo.c
//Description: A basic implementation of a text-processor. 
//compile: make command at terminal (see makefile)
//=======================================================

//struct termios, tcgetattr()/setattr(), ECHO, TCSAFLUSH
#include <termios.h> 
#include <unistd.h>
#include <stdlib.h>

struct termios orig_termios;


//===========================================
//Function: disableRawMode()
//Description: Save a copy of the termios struct 
//in its original state, and use tcsetattr() to 
//apply it to the terminal when the program
//exits.
//functions: tcsetattr()
//============================================
void disableRawMode()
{
  //tcsetattr()
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);

}//end deisableRawMode()


//===============================================
//Function: enableRawMode()
//c_lflag - local flags
//ECHO - Causes each key you type to be printed to the terminal
//termios struct - After modifying the values to the terminal using
//tcsetattr(). TCSAFLUSH argument specifies when to apply the change,
//in this case, it waits for all pending output to be written to the terminal,
//and discards any input that hasn't been read.
//ECHO - A bit-flag. We use the bitwise NOT operator, and a butwise AND
//operator which forces the fourth bit in the flags field to become 0,
//and causes every other but to reatin its value.
//===============================================
void enableRawMode()
{

  tcgetattr(STDIN_FILENO, &orig_termios);

  //atexit-comes from <stdlib.h>
  //Used to register disableRawMode() to be called
  //Automatically when the program exits, either from main(), or exit().

  atexit(disableRawMode);

  //Assign orig_termios to the raw struct to make a copy of it 
  //Before makeing further changes.

  struct termios raw = orig_termios;

  //tcgetattr(STDIN_FILENO, &raw);

  raw.c_lflag &= ~(ECHO);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

}//end enableRawMode()


//===================================
//MAIN
//===================================

int main()
{

  enableRawMode();

  char c;

  //STDIN_FILENO - comes from unistd.h
  //read() will read 1 byte from the standard input into the variable c
  //and will continue doing so until there is no more bytes to read.

  //While there is no 'q' character on the line of text, continue reading
  //in characters from standard input.

  while(read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
 
  return 0;
}//end main()
