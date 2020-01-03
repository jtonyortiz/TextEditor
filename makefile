#
#Author: James Anthony Ortiz
#File: makefile
#Description: makefile for TextEditor project
#created in C99.
#Compile with 'make'.
#




kilo: kilo.c
	$(CC) kilo.c -o kilo -Wall -Wextra -pedantic -std=c99

kilo2: kilo2.c
	$(CC) kilo2.c -o kilo2 -Wall -Wextra -pedantic -std=c99
