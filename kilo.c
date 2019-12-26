//=======================================================
//Author: James Anthony Ortiz
//File: kilo.c
//Description: A basic implementation of a text-processor. 
//compile: 'make' command at terminal (see makefile)
//=======================================================

#include <ctype.h>
#include <stdio.h> //lib for standard input/output, error handling

//struct termios, tcgetattr()/setattr(), ECHO, TCSAFLUSH
#include <termios.h> 
#include <unistd.h>
#include <stdlib.h> //Standard library
#include <errno.h> //For error handling

//Global Struct:
struct termios orig_termios;

//==============================================================
//Function: die()
//Description: prints an error message, exits program.
//Parameters:
//perror() - comes from <stdio.h>
//perror looks at the global errno variable and prints a personalized
//message for it. It also prints the string given to it before 
//it prints the error msg.
//exit() - comes from <stdlib.h>
//Exits program with exit status '1'.
//==============================================================

void die(const char *s)
{

  perror(s); //provides personalized error msg.
  exit(1); //Exit out of program.
  
}//end die() function


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
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");

}//end deisableRawMode()


//====================================================================
//Function: enableRawMode()
//Paramters:
//c_lflag - local flags
//ECHO - Causes each key you type to be printed to the terminal
//termios struct - After modifying the values to the terminal using
//tcsetattr(). 
//TCSAFLUSH - argument specifies when to apply the change,
//in this case, it waits for all pending output to be written to the terminal,
//and discards any input that hasn't been read.
//ECHO - A bit-flag. We use the bitwise NOT operator, and a butwise AND
//operator which forces the fourth bit in the flags field to become 0,
//and causes every other but to reatin its value.
//ISIG - comes from <termios.h>
//IEXTEN - idem. but belongs to the c_lflag field.
//ICRNL - 'I' stands for input flag, 'CR' stands for carriage return 
//'NL' stands for "new line".
//OPOST - 'O' Stands for output flag, 'Post' for post-processing
//of output.
//BRKINT, INCPCK, ISTRIP & CS8 all come from termios.h.
//BRKINT - when turned on, a break condition will cause a SIGINT
//signal to be sent to the program, similar to Ctrl-c.
//INPCK - enables parity checking
//ISTRIP - causes 8yth bit of each input byte to be stripped, 
//meaning that it will be set to 0.
//CS8 - Not an actual flag. A bit mask with miltiple bits which is set with
//bitwise-OR ('|'). It sets char bits to 8 bits per byte.
//VMIN - Sets the minimum number of bytes of input needed before read() 
//can return. Set to 0 to read() returns as soon as imput is read.
//VTIME - Sets the maximum amount of time to wait before read() returns.
//If read times out, it will return 0.
//======================================================================
void enableRawMode()
{
  if(tcgetattr(STDIN_FILENO, &orig_termios) == -1) 
    die("tcgetattr");

  //atexit-comes from <stdlib.h>
  //Used to register disableRawMode() to be called
  //Automatically when the program exits, either from main(), or exit().
  atexit(disableRawMode);

  //Assign orig_termios to the raw struct to make a copy of it
  //Before makeing further changes.
  struct termios raw = orig_termios;

  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK| ISTRIP |  IXON); //Added ICRNL and IXON 
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); //Added (ECHO | ICANON | ISIG)
 

  //VMIN & VITME - Indexes into the c_cc field "control characters"
  //cc array contains special terminal settings.

  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) 
    die("tcsetattr");

}//end enableRawMode()


//===================================
//MAIN
//Description: main driver program
//for text-processor.
//===================================

int main()
{

  enableRawMode();

  //STDIN_FILENO - comes from unistd.h
  //read() will read 1 byte from the standard input into the variable c
  //and will continue doing so until there is no more bytes to read.

  //While-loop: while there is no 'q' character on the line of text, 
  //continue reading in characters from standard input.
  
  while (1)
  {
    char c = '\0';

    //errno and EAGAIN come from <errno.h>
    //tcsetattr(), tcgetattr(), and read() return -1 on failure,
    //then set errno to indicate error.

    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) 
      die("read");

    if(iscntrl(c))
    {
      printf("%d\r\n", c);
    }
    else
    {
      printf("%d ('%c')\r\n", c, c);
    }
    if(c == 'q') break; //break from while-loop
  }
 
  return 0;
}//end main()
