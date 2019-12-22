#
#Author: James Anthony Ortiz
#File: makefile
#Description: makefile for TextEditor project
#created in C99.
#Compile with 'make' or 'make kilo'
#




kilo: kilo.c
	$(CC) kilo.c -o kilo -Wall -Wextra -pedantic -std=c99
