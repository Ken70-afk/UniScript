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
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/
#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* Define the number of tokens */
#define NUM_TOKENS 21

/* Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,      /*  0: Error token */
	MNID_T,     /*  1: Method name identifier token */
	INL_T,      /*  2: Integer literal token */
	STR_T,      /*  3: String literal token */
	LPR_T,      /*  4: Left parenthesis token */
	RPR_T,      /*  5: Right parenthesis token */
	LBR_T,      /*  6: Left brace token */
	RBR_T,      /*  7: Right brace token */
	KW_T,       /*  8: Keyword token */
	EOS_T,      /*  9: End of statement (semicolon) */
	RTE_T,      /* 10: Run-time error token */
	SEOF_T,     /* 11: Source end-of-file token */
	CMT_T,      /* 12: Comment token */
	VID_T,      /* 13: Variable identifier token */
	EQ_T,       /* 14: Equal sign (assignment operator) */
	FPL_T,      /* 15: Floating-point literal token */
	ADD_T,      /* 16: Addition operator */
	SUB_T,      /* 17: Subtraction operator */
	MUL_T,      /* 18: Multiplication operator */
	DIV_T,      /* 19: Division operator */
	NWL_T       /* 20: Newline token */

};


/* Define the list of token strings */
static uni_string tokenStrTable[NUM_TOKENS] = {
	"ERR_T",    /*  0 */
	"MNID_T",   /*  1 */
	"INL_T",    /*  2 */
	"STR_T",    /*  3 */
	"LPR_T",    /*  4 */
	"RPR_T",    /*  5 */
	"LBR_T",    /*  6 */
	"RBR_T",    /*  7 */
	"KW_T",     /*  8 */
	"EOS_T",    /*  9 */
	"RTE_T",    /* 10 */
	"SEOF_T",   /* 11 */
	"CMT_T",    /* 12 */
	"VID_T",    /* 13 */
	"EQ_T",     /* 14 */
	"FPL_T",    /* 15 */
	"ADD_T",    /* 16 */
	"SUB_T",    /* 17 */
	"MUL_T",    /* 18 */
	"DIV_T",     /* 19 */
	"NWL_T"
};


/* Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	uni_int codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	uni_int intValue;				/* integer literal attribute (value) */
	uni_int keywordIndex;			/* keyword index in the keyword table */
	uni_int contentString;			/* string literal offset from the beginning of the string literal buffer */
	uni_float floatValue;				/* floating-point literal attribute (value) */
	uni_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	uni_char errLexeme[ERR_LEN + 1];	/* error token attribute */
} TokenAttribute;

/* Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	uni_byte flags;			/* Flags information */
	union {
		uni_int intValue;				/* Integer value */
		uni_float floatValue;			/* Float value */
		uni_string stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	uni_int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	uni_int scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* Define lexeme FIXED classes for UniScript */
#define EOS_CHR '\0'       // End of string
#define EOF_CHR 0xFF       // End of file marker
#define SLASH_CHR '/'      // Slash character for comments
#define DQUT_CHR '"'       // Double quote for string literals
#define HST_CHR '#'        // Hash for inline comments
#define TAB_CHR '\t'       // Tab character
#define SPC_CHR ' '        // Space character
#define NWL_CHR '\n'       // Newline character
#define SCL_CHR ';'        // Semicolon for end of statement
#define LPR_CHR '('        // Left parenthesis for function calls
#define RPR_CHR ')'        // Right parenthesis for function calls
#define LBR_CHR '{'        // Left brace for block start
#define RBR_CHR '}'        // Right brace for block end
#define PLUS_CHR '+'       // Plus operator
#define MINUS_CHR '-'      // Minus operator
#define MUL_CHR '*'        // Multiplication operator
#define DIV_CHR '/'        // Division operator
#define EQ_CHR '='         // Equal sign (assignment operator)
#define DOT_CHR '.'		   // Dot character


/* Error states and illegal state */
#define ESNR    9          // Error state with no retract
#define ESWR    10         // Error state with retract
#define FS      11         // Illegal state

/* State transition table definition */
#define NUM_STATES        14
#define CHAR_CLASSES      8

/* Transition table */
static uni_int transitionTable[NUM_STATES][CHAR_CLASSES] = {
	/* State 0: Initial state */
	/* L(0)  D(1)   Q(2)   S(3)   E(4)   EQ(5)  DOT(6) O(7) */
	{     1,    8,    4,    6, ESWR,    9, ESNR, ESNR}, // S0: NOAS

	/* State 1: Identifier */
	{     1, ESNR,    2, ESNR, ESNR, ESNR, ESNR, ESNR}, // S1: NOAS

	/* State 2: Identifier accepted */
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS}, // S2: ASNR (ID)

	/* State 3: Error state */
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS}, // S3: ASWR (ER)

	/* State 4: String handling */
	{     4,    4,    5,    4, ESWR,    4,    4,    4}, // S4: NOAS

	/* State 5: String literal accepted */
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS}, // S5: ASNR (SL)

	/* State 6: Possible start of comment */
	{     6,    6,    6,    7, ESWR,    6,    6,    6}, // S6: NOAS

	/* State 7: Inside comment */
	{     7,    7,    7,    7, ESWR,    7,    7,    7}, // S7: NOAS

	/* State 8: Start of number (integer or float) */
	{  ESWR,    8, ESNR, ESNR, ESNR, ESNR,    9, ESNR}, // S8: NOAS

	/* State 9: After dot '.', expecting digits */
	{  ESNR,   10, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR}, // S9: NOAS

	/* State 10: Fractional part of floating-point number */
	{  ESWR,   10, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR}, // S10: NOAS

	/* State 11: Integer literal accepted */
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS}, // S11: ASWR (IL)

	/* State 12: Floating-point literal accepted */
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS}  // S12: ASWR (FL)
};

/* Define accepting states types */
#define NOFS    0       /* Not accepting state */
#define FSNR    1       /* Accepting state with no retract */
#define FSWR    2       /* Accepting state with retract */

/* List of acceptable states */
static uni_int stateType[NUM_STATES] = {
	NOFS, /*  0 */
	NOFS, /*  1 */
	FSNR, /*  2 (ID) */
	FSWR, /*  3 (ER) */
	NOFS, /*  4 */
	FSNR, /*  5 (SL) */
	NOFS, /*  6 */
	FSNR, /*  7 (COM) */
	NOFS, /*  8 */
	NOFS, /*  9 */
	FSWR, /* 10 (FL) */
	FSWR, /* 11 (IL) */
	FSWR  /* 12 (FL) */
};

/*
-------------------------------------------------
Function definitions
-------------------------------------------------
*/

/* Static (local) function prototypes */
uni_int			startScanner(BufferPointer psc_buf);
static uni_int	nextClass(uni_char c);					/* character class function */
static uni_int	nextState(uni_int, uni_char);		/* state machine function */
uni_null		printScannerData(ScannerData scData);
Token			tokenizer(uni_null);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(uni_string lexeme);

/* Declare accepting states functions */
Token funcSL(uni_string lexeme);
Token funcIL(uni_string lexeme);
Token funcID(uni_string lexeme);
Token funcCMT(uni_string lexeme);
Token funcKEY(uni_string lexeme);
Token funcErr(uni_string lexeme);
Token funcAssignOp(uni_string lexeme);
Token funcFL(uni_string lexeme);

/*
 * Accepting function (action) callback table (array) definition
 */

 /* Define final state table */
/* Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,      /* 0 */
	NULL,      /* 1 */
	funcID,    /* 2 */
	funcErr,   /* 3 */
	NULL,      /* 4 */
	funcSL,    /* 5 */
	NULL,      /* 6 */
	funcCMT,   /* 7 */
	NULL,      /* 8 */
	NULL,      /* 9 */
	funcFL,    /* 10 */
	funcIL,    /* 11 */
	funcFL     /* 12 */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* Define the number of keywords from the language */
#define KWT_SIZE 14

/* Define the list of keywords */
static uni_string keywordTable[KWT_SIZE] = {
	"function",
	"print",
	"let",
	"const",
	"if",
	"else",
	"while",
	"for",
	"break",
	"continue",
	"return",
	"true",
	"false",
	"prompt"
};


/* Scanner data */
ScannerData scData;

#endif

