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
static Token lookahead;
extern BufferPointer stringLiteralTable;
extern uni_int line;
extern Token tokenizer(uni_null);
extern uni_string keywordTable[KWT_SIZE];
static uni_int syntaxErrorNumber = 0;

#define LANG_WRTE       "print&"
#define LANG_READ       "input&"
#define LANG_MAIN       "main&"

/* Constants for keywords */
enum KEYWORDS {
    NO_ATTR = -1,
    KW_function,   /* KW_T (function) */
    KW_print,      /* KW_T (print) */
    KW_let,        /* KW_T (let) */
    KW_const,      /* KW_T (const) */
    KW_if,         /* KW_T (if) */
    KW_else,       /* KW_T (else) */
    KW_while,      /* KW_T (while) */
    KW_for,        /* KW_T (for) */
    KW_break,      /* KW_T (break) */
    KW_continue,   /* KW_T (continue) */
    KW_return,     /* KW_T (return) */
    KW_true,       /* KW_T (true) */
    KW_false,      /* KW_T (false) */
    KW_prompt      /* KW_T (prompt) */
};

/* Define the number of BNF rules */
#define NUM_BNF_RULES 19

/* Parser Data */
typedef struct parserData {
    uni_int parsHistogram[NUM_BNF_RULES];    /* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
uni_int numParserErrors;

/* Scanner Data */
ParserData psData;

/* Function declarations */
uni_null startParser();
uni_null matchToken(uni_int, uni_int);
uni_null syncErrorHandler(uni_int);
uni_null printError();
uni_null printBNFData(ParserData psData);

enum BNF_RULES {
    BNF_error,                  /*  0: Error token */
    BNF_codeSession,            /*  1 */
    BNF_comment,                /*  2 */
    BNF_opt_parameters,         /*  3: Optional Parameters */
    BNF_parameters,             /*  4: Parameters */
    BNF_parameter,              /*  5: Parameter */
    BNF_optionalStatements,     /*  6 */
    BNF_outputStatement,        /*  7 */
    BNF_outputVariableList,     /*  8 */
    BNF_program,                /*  9 */
    BNF_statement,              /* 10 */
    BNF_statements,             /* 11 */
    BNF_statementsPrime,        /* 12 */
    BNF_expression,             /* 13: Arithmetic expressions */
    BNF_term,                   /* 14: Terms in expressions */
    BNF_factor,                 /* 15: Factors in expressions */
    BNF_assignment,             /* 16: Variable assignments */
    BNF_declaration,            /* 17: Variable declarations */
    BNF_promptStatement
};

/* Define the list of BNF strings */
static uni_string BNFStrTable[NUM_BNF_RULES] = {
    "BNF_error",
    "BNF_codeSession",
    "BNF_comment",
    "BNF_opt_parameters",
    "BNF_parameters",
    "BNF_parameter",
    "BNF_optionalStatements",
    "BNF_outputStatement",
    "BNF_outputVariableList",
    "BNF_program",
    "BNF_statement",
    "BNF_statements",
    "BNF_statementsPrime",
    "BNF_expression",
    "BNF_term",
    "BNF_factor",
    "BNF_assignment",
    "BNF_declaration",
   "BNF_promptStatement"
};


/* Place ALL non-terminal function declarations */
uni_null codeSession();
uni_null comment();            /* Single-line comment parsing */
uni_null opt_parameters();
uni_null parameters();
uni_null parameter();
uni_null optionalStatements();
uni_null outputStatement();
uni_null outputVariableList();
uni_null program();
uni_null statement();
uni_null statements();
uni_null statementsPrime();
uni_null expression();         /* Handles arithmetic expressions */
uni_null term();               /* Handles terms (multiplication/division) */
uni_null variables();             /* Handles factors (variables, numbers, nested expressions) */
uni_null assignment();         /* Handles variable assignments */
uni_null declaration();        /* Handles variable declarations */
uni_null promptStatement();

#endif

