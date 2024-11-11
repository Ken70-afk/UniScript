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
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@           %&@@@@@@@@@@@     @@    ”
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    ”
# ECHO "    @@      @& @   @ @       @     @@    ”
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    ”
# ECHO "    @@      & @ @  @@              @@    ”
# ECHO "    @@       @/ @*@ @ @   @        @@    ”
# ECHO "    @@           @@@@  @@ @ @      @@    ”
# ECHO "    @@            /@@    @@@ @     @@    ”
# ECHO "    @@     @      / /     @@ @     @@    ”
# ECHO "    @@     @ @@   /@/   @@@ @      @@    ”
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@         S O F I A           @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

uni_boln expectMethod = UNI_FALSE;
/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
uni_int line;								/* Current line number of the source code */
extern uni_int errorNumber;				/* Defined in platy_st.c - run-time error number */

extern uni_int stateType[NUM_STATES];
extern uni_string keywordTable[KWT_SIZE];

extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern uni_int transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */

uni_int startScanner(BufferPointer psc_buf) {
	/* TO_DO: Start histogram */
	for (uni_int i=0; i<NUM_TOKENS;i++)
		scData.scanHistogram[i] = 0;
	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(uni_null) {
	Token currentToken = { 0 };
	uni_char c;
	uni_int state = 0;
	uni_int lexStart;
	uni_int lexEnd;
	uni_int lexLength;
	uni_int i;

	uni_string lexeme = (uni_string)malloc(VID_LEN * sizeof(uni_char));
	if (!lexeme) {
		fprintf(stderr, "Memory allocation failed for lexeme.\n");
		return currentToken;
	}
	lexeme[0] = EOS_CHR;

	while (1) {
		c = readerGetChar(sourceBuffer);

		if (c < 0 || c >= NCHAR) {
			fprintf(stderr, "Invalid character encountered.\n");
			return currentToken;
		}

		switch (c) {
		case SPC_CHR:
		case TAB_CHR:
			break;
		case NWL_CHR:
			line++;
			break;
		case SLASH_CHR: {
			uni_char nextChar = readerGetChar(sourceBuffer);
			if (nextChar == SLASH_CHR) {
				return funcCMT("//");
			}
			else {
				readerRetract(sourceBuffer);
			}
			break;
		}
		case SCL_CHR:
			currentToken.code = EOS_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case LPR_CHR:
			currentToken.code = LPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case RPR_CHR:
			currentToken.code = RPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case LBR_CHR:
			currentToken.code = LBR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case RBR_CHR:
			currentToken.code = RBR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case EOS_CHR:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case EOF_CHR:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

		default:
			state = nextState(state, c);
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);

			while (stateType[state] == NOFS) {
				c = readerGetChar(sourceBuffer);
				state = nextState(state, c);
			}

			if (stateType[state] == FSWR)
				readerRetract(sourceBuffer);

			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate((uni_int)lexLength + 2, 0, MODE_FIXED);

			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Cannot create lexeme buffer.\n");
				exit(1);
			}

			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			lexeme = readerGetContent(lexemeBuffer, 0);

			if (!lexeme) {
				fprintf(stderr, "Error: Null lexeme encountered.\n");
				return currentToken;
			}

			uni_char lookahead = readerGetChar(sourceBuffer);
			if (lookahead == LPR_CHR) {
				expectMethod = UNI_TRUE;
			}
			else {
				expectMethod = UNI_FALSE;
				readerRetract(sourceBuffer);  // Retract if not a method
			}

			currentToken = (*finalStateTable[state])(lexeme);
			readerRestore(lexemeBuffer);
			return currentToken;
		}
	}
}

/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* TO_DO: Just change the datatypes */

