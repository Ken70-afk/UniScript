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
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/* Parser data */
extern ParserData psData; /* BNF statistics */

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TO_DO: This is the function to start the parser - check your program definition */

uni_null startParser() {
	/* TO_DO: Initialize Parser data */
	uni_int i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TO_DO: This is the main code for match - check your definition */
uni_null matchToken(uni_int tokenCode, uni_int tokenAttribute) {
	if (lookahead.code == tokenCode && (tokenAttribute == NO_ATTR || lookahead.attribute.codeType == tokenAttribute)) {
		lookahead = tokenizer();  // Update lookahead
		if (lookahead.code == ERR_T) {  // Handle errors
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else {
		printf("Debug: Token match failed (Expected Code=%d, Attribute=%d; Got Code=%d, Attribute=%d)\n",
			tokenCode, tokenAttribute, lookahead.code, lookahead.attribute.codeType);
		syncErrorHandler(tokenCode);  // Sync on error
	}
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TO_DO: This is the function to handler error - adjust basically datatypes */
uni_null syncErrorHandler(uni_int syncTokenCode) {
	printError();
	syntaxErrorNumber++;

	while (lookahead.code != syncTokenCode && lookahead.code != SEOF_T) {
		lookahead = tokenizer(); // Skip tokens until synchronization point
	}

	if (lookahead.code == syncTokenCode) {
		lookahead = tokenizer(); // Consume sync token
	}
}


/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TO_DO: This is the function to error printing - adjust basically datatypes */
uni_null printError() {
	extern numParserErrors;			/* link to number of errors (defined in Parser.h) */
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
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
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++; // Updated parser error
	}
}


  /*
   ************************************************************
   * Program statement
   * BNF: <program> → function functionName ( <opt_parameters> ) { <codeSession> }
   * FIRST(<program>) = {KW_T(function)}.
   ***********************************************************
   */
uni_null program() {
	psData.parsHistogram[BNF_program]++;

	// Process initial comments
	while (lookahead.code == CMT_T) {
		comment(); // Process comments at the beginning
	}

	// Ensure the function keyword is present
	if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_function) {
		matchToken(KW_T, KW_function); // Match 'function' keyword
		matchToken(MNID_T, NO_ATTR);   // Match the function name
		matchToken(LPR_T, NO_ATTR);    // Match '('
		opt_parameters();              // Parse optional parameters
		matchToken(RPR_T, NO_ATTR);    // Match ')'
		matchToken(LBR_T, NO_ATTR);    // Match '{'

		// Process comments within the function
		while (lookahead.code == CMT_T) {
			comment();
		}

		codeSession(); // Parse the function body

		// Match closing brace and handle trailing comments
		matchToken(RBR_T, NO_ATTR);
		while (lookahead.code == CMT_T) {
			comment();
		}
	}
	else if (lookahead.code != SEOF_T) {
		printError(); // Handle unexpected tokens
	}

	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

uni_null comment() {
	psData.parsHistogram[BNF_comment]++; // Increment the counter for comments
	matchToken(CMT_T, NO_ATTR);          // Match the single-line comment token

	// Consume tokens until the end of the line or file
	while (lookahead.code != SEOF_T && lookahead.code != NWL_T) {
		lookahead = tokenizer();
	}

	// Move past the newline if it exists
	if (lookahead.code == NWL_T) {
		lookahead = tokenizer();
	}

	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}

uni_null codeSession() {
	psData.parsHistogram[BNF_codeSession]++;

	// Process all statements and declarations within the function body
	optionalStatements();

	printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
}


uni_null optionalStatements() {
	psData.parsHistogram[BNF_optionalStatements]++;

	int safeguardCounter = 0; // Initialize the safeguard counter

	while (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
		// Increment the safeguard counter and check if it exceeds the limit
		if (++safeguardCounter > 100) {
			printf("Error: Infinite loop detected in optionalStatements\n");
			break; // Exit the loop if the counter exceeds the limit
		}

		switch (lookahead.code) {
		case KW_T:
			if (lookahead.attribute.codeType == KW_let) {
				declaration();  // Parse variable declarations
			}
			else {
				statement();  // Parse other statements
			}
			break;
		case VID_T:
			assignment();  // Handle variable assignments
			break;
		default:
			printError();  // Handle unexpected tokens
			lookahead = tokenizer();  // Consume invalid token
			break;
		}
	}

	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = {KW_T(print), ...}
 ***********************************************************
 */
uni_null statements() {
	psData.parsHistogram[BNF_statements]++;
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}


/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T, 
 *		KW_T(KW_if), KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
uni_null statementsPrime() {
	psData.parsHistogram[BNF_statementsPrime]++;

	if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_print) {
		statements();  // Continue parsing subsequent statements
	}
	// Otherwise, it's an empty production (ϵ).

	printf("%s%s\n", STR_LANGNAME, ": Statements Prime parsed");
}


/*
 ************************************************************
 * Single statement
 * BNF: <statement> -> <output statement> | ...
 * FIRST(<statement>) = {KW_T(print), ...}
 ***********************************************************
 */
uni_null statement() {
	psData.parsHistogram[BNF_statement]++;

	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == KW_print) {
			outputStatement();  // Parse print statement
			break;
		}
		else if (lookahead.attribute.codeType == KW_prompt) {
			promptStatement();  // Parse prompt statement
			break;
		}
		break;
	default:
		printError();
		lookahead = tokenizer();  // Consume invalid token
		break;
	}

	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}



/*
 ************************************************************
 * Output Statement
 * BNF: <output_statement> -> print (<output_variable_list>);
 * FIRST(<output_statement>) = {KW_T(print)}
 ***********************************************************
 */
 /*
  ************************************************************
  * Output Statement
  * BNF: <output_statement> -> print (<output_variable_list>);
  * FIRST(<output_statement>) = {KW_T(print)}
  ***********************************************************
  */
uni_null outputStatement() {
	psData.parsHistogram[BNF_outputStatement]++;
	matchToken(KW_T, KW_print);    // Match 'print'
	matchToken(LPR_T, NO_ATTR);   // Match '('
	outputVariableList();         // Parse variable list (STR_T or VID_T)
	matchToken(RPR_T, NO_ATTR);   // Match ')'
	matchToken(EOS_T, NO_ATTR);   // Match ';'
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}


/*
 ************************************************************
 * Output Variable List
 * BNF: <output_variable_list> -> STR_T | VID_T
 * FIRST(<output_variable_list>) = {STR_T, VID_T}
 ***********************************************************
 */
uni_null outputVariableList() {
	psData.parsHistogram[BNF_outputVariableList]++;

	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR); // Match string literal
		break;
	case VID_T:
		matchToken(VID_T, NO_ATTR); // Match variable identifier
		break;
	default:
		printError();
	}

	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}


