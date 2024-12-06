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

	while (1) {
		c = readerGetChar(sourceBuffer);

		// Handle whitespace
		if (c == SPC_CHR || c == TAB_CHR || c == NWL_CHR) {
			if (c == NWL_CHR)
				line++;
			continue;
		}

		if (c == NWL_CHR) {
			line++;  // Increment line number
			currentToken.code = NWL_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		}

		// Handle single-line comments starting with //
		if (c == SLASH_CHR) {
			uni_char nextChar = readerGetChar(sourceBuffer);
			if (nextChar == SLASH_CHR) {
				currentToken.code = CMT_T;
				scData.scanHistogram[currentToken.code]++;
				// Consume characters until newline or EOF
				while ((c = readerGetChar(sourceBuffer)) != NWL_CHR && c != EOF_CHR && c != EOS_CHR);
				if (c == NWL_CHR)
					line++;
				continue;
			}
			else {
				readerRetract(sourceBuffer);
			}
		}

		// Handle single-character tokens
		switch (c) {
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
		case EQ_CHR:
			currentToken.code = EQ_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case PLUS_CHR:
			currentToken.code = ADD_T;
			currentToken.attribute.arithmeticOperator = OP_ADD;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case MINUS_CHR:
			currentToken.code = SUB_T;
			currentToken.attribute.arithmeticOperator = OP_SUB;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case MUL_CHR:
			currentToken.code = MUL_T;
			currentToken.attribute.arithmeticOperator = OP_MUL;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case DIV_CHR:
			currentToken.code = DIV_T;
			currentToken.attribute.arithmeticOperator = OP_DIV;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case DQUT_CHR:
			// Handle string literals
			lexStart = readerGetPosRead(sourceBuffer);
			while ((c = readerGetChar(sourceBuffer)) != DQUT_CHR && c != EOF_CHR && c != EOS_CHR) {
				if (c == NWL_CHR)
					line++;
			}
			if (c == DQUT_CHR) {
				lexEnd = readerGetPosRead(sourceBuffer);
				lexLength = lexEnd - lexStart;
				lexemeBuffer = readerCreate(lexLength + 2, 0, MODE_FIXED);

				if (!lexemeBuffer) {
					fprintf(stderr, "Scanner error: Cannot create lexeme buffer.\n");
					exit(1);
				}

				readerSetMark(sourceBuffer, lexStart - 1);
				readerRestore(sourceBuffer);
				for (i = 0; i < lexLength + 1; i++) {
					readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
				}
				readerAddChar(lexemeBuffer, READER_TERMINATOR);
				uni_string lexeme = readerGetContent(lexemeBuffer, 0);

				currentToken = funcSL(lexeme);
				scData.scanHistogram[currentToken.code]++;
				readerRestore(lexemeBuffer);
				return currentToken;
			}
			else {
				// Handle unterminated string error
				currentToken = funcErr("Unterminated string literal");
				return currentToken;
			}
			break;
		case EOF_CHR:
		case EOS_CHR:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = (c == EOF_CHR) ? SEOF_255 : SEOF_0;
			return currentToken;
		default:
			break;
		}

		// Handle identifiers (variables and functions)
		if (isalpha(c)) {
			// Start of an identifier
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);

			while (isalpha(c = readerGetChar(sourceBuffer))) {
				// Continue reading letters
			}

			readerRetract(sourceBuffer);
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate(lexLength + 1, 0, MODE_FIXED);

			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Cannot create lexeme buffer.\n");
				exit(1);
			}

			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++) {
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			}
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			uni_string lexeme = readerGetContent(lexemeBuffer, 0);

			// Now, look ahead to see if the next character is '('
			c = readerGetChar(sourceBuffer);
			if (c == LPR_CHR) {
				// Function identifier
				currentToken.code = MNID_T;
				readerRetract(sourceBuffer); // Leave '(' to be processed next time
			}
			else {
				// Variable identifier
				currentToken.code = VID_T;
				readerRetract(sourceBuffer); // Put back the character
			}

			// Check if the lexeme is a keyword
			for (i = 0; i < KWT_SIZE; i++) {
				if (strcmp(lexeme, keywordTable[i]) == 0) {
					currentToken.code = KW_T;
					currentToken.attribute.keywordIndex = i;
					break;
				}
			}

			if (currentToken.code == MNID_T || currentToken.code == VID_T) {
				// Copy the lexeme into the token attribute
				strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
				currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;  // Null-terminate
			}

			scData.scanHistogram[currentToken.code]++;
			readerRestore(lexemeBuffer);
			return currentToken;
		}

		// Handle numbers (integers and floating-point literals)
		if (isdigit(c)) {
			// Start of a number
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);

			uni_boln isFloat = UNI_FALSE;

			while (1) {
				c = readerGetChar(sourceBuffer);

				if (isdigit(c)) {
					// Continue reading digits
					continue;
				}
				else if (c == DOT_CHR) {
					if (isFloat) {
						// Error: multiple dots in number
						break;
					}
					isFloat = UNI_TRUE;
				}
				else {
					// Not a digit or dot, end of number
					break;
				}
			}

			readerRetract(sourceBuffer);
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate(lexLength + 1, 0, MODE_FIXED);

			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Cannot create lexeme buffer.\n");
				exit(1);
			}

			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++) {
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			}
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			uni_string lexeme = readerGetContent(lexemeBuffer, 0);

			if (isFloat) {
				currentToken = funcFL(lexeme);
			}
			else {
				currentToken = funcIL(lexeme);
			}
			scData.scanHistogram[currentToken.code]++;
			readerRestore(lexemeBuffer);
			return currentToken;
		}

		// Handle any other unrecognized character
		uni_char errLexeme[2] = { c, '\0' };
		currentToken = funcErr(errLexeme);
		return currentToken;
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
	if (isalpha(c))
		return 0; // Letter
	else if (isdigit(c))
		return 1; // Digit
	else {
		switch (c) {
		case DQUT_CHR:
			return 2; // Double quote '"'
		case SLASH_CHR:
			return 3; // Slash '/'
		case EOS_CHR:
		case EOF_CHR:
			return 4; // End of file
		case EQ_CHR:
			return 5; // Equal sign '='
		case DOT_CHR:
			return 6; // Dot '.'
		default:
			return 7; // Other characters
		}
	}
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
	uni_long tlong = atol(lexeme);
	if (tlong >= INT_MIN && tlong <= INT_MAX) {
		currentToken.code = INL_T;
		currentToken.attribute.intValue = (uni_int)tlong;
		scData.scanHistogram[currentToken.code]++;
	}
	else {
		// Handle integer out of range
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
		strcat(currentToken.attribute.errLexeme, "...");
		currentToken.code = ERR_T;
		scData.scanHistogram[ERR_T]++;
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
	// Since identifiers are fully handled in tokenizer, this function might be redundant
	Token currentToken = { 0 };
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

void printToken(Token t) {
	switch (t.code) {
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case INL_T:
		printf("INL_T\t\t%d\n", t.attribute.intValue);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t", t.attribute.contentString);
		if (stringLiteralTable != NULL) {
			printf("%s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
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
		if (t.attribute.keywordIndex >= 0 && t.attribute.keywordIndex < KWT_SIZE) {
			printf("KW_T\t\t%s\n", keywordTable[t.attribute.keywordIndex]);
		}
		else {
			printf("KW_T\t\t(Invalid Keyword Index)\n");
		}
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		if (errorNumber) {
			printf(" [Error Code: %d]", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\n", t.attribute.seofType);
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case VID_T:
		printf("VID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case EQ_T:
		printf("EQ_T\t\t=\n");
		break;
	case FPL_T:
		printf("FPL_T\t\t%f\n", t.attribute.floatValue);
		break;
	case ADD_T:
		printf("ADD_T\t\t+\n");
		break;
	case SUB_T:
		printf("SUB_T\t\t-\n");
		break;
	case MUL_T:
		printf("MUL_T\t\t*\n");
		break;
	case DIV_T:
		printf("DIV_T\t\t/\n");
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
Token funcAssignOp(uni_string lexeme) {
	Token currentToken = { 0 };
	currentToken.code = EQ_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


Token funcFL(uni_string lexeme) {
	Token currentToken = { 0 };
	uni_float tfloat = atof(lexeme);
	if (tfloat >= -DBL_MAX && tfloat <= DBL_MAX) {
		currentToken.code = FPL_T;
		currentToken.attribute.floatValue = tfloat;
		scData.scanHistogram[currentToken.code]++;
	}
	else {
		// Handle floating-point out of range
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
		strcat(currentToken.attribute.errLexeme, "...");
		currentToken.code = ERR_T;
		scData.scanHistogram[ERR_T]++;
	}
	return currentToken;
}