uni_int nextState(uni_int state, uni_char c) {
	uni_int col;
	uni_int next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
/* TO_DO: Use your column configuration */

/* Adjust the logic to return next column in TT */
/*    [A-z],[0-9],    _,    ",   /,   SEOF, other
	   L(0), D(1), U(2), Q(3), S(4), E(5),  O(6) */

uni_int nextClass(uni_char c) {
	uni_int val = -1;
	switch (c) {
	case UND_CHR:
		val = 2; // Underscore
		break;
	case DQUT_CHR:
		val = 3; // Double quote for string literals
		break;
	case SLASH_CHR:
		val = 4; // Slash for starting comments
		break;
	case EOS_CHR:
	case EOF_CHR:
		val = 5; // End of source or file
		break;
	default:
		if (isalpha(c))
			val = 0; // Letters
		else if (isdigit(c))
			val = 1; // Digits
		else
			val = 6; // Other
	}
	return val;
}

/*
 ************************************************************
 * Acceptance State Function COM
 *		Function responsible to identify COM (comments).
 ************************************************************
 */
Token funcCMT(uni_string lexeme) {
	Token currentToken = { 0 };
	currentToken.code = CMT_T;  // Comment token code

	// Consume characters until newline or EOF to ignore the comment content
	uni_char c;
	while ((c = readerGetChar(sourceBuffer)) != NWL_CHR && c != EOS_CHR && c != EOF_CHR) {}

	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}



/*
************************************************************
* Acceptance State Function IL
*		Function responsible to identify IL (integer literals).
* - It respects the integer size limit (e.g., 2-byte integer in C).
* - If the integer literal is too large, it returns an error token.
* - Only the first ERR_LEN characters are accepted. If exceeded,
*   additional characters are replaced with "..." in the error lexeme.
***********************************************************
*/

Token funcIL(uni_string lexeme) {
	Token currentToken = { 0 };
	uni_long tlong;

	// Check if lexeme exceeds allowed integer literal length
	if (lexeme[0] != EOS_CHR && strlen(lexeme) > NUM_LEN) {
		// Exceeds length: handle as an error token
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
		strcat(currentToken.attribute.errLexeme, "...");
		currentToken.code = ERR_T;
		scData.scanHistogram[ERR_T]++;
	}
	else {
		// Convert lexeme to long integer
		tlong = atol(lexeme);
		// Validate integer range
		if (tlong >= 0 && tlong <= SHRT_MAX) {
			currentToken.code = INL_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.intValue = (uni_int)tlong;
		}
		else {
			// Out of range: handle as an error token
			strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
			currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
			strcat(currentToken.attribute.errLexeme, "...");
			currentToken.code = ERR_T;
			scData.scanHistogram[ERR_T]++;
		}
	}
	return currentToken;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		This function identifies identifiers (IDs) and keywords.
 * - Checks if the lexeme matches a keyword in UniScript.
 * - If not a keyword, it treats the lexeme as a general identifier.
 * - Enforces VID_LEN limit for identifier length and null-terminates.
 ***********************************************************
 */
Token funcID(uni_string lexeme) {
	Token currentToken = funcKEY(lexeme);  // Check if lexeme is a keyword

	if (currentToken.code == KW_T) return currentToken;  // Return if it's a keyword

	// Not a keyword, so treat it as an identifier
	currentToken.code = MNID_T;
	strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
	currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;  // Null-terminate

	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}



/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible for identifying string literals (SL).
 * - Stores string literals in the String Literal Table
 *   (stringLiteralTable), ensuring each string ends with '\0'.
 * - Increments the line count for any newline character within the string.
 * - Handles cases where the end quote is missing by returning an error token.
 ***********************************************************
 */
Token funcSL(uni_string lexeme) {
    Token currentToken = { 0 };
    uni_int i = 0, len = (uni_int)strlen(lexeme);
    currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);

    // Add each character to the string literal table except the starting and ending quotes
    for (i = 1; i < len - 1; i++) {
        if (lexeme[i] == NWL_CHR)
            line++;  // Increment line count if newline is encountered

        if (!readerAddChar(stringLiteralTable, lexeme[i])) {
            // If adding to string table fails, set runtime error
            currentToken.code = RTE_T;
            scData.scanHistogram[currentToken.code]++;
            strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
            errorNumber = RTE_CODE;
            return currentToken;
        }
    }

    // Null-terminate the string in the table
    if (!readerAddChar(stringLiteralTable, EOS_CHR)) {
        currentToken.code = RTE_T;
        scData.scanHistogram[currentToken.code]++;
        strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
        errorNumber = RTE_CODE;
        return currentToken;
    }

    currentToken.code = STR_T;
    scData.scanHistogram[currentToken.code]++;
    return currentToken;
}

/*
************************************************************
 * Acceptance State Function KEY
 *		Function responsible for identifying keywords (KW).
 * - Checks if the lexeme matches any entry in the keyword table.
 * - If a match is found, assigns KW_T as the token code.
 * - If no match is found, calls funcErr to handle it as an error.
 ***********************************************************
 */
Token funcKEY(uni_string lexeme) {
	Token currentToken = { 0 };
	uni_int kwindex = -1;

	// Check if the lexeme is a keyword
	for (uni_int j = 0; j < KWT_SIZE; j++) {
		if (strcmp(lexeme, keywordTable[j]) == 0) {
			kwindex = j;
			break;
		}
	}

	if (kwindex != -1) {
		currentToken.code = KW_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.keywordIndex = kwindex;
	}
	else {
		// Not a keyword, let funcID handle it as a regular identifier
		currentToken.code = MNID_T;
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;  // Null-terminate
		scData.scanHistogram[currentToken.code]++;
	}

	return currentToken;
}

/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible for handling ERR tokens.
 * - This function uses errLexeme, respecting the length limit ERR_LEN.
 * - If lexeme exceeds ERR_LEN, it truncates the lexeme and appends "...".
 * - The function updates the line count if newlines are present in the lexeme.
 ***********************************************************
 */
Token funcErr(uni_string lexeme) {
	Token currentToken = { 0 };
	uni_int i = 0, len = (uni_int)strlen(lexeme);

	// Truncate lexeme if it exceeds ERR_LEN, adding ellipsis if needed
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR; // Null-terminate after truncation
		strcat(currentToken.attribute.errLexeme, "..."); // Append ellipsis to indicate truncation
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme); // Copy lexeme directly if within limit
	}

	// Update line count based on newline characters in the lexeme
	for (i = 0; i < len; i++) {
		if (lexeme[i] == NWL_CHR)
			line++;
	}

	// Assign error token code and update histogram
	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

uni_null printToken(Token t) {
	extern uni_string keywordTable[]; /* Link to keyword table */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Handle run-time error if present */
		if (errorNumber) {
			printf(" [Error Code: %d]", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t", (uni_int)t.attribute.contentString);
		/* Ensure content exists before printing */
		if (stringLiteralTable != NULL) {
			printf("%s\n", readerGetContent(stringLiteralTable, (uni_int)t.attribute.contentString));
		}
		else {
			printf("(null)\n");
		}
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case KW_T:
		/* Ensure keyword index is within bounds */
		if (t.attribute.keywordIndex >= 0 && t.attribute.keywordIndex < KWT_SIZE) {
			printf("KW_T\t\t%s\n", keywordTable[t.attribute.keywordIndex]);
		}
		else {
			printf("KW_T\t\t(Invalid Keyword Index)\n");
		}
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
uni_null printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/
