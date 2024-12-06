/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F24)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "    @@                             @@    �
# ECHO "    @@           %&@@@@@@@@@@@     @@    �
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    �
# ECHO "    @@      @& @   @ @       @     @@    �
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    �
# ECHO "    @@      & @ @  @@              @@    �
# ECHO "    @@       @/ @*@ @ @   @        @@    �
# ECHO "    @@           @@@@  @@ @ @      @@    �
# ECHO "    @@            /@@    @@@ @     @@    �
# ECHO "    @@     @      / /     @@ @     @@    �
# ECHO "    @@     @ @@   /@/   @@@ @      @@    �
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@         S O F I A           @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/


/*
************************************************************
* File name: MainParser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013]
* Assignment: A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main code for Parser (A32)
* Function list: (...).
************************************************************
*/

/*
************************************************************
 * IMPORTANT NOTE:
 * The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define does not have any effect in other compilers  projects.
 *********************************************************
 */


#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h> /* Constants for calls to exit()*/

#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

 /* Check for ANSI C compliancy */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/*
 * -------------------------------------------------------------
 *  Global vars and External vars
 * -------------------------------------------------------------
 */

 /* Global objects - variables */
static BufferPointer sourceBuffer; /* pointer to input (source) buffer */
BufferPointer stringLiteralTable; /* This buffer is used as a repository for string literals */
uni_int errorNumber;     /* Run-time error number = 0 by default (ANSI) */

/* External objects */
extern uni_int syntaxErrorNumber /* number of syntax errors reported by the parser */;
extern uni_int line; /* source code line number - defined in scanner.c */

extern ParserData psData;

/*
 * -------------------------------------------------------------
 *  Function declarations
 * -------------------------------------------------------------
 */

 /* Function declarations (prototypes) */
extern uni_null startParser(uni_null);
extern uni_int startScanner(BufferPointer sc_buf);

static uni_null printParserError(uni_string fmt, ...);
static uni_null displayParser(BufferPointer ptrBuffer);
static uni_doub getParserFileSize(uni_string fname);
static uni_null callGarbageCollector(uni_null);

/*
************************************************************
 *  Parser Main function
 *  Parameters:
 *  - argc / argv = Parameters from command prompt
 *  Return value:
 *	- Success operation.
***********************************************************
*/

uni_int mainParser(uni_int argc, uni_string* argv) {

	numParserErrors = 0;			/* Initializes the errors */

	FILE* fi;       /* input file handle */
	uni_int loadsize = 0; /*the size of the file loaded in the buffer */

	/*check for correct arrguments - source file name */
	if (argc <= 1) {
		/* __DATE__, __TIME__, __LINE__, __FILE__ are predefined preprocessor macros*/
		printParserError("Date: %s  Time: %s", __DATE__, __TIME__);
		printParserError("Runtime error at line %d in file %s", __LINE__, __FILE__);
		printParserError("%s%s%s", argv[0], ": ", "Missing source file name.");
		printParserError("%s%s%s", "Usage: ", "parser", "  source_file_name");
		exit(EXIT_FAILURE);
	}

	/* create a source code input buffer - multiplicative mode */
	sourceBuffer = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_INCREMENT, MODE_MULTI);
	if (sourceBuffer == NULL) {
		printParserError("%s%s%s", argv[0], ": ", "Could not create source buffer");
		exit(EXIT_FAILURE);
	}

	/*open source file */
	if ((fi = fopen(argv[2], "r")) == NULL) {
		printParserError("%s%s%s%s", argv[0], ": ", "Cannot open file: ", argv[2]);
		exit(EXIT_FAILURE);
	}

	/* load source file into input buffer  */
	printf("Reading file %s ....Please wait\n", argv[2]);
	loadsize = readerLoad(sourceBuffer, fi);
	if (loadsize == UNI_ERROR)
		printParserError("%s%s%s", argv[0], ": ", "Error in loading buffer.");

	/* close source file */
	fclose(fi);
	/*find the size of the file  */
	if (loadsize == UNI_ERROR) {
		printf("The input file %s %s\n", argv[2], "is not completely loaded.");
		printf("Input file size: %ld\n", getParserFileSize(argv[2]));
	}
	/* Add SEOF (EOF) to input buffer and display the source buffer */
	if ((loadsize != UNI_ERROR) && (loadsize != 0)) {
		if (readerAddChar(sourceBuffer, READER_TERMINATOR)) {
			displayParser(sourceBuffer);
		}
	}
	/* create string Literal Table */
	stringLiteralTable = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_INCREMENT, MODE_ADDIT);
	if (stringLiteralTable == NULL) {
		printParserError("%s%s%s", argv[0], ": ", "Could not create string literal buffer");
		exit(EXIT_FAILURE);
	}

	/* Registrer exit function */
	atexit(callGarbageCollector);

	/* Initialize scanner  */
	startScanner(sourceBuffer);

	/* Start parsing */
	printf("\nParsing the source file...\n\n");
	startParser();

	printf("\nNumber of Parser errors: %d\n", numParserErrors);

	/* Prints the statistics */
	printBNFData(psData);

	return (EXIT_SUCCESS); /* same effect as exit(0) */

}

/*
************************************************************
 * Error printing function with variable number of arguments
 * Params: Variable arguments, using formats from C language.
 *	 - Internal vars use list of arguments and types from stdarg.h
 *   - NOTE: The format is using signature from C Language
************************************************************
*/

uni_null printParserError(uni_string fmt, ...) {

	va_list ap;
	va_start(ap, fmt);

	(uni_null)vfprintf(stderr, fmt, ap);
	va_end(ap);

	/* Move to new line */
	if (strchr(fmt, '\n') == NULL)
		fprintf(stderr, "\n");
}

/*
************************************************************
* The function return the size of an open file
* Param:
*	- Filename
* Return:
*	- Size of the file
************************************************************
*/

uni_doub getParserFileSize(uni_string fname) {
	FILE* input;
	uni_doub flength;
	input = fopen(fname, "r");
	if (input == NULL) {
		printParserError("%s%s", "Cannot open file: ", fname);
		return 0;
	}
	fseek(input, 0L, SEEK_END);
	flength = ftell(input);
	fclose(input);
	return flength;
}

/*
************************************************************
* The function display buffer contents
* Param:
*	- Parser to be displayed.
************************************************************
*/

uni_null displayParser(BufferPointer ptrBuffer) {
	printf("\nPrinting input buffer parameters:\n\n");
	printf("The capacity of the buffer is:  %d\n", readerGetSize(ptrBuffer));
	printf("The current size of the buffer is:  %d\n", readerGetPosWrte(ptrBuffer));
	printf("\nPrinting input buffer contents:\n\n");
	readerRecover(ptrBuffer);
	readerPrint(ptrBuffer);
}

/*
************************************************************
* The function frees all dynamically allocated memory.
* This function is always called despite how the program terminates - normally or abnormally.
************************************************************
*/

uni_null callGarbageCollector(uni_null) {
	if (syntaxErrorNumber)
		printf("\nSyntax errors: %d\n", syntaxErrorNumber);
	printf("\nCollecting garbage...\n");
	readerFree(sourceBuffer);
	readerFree(stringLiteralTable);
}