/*
 ************************************************************
 * Optional Parameters
 * BNF: <opt_parameters> -> <parameters> | ϵ
 * FIRST(<opt_parameters>) = {VID_T, ϵ}.
 ***********************************************************
 */
uni_null opt_parameters() {
	psData.parsHistogram[BNF_opt_parameters]++;

	/* Check if the lookahead is a valid parameter token */
	switch (lookahead.code) {
	case VID_T:  // If a parameter exists, parse it
		parameters();
		break;
	default:
		; // Empty (ϵ)
	}

	printf("%s%s\n", STR_LANGNAME, ": Optional Parameters parsed");
}

/*
 ************************************************************
 * Parameters
 * BNF: <parameters> -> <parameter> | <parameters><parameter>
 * FIRST(<parameters>) = {VID_T}.
 ***********************************************************
 */
uni_null parameters() {
	psData.parsHistogram[BNF_parameters]++;

	/* Parse the first parameter */
	parameter();

	/* If more parameters exist, parse them recursively */
	while (lookahead.code == VID_T) {
		parameter();
	}

	printf("%s%s\n", STR_LANGNAME, ": Parameters parsed");
}

/*
 ************************************************************
 * Parameter
 * BNF: <parameter> -> VID_T
 * FIRST(<parameter>) = {VID_T}.
 ***********************************************************
 */
uni_null parameter() {
	psData.parsHistogram[BNF_parameter]++;

	/* Match the parameter identifier */
	matchToken(VID_T, NO_ATTR);

	printf("%s%s\n", STR_LANGNAME, ": Parameter parsed");
}



