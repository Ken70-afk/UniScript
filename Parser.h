/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# "=---------------------------------------="
# "|  COMPILERS - ALGONQUIN COLLEGE (F24)  |"
# "=---------------------------------------="
# "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# "    @@                             @@    ”
# "    @@           %&@@@@@@@@@@@     @@    ”
# "    @@       @%% (@@@@@@@@@  @     @@    ”
# "    @@      @& @   @ @       @     @@    ”
# "    @@     @ @ %  / /   @@@@@@     @@    ”
# "    @@      & @ @  @@              @@    ”
# "    @@       @/ @*@ @ @   @        @@    ”
# "    @@           @@@@  @@ @ @      @@    ”
# "    @@            /@@    @@@ @     @@    ”
# "    @@     @      / /     @@ @     @@    ”
# "    @@     @ @@   /@/   @@@ @      @@    ”
# "    @@     @@@@@@@@@@@@@@@         @@    ”
# "    @@                             @@    ”
# "    @@         S O F I A           @@    ”
# "    @@                             @@    ”
# "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# "                                         "
# "[READER SCRIPT .........................]"
# "                                         "
*/


/*
************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
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

/* Global vars */
static Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern uni_int		line;
extern Token			tokenizer(uni_null);
extern uni_string		keywordTable[KWT_SIZE];
static uni_int		syntaxErrorNumber = 0;

#define LANG_WRTE		"print&"
#define LANG_READ		"input&"
#define LANG_MAIN		"main&"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_data,
	KW_code,
	KW_int,
	KW_real,
	KW_string,
	KW_if,
	KW_then,
	KW_else,
	KW_while,
	KW_do
};

/* TO_DO: Define the number of BNF rules */
#define NUM_BNF_RULES 12

/* Parser */
typedef struct parserData {
	uni_int parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
uni_int numParserErrors;

/* Scanner data */
ParserData psData;

/* Function definitions */
uni_null startParser();
uni_null matchToken(uni_int, uni_int);
uni_null syncErrorHandler(uni_int);
uni_null printError();
uni_null printBNFData(ParserData psData);

/* List of BNF statements */
enum BNF_RULES {
	BNF_error,										/*  0: Error token */
	BNF_codeSession,								/*  1 */
	BNF_comment,									/*  2 */
	BNF_dataSession,								/*  3 */
	BNF_optVarListDeclarations,						/*  4 */
	BNF_optionalStatements,							/*  5 */
	BNF_outputStatement,							/*  6 */
	BNF_outputVariableList,							/*  7 */
	BNF_program,									/*  8 */
	BNF_statement,									/*  9 */
	BNF_statements,									/* 10 */
	BNF_statementsPrime								/* 11 */
};


/* TO_DO: Define the list of keywords */
static uni_string BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",
	"BNF_codeSession",
	"BNF_comment",
	"BNF_dataSession",
	"BNF_optVarListDeclarations",
	"BNF_optionalStatements",
	"BNF_outputStatement",
	"BNF_outputVariableList",
	"BNF_program",
	"BNF_statement",
	"BNF_statements",
	"BNF_statementsPrime"
};

/* TO_DO: Place ALL non-terminal function declarations */
uni_null codeSession();
uni_null comment();
uni_null dataSession();
uni_null optVarListDeclarations();
uni_null optionalStatements();
uni_null outputStatement();
uni_null outputVariableList();
uni_null program();
uni_null statement();
uni_null statements();
uni_null statementsPrime();

#endif
