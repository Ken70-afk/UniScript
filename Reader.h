/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: Diyon Johnson / Abhinav Ranjith
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=------------------------------------------------="
# ECHO "|        COMPILERS - ALGONQUIN COLLEGE (F24)     |"
# ECHO "=------------------------------------------------="
# ECHO "                                        	      "
# ECHO " +++++++=++*+++++++++++++++=*++++++++++++++++++++ "
# ECHO " ++++++++++++++++++++++++++++++++++++++=+++++++++ "
# ECHO " ++++++++++++++++++++++++***+*+*+=**+++++++++++++ "
# ECHO " +++++*+++++.        -++++=+=.        -+++*+++++= "
# ECHO " ++++++++++  @@@@@@@  ++++++  @@@@@@@# -++++**+++ "
# ECHO " +++++++++* @@     @@ ++++++ @@     @@ .=**====+* "
# ECHO " +++++++++* -@ = @ @@ ++++++ =@  @@  @  **=++**+= "
# ECHO " *++++++++* -@ @@@ @@ ++++++ :@ @@@ %@ .+==++++++ "
# ECHO " +++++++++* -@     @@ ++=+== :@     @@  +++++++*+ "
# ECHO " *+++++++++ -@ @@% @@ +*+**+ :@ @@  %@  +++++++*+ "
# ECHO " +++++++*=+ -@     @@ ===+=+ :@  .@@#@  +++***+++ "
# ECHO " ++++++++++ :@ ##% @@ *++++* .@ @@  @@  +++++++++ "
# ECHO " +===++*+++ -@     @@  .++-  @@     @@ .++++**+*+ "
# ECHO " +*++*++=++ =@  @@  @@@     @@  @@@ @@ :+++++++=+ "
# ECHO " ++*+=+**++  @@ @@    @@@@@@    @@  @@ +++++****+ "
# ECHO " ++++*===+==  @@   %@@   @   @@    @@  .*++++==++ "
# ECHO " ++++*+***++-  @@@      @@       @@  @@  ++++*+++ "
# ECHO " ++++++*=+++==   @@@@   @@   @@@@*  @@ @ =+++++++ "
# ECHO " *===*+*+=+++==:    %@@@@@@@@@:     @@@  ++++++++ "
# ECHO " **++++==**+++++=:              -+*-    =++++++*+ "
# ECHO " +++++++++++*+=++++++==---==+++*++++++++++++++*+= "
# ECHO " **=++++*++*+*+++*+++++++++++++++++++++++++++=+*+ "
# ECHO " *+++*+++++++*+  U N I S C R I P T ++++++++++++++ "
# ECHO " *+*+++++==++++*++=++++++++++++++++++++++++++=+++ "
# ECHO " *+*+++++**++*+****++++++++++++++++++++++++++*+++ "
# ECHO "                                                  "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm',  /* Multiplicative mode (constant increment to be multiplied) */
	MODE_TOTAL = 't'
};

/* Constants about controls (not need to change) */
#define READER_TERMINATOR	'\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	100		/* default increment factor */

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
//typedef struct flag {
//	uni_boln FLAG_EMP;			/* indicates if the buffer is empty */
//	uni_boln FLAG_FUL;			/* indicates if the buffer is full */
//	uni_boln FLAG_END;			/* indicates if the buffer was completely read */
//	uni_boln FLAG_REL;			/* indicates if the buffer memory was changed */
//} Flag;

#define FLAG_EMP 0x01    /* EMP flag bit 00000001 */
#define FLAG_FUL 0x02    /* FUL flag bit 00000010 */
#define FLAG_END 0x04    /* END flag bit 00000100 */
#define FLAG_REL 0x08    /* REL flag bit 00001000*/

typedef uni_char sofia_boln[4];  /* Define a 4-element boolean array for the flags */



/* Offset declaration */
typedef struct position {
	uni_int wrte;			/* the offset to the add chars (in chars) */
	uni_int read;			/* the offset to the get a char position (in chars) */
	uni_int mark;			/* the offset to the mark position (in chars) */
} Position;




/* Buffer structure */
typedef struct bufferReader {
	uni_string	content;			/* pointer to the beginning of character array (character buffer) */
	uni_int		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	uni_int		increment;			/* character array increment factor */
	uni_char		mode;				/* operational mode indicator */
	uni_boln			flags[4];				/* contains character array reallocation flag and end-of-buffer flag */
	Position		positions;			/* Offset / position field */
	uni_int		histogram[NCHAR];	/* Statistics of chars */
	uni_int		numReaderErrors;	/* Number of errors from Reader */
	uni_byte		checksum;
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	readerCreate		(uni_int, uni_int, uni_char);
BufferPointer	readerAddChar		(BufferPointer const, uni_char);
uni_boln		readerClear		    (BufferPointer const);
uni_boln		readerFree		    (BufferPointer const);
uni_boln		readerIsFull		(BufferPointer const);
uni_boln		readerIsEmpty		(BufferPointer const);
uni_boln		readerSetMark		(BufferPointer const, uni_int);
uni_int		readerPrint		    (BufferPointer const);
uni_int		readerLoad			(BufferPointer const, FILE* const);
uni_boln		readerRecover		(BufferPointer const);
uni_boln		readerRetract		(BufferPointer const);
uni_boln		readerRestore		(BufferPointer const);
uni_null		readerCalcChecksum	(BufferPointer const);
uni_boln		readerPrintFlags	(BufferPointer const);
uni_null		readerPrintStat     (BufferPointer const);
/* Getters */
uni_char		readerGetChar		(BufferPointer const);
uni_string	readerGetContent	(BufferPointer const, uni_int);
uni_int		readerGetPosRead	(BufferPointer const);
uni_int		readerGetPosWrte	(BufferPointer const);
uni_int		readerGetPosMark	(BufferPointer const);
uni_int		readerGetSize		(BufferPointer const);
uni_int		readerGetInc		(BufferPointer const);
uni_char		readerGetMode		(BufferPointer const);
uni_int		readerGetNumErrors	(BufferPointer const);

#endif