/*
 ************************************************************
 * The function prints statistics of BNF rules
 * Param:
 *	- Parser data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
/*
uni_null printBNFData(ParserData psData) {
}
*/
uni_null printBNFData(ParserData psData) {
	printf("Statistics:\n");
	printf("----------------------------------\n");
	for (int i = 0; i < NUM_BNF_RULES; i++) {
		if (psData.parsHistogram[i] > 0) {
			printf("Token[%s]=%d\n", BNFStrTable[i], psData.parsHistogram[i]);
		}
	}
	printf("----------------------------------\n");
}




uni_null expression() {
	psData.parsHistogram[BNF_expression]++;
	term();

	while (lookahead.code == ADD_T || lookahead.code == SUB_T) {
		printf("%s%s\n", STR_LANGNAME, ": Additive arithmetic expression parsed");
		matchToken(lookahead.code, NO_ATTR);  // Consume operator
		term();
	}

	printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
}

uni_null term() {
	psData.parsHistogram[BNF_term]++;
	variables();

	while (lookahead.code == MUL_T || lookahead.code == DIV_T) {
		printf("%s%s\n", STR_LANGNAME, ": Multiplicative arithmetic expression parsed");
		matchToken(lookahead.code, NO_ATTR);
		variables();
	}

	printf("%s%s\n", STR_LANGNAME, ": Term parsed");
}


uni_null variables() {
	psData.parsHistogram[BNF_factor]++;

	switch (lookahead.code) {
	case VID_T:
		printf("%s%s\n", STR_LANGNAME, ": Variable identifier parsed");
		matchToken(VID_T, NO_ATTR);
		break;
	case INL_T:
		printf("%s%s\n", STR_LANGNAME, ": Integer Variable identifier parsed");
		matchToken(lookahead.code, NO_ATTR);  // Match valid tokens
		break;
	case FPL_T:
		printf("%s%s\n", STR_LANGNAME, ": Float Variable identifier parsed");
		matchToken(lookahead.code, NO_ATTR);  // Match valid tokens
		break;
	case STR_T:
		printf("%s%s\n", STR_LANGNAME, ": String Variable identifier parsed");
		matchToken(lookahead.code, NO_ATTR);  // Match valid tokens
		break;
	case LPR_T:
		matchToken(LPR_T, NO_ATTR); // Match '('
		expression();               // Parse sub-expression
		matchToken(RPR_T, NO_ATTR); // Match ')'
		break;
	default:
		printError();
		lookahead = tokenizer();    // Consume invalid token
		break;
	}

	printf("%s%s\n", STR_LANGNAME, ": Variable declaration parsed");
}

uni_null declaration() {
	psData.parsHistogram[BNF_declaration]++;

	matchToken(KW_T, KW_let);      // Match 'let'
	matchToken(VID_T, NO_ATTR);    // Match variable name

	if (lookahead.code == EQ_T) {  // Optional initialization
		matchToken(EQ_T, NO_ATTR);
		expression();              // Parse initialization expression
	}

	if (lookahead.code != EOS_T) {
		printf("%s%s%d\n", STR_LANGNAME, ": Syntax error: Missing semicolon after declaration at line: ", line);
		syntaxErrorNumber++;
		// Attempt to recover by consuming tokens until EOS_T or RBR_T
		while (lookahead.code != EOS_T && lookahead.code != RBR_T && lookahead.code != SEOF_T) {
			lookahead = tokenizer();
		}
	}

	matchToken(EOS_T, NO_ATTR);    // Match semicolon
	printf("%s%s\n", STR_LANGNAME, ": Declaration parsed");
}



uni_null assignment() {
	psData.parsHistogram[BNF_assignment]++;

	matchToken(VID_T, NO_ATTR);  // Match variable name
	printf("%s%s\n", STR_LANGNAME, ": Assignment expression parsed");

	matchToken(EQ_T, NO_ATTR);   // Match assignment operator
	expression();                // Parse assigned expression
	matchToken(EOS_T, NO_ATTR);  // Match semicolon

	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}

uni_null promptStatement() {
	psData.parsHistogram[BNF_promptStatement]++;
	matchToken(KW_T, KW_prompt);                // Match 'prompt'
	printf("%s%s\n", STR_LANGNAME, ": Input statement parsed");

	matchToken(LPR_T, NO_ATTR);                 // Match '('
	matchToken(VID_T, NO_ATTR);                 // Match the variable identifier
	matchToken(RPR_T, NO_ATTR);                 // Match ')'
	matchToken(EOS_T, NO_ATTR);                 // Match semicolon
}

