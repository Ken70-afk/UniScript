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
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
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
#define NUM_TOKENS 13

/* Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token */
	INL_T,		/*  2: Integer literal token */
	STR_T,		/*  3: String literal token */
	LPR_T,		/*  4: Left parenthesis token */
	RPR_T,		/*  5: Right parenthesis token */
	LBR_T,		/*  6: Left brace token */
	RBR_T,		/*  7: Right brace token */
	KW_T,		/*  8: Keyword token */
	EOS_T,		/*  9: End of statement (semicolon) */
	RTE_T,		/* 10: Run-time error token */
	SEOF_T,		/* 11: Source end-of-file token */
	CMT_T		/* 12: Comment token */
};

/* Define the list of token strings */
static uni_string tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"MNID_T",
	"INL_T",
	"STR_T",
	"LPR_T",
	"RPR_T",
	"LBR_T",
	"RBR_T",
	"KW_T",
	"EOS_T",
	"RTE_T",
	"SEOF_T",
	"CMT_T"
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
#define UND_CHR '_'        // Underscore character
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

/* Error states and illegal state */
#define ESNR    8          // Error state with no retract
#define ESWR    9          // Error state with retract
#define FS      10         // Illegal state

/* State transition table definition */
#define NUM_STATES        10
#define CHAR_CLASSES      7

/* Transition table - type of states defined in separate table */
static uni_int transitionTable[NUM_STATES][CHAR_CLASSES] = {
	/*    [A-z],[0-9],    _,   ",    /,   SEOF, other
		   L(0), D(1), U(2), Q(3), S(4), E(5),  O(6) */
		{     1, ESNR, ESNR,    4,    6, ESWR, ESNR}, // S0: NOAS - Initial state
		{     1,    1,    1,    2,    3,    3,    3}, // S1: NOAS - Handling letters/numbers for identifiers
		{    FS,   FS,   FS,   FS,   FS,   FS,   FS}, // S2: ASNR (ID) - Identifier accepted
		{    FS,   FS,   FS,   FS,   FS,   FS,   FS}, // S3: ASWR (KEY) - Keyword accepted
		{     4,    4,    4,    5,    4, ESWR,    4}, // S4: NOAS - String handling
		{    FS,   FS,   FS,   FS,   FS,   FS,   FS}, // S5: ASNR (SL) - String literal accepted
		{     6,    6,    6,    6,    8, ESWR,    6}, // S6: NOAS - Possible start of comment if followed by another /
		{     7,    7,    7,    7,    7, ESWR,    7}, // S7: NOAS - Inside comment (continues until newline)
		{    FS,   FS,   FS,   FS,   FS,   FS,   FS}, // S8: ASNR (COM) - Comment accepted when newline reached
		{    FS,   FS,   FS,   FS,   FS,   FS,   FS}  // S9: ASWR (ER) - Error state
};

/* Define accepting states types */
#define NOFS    0       /* not accepting state */
#define FSNR    1       /* accepting state with no retract */
#define FSWR    2       /* accepting state with retract */

/* List of acceptable states */
static uni_int stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSNR, /* 02 (ID) - Identifier */
	FSWR, /* 03 (KEY) - Keyword */
	NOFS, /* 04 - Inside string */
	FSNR, /* 05 (SL) - String literal */
	NOFS, /* 06 - Possible comment start */
	FSNR, /* 07 (COM) - Comment */
	FSNR, /* 08 - Error state, no retract */
	FSWR  /* 09 - Error state, with retract */
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

/*
 * Accepting function (action) callback table (array) definition
 */

 /* Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,		/* MNID	[02] */
	funcKEY,	/* KEY  [03] */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] */
	NULL,		/* -    [06] */
	funcCMT,	/* COM  [07] */
	funcErr,	/* ERR1 [06] */
	funcErr		/* ERR2 [07] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* Define the number of keywords from the language */
#define KWT_SIZE 13

/* Define the list of keywords */
static uni_string keywordTable[KWT_SIZE] = {
	"function",    /* KW00 */
	"print",       /* KW01 */
	"let",         /* KW02 */
	"const",       /* KW03 */
	"if",          /* KW04 */
	"else",        /* KW05 */
	"while",       /* KW06 */
	"for",         /* KW07 */
	"break",       /* KW08 */
	"continue",    /* KW09 */
	"return",      /* KW10 */
	"true",        /* KW11 */
	"false"        /* KW12 */
};

/* Scanner data */
ScannerData scData;

#endif

