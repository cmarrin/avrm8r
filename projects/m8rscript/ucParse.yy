/*
 _______________________________________________________________________
 |
 |
 |   Description:
 |	This file contains the bison rules for the ucscript Parser
 |
 |   Author(s)		: Chris Marrin
 |
 _______________________________________________________________________
 */

%{
#include "config.h"
#include "ucParser.h"
#include "ucParse.hpp"

#ifndef alloca
#define alloca malloc
#endif

#ifdef BISON_DEBUG
#define YYDEBUG 1
#endif

#define YYLEX_PARAM (((UC::Parser*) parser)->getScanner())
#define YYPARSE_PARAM parser

inline void yyerror(const char* s) { /*(UC::ParseHandler* ph)->error(s);*/ }

int yylex (YYSTYPE * yylval_param , void* yyscanner)
{
	return ((UC::Scanner*) yyscanner)->getToken(yylval_param);
}

%}

%union {
	uint16_t	atom;
	int32_t		integer;
	float		number;
};

/* These values must match the strings in Scanner.cpp */

/* keywords go from 1-32, where the control codes and space go */

%token K_FUNCTION		1
%token K_NEW			2
%token K_DELETE			3
%token K_TYPEOF			4
%token K_IN				5
%token K_VAR			6
%token K_TRUE			7
%token K_FALSE			8
%token K_NULL			9
%token K_DO				10
%token K_WHILE			11
%token K_FOR			12
%token K_IF				13
%token K_ELSE			14
%token K_SWITCH			15
%token K_CASE			16
%token K_DEFAULT		17
%token K_BREAK			18
%token K_CONTINUE		19
%token K_RETURN			20
%token K_THIS			21

/* 33-47 are special chars and can't be used */

/* 48-57 are the digits and can be used */
%token T_FLOAT			48
%token T_IDENTIFIER		49
%token T_STRING			50
%token T_INTEGER		51

/* 58-64 are special chars and can't be used */

/* 65-90 are letters and can be used */

%token O_RSHIFTEQ		65
%token O_RSHIFTFILLEQ	66
%token O_LSHIFTEQ		67
%token O_ADDEQ			68
%token O_SUBEQ			69
%token O_MULEQ			70
%token O_DIVEQ			71
%token O_MODEQ			72
%token O_ANDEQ			73
%token O_XOREQ			74
%token O_OREQ			75
%token O_RSHIFT			76
%token O_RSHIFTFILL		77
%token O_LSHIFT			78
%token O_INC			79
%token O_DEC			80
%token O_LAND			81
%token O_LOR			82
%token O_LE				83
%token O_GE				84
%token O_EQ				85
%token O_NE				86

%token O_BLOCK_START	87
%token O_OBJ_START		88

/*  we expect if..then..else to produce a shift/reduce conflict */
%expect 1
%pure_parser

%start program
%%

program:	source_elements
    ;

source_elements:	source_element
    |			source_elements source_element
    ;

source_element:
		statement
    |	function_declaration
    ;
	
primary_expression
	: T_IDENTIFIER
	| K_THIS
    | K_NULL
    | K_TRUE
    | K_FALSE
    | T_FLOAT
	| T_INTEGER
    | T_STRING
    |	objectLiteral
	| '(' expression ')'
	;
	
postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression objectLiteral
	| postfix_expression T_STRING
	| postfix_expression '.' T_IDENTIFIER
	| postfix_expression O_INC
	| postfix_expression O_DEC
    ;
	;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

unary_expression
	: postfix_expression
	| unary_operator unary_expression
	;

unary_operator
	: '+'
	| '-'
	| '~'
	| '!'
	| K_DELETE
	| K_TYPEOF
	| O_INC
	| O_DEC
	| K_NEW
	;

multiplicative_expression
	: unary_expression
	| multiplicative_expression '*' unary_expression
	| multiplicative_expression '/' unary_expression
	| multiplicative_expression '%' unary_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

shift_expression
	: additive_expression
	| shift_expression O_LSHIFT additive_expression
	| shift_expression O_RSHIFT additive_expression
	| shift_expression O_RSHIFTFILL additive_expression
	;

relational_expression
	: shift_expression
	| relational_expression K_IN shift_expression
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression O_LE shift_expression
	| relational_expression O_GE shift_expression
	;

equality_expression
	: relational_expression
	| equality_expression O_EQ relational_expression
	| equality_expression O_NE relational_expression
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression O_LAND inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression O_LOR logical_and_expression
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression
	;

assignment_operator
	: '='
	| O_MULEQ
	| O_DIVEQ
	| O_MODEQ
	| O_ADDEQ
	| O_SUBEQ
	| O_LSHIFTEQ
	| O_RSHIFTEQ
	| O_RSHIFTFILLEQ
	| O_ANDEQ
	| O_XOREQ
	| O_OREQ
	;

expression
	: assignment_expression
	| expression ',' assignment_expression
	;

declaration_statement
	: K_VAR variable_declaration_list ';'
	;
	
variable_declaration_list:
		variable_declaration
    |	variable_declaration_list ',' variable_declaration
    ;

variable_declaration:
    	T_IDENTIFIER
    |	T_IDENTIFIER initializer
    ;

initializer:
    	'=' assignment_expression
    ;

statement
	: compound_statement
	| declaration_statement
	| expression_statement
	| selection_statement
	| switch_statement
	| iteration_statement
	| jump_statement
	;

compound_statement
	: O_BLOCK_START '}'
	| O_BLOCK_START statement_list '}'
	;

statement_list
	: statement
	| statement_list statement
	;

expression_statement
	: ';'
	| expression ';'
	;

selection_statement
	: K_IF '(' expression ')' statement
	| K_IF '(' expression ')' statement K_ELSE statement
	;
	
switch_statement:
		K_SWITCH '(' expression ')' case_block
	;

case_block:
		O_BLOCK_START case_clauses_opt '}'
	|	O_BLOCK_START case_clauses_opt default_clause case_clauses_opt '}'
	;

case_clauses_opt:
		case_clauses
	|	/* empty */
	;

case_clauses:
		case_clause
	|	case_clauses case_clause
	;

case_clause:
		K_CASE expression ':' statement
	;

default_clause:
		K_DEFAULT ':' statement
	;

iteration_statement
	: K_WHILE '(' expression ')' statement
	| K_DO statement K_WHILE '(' expression ')' ';'
	| K_FOR '(' expression_statement expression_statement expression ')' statement
	;

jump_statement
	: K_CONTINUE ';'
	| K_BREAK ';'
	| K_RETURN ';'
	| K_RETURN expression ';'
	;

function_declaration
	: K_FUNCTION function_definition
	;

function_definition
	: T_IDENTIFIER '(' ')' compound_statement
	| T_IDENTIFIER '(' formal_parameter_list ')' compound_statement
    ;

formal_parameter_list:
		T_IDENTIFIER
    |	formal_parameter_list ',' T_IDENTIFIER
    ;

objectLiteral:
		O_OBJ_START propertyNameAndValueList ']'
    ;

propertyNameAndValueList:
		/* empty */
    |	property
    |	propertyNameAndValueList ',' property
    ;

property:
		assignment_expression
    |	T_IDENTIFIER ':' assignment_expression
    |	T_STRING ':' assignment_expression
    ;

%%
#include <stdio.h>

extern char yytext[];

void yyerror(char* s)
{
	fflush(stdout);
}











#if 0

/*
 _______________________________________________________________________
 |
 |
 |   Description:
 |	This file contains the bison rules for the ucscript Parser
 |
 |   Author(s)		: Chris Marrin
 |
 _______________________________________________________________________
 */

%{
#include "ucParseHandler.h"
#include "ucParser.hpp"
#include "ucAtomList.h"

#ifndef alloca
#define alloca malloc
#endif

#ifdef BISON_DEBUG
#define YYDEBUG 1
#endif

#define YYLEX_PARAM (((UC::ParseHandler*) ph)->getScanner())
#define YYPARSE_PARAM ph

inline void yyerror(const char* s) { /*(UC::ParseHandler* ph)->error(s);*/ }

extern int yylex (YYSTYPE * yylval_param , void* yyscanner);

%}

%union {
	UC::RawAtom	atom;
    char*		string;
    int			keyword;
	int32_t		integer;
	float		number;
};

%type <atom> IDENTIFIER
%type <string> STRING
%type <num> NUMBER

%token IDENTIFIER STRING NUMBER INTEGER HEXNUMBER
%token K_FUNCTION
%token K_TRUE K_FALSE K_NULL
%token K_BREAK K_CONTINUE K_ELSE K_FOR K_IF K_NEW K_RETURN K_WHILE K_THIS K_VAR
%token K_DO K_SWITCH K_CASE K_DEFAULT
%token O_EQ O_NE O_LE O_GE O_LSHIFT O_RSHIFT O_RSHIFTFILL O_AND O_OR
%token O_PLUSEQ O_MINUSEQ O_MULTIPLYEQ O_DIVIDEEQ
%token O_MODEQ O_LSHIFTEQ O_IN O_DELETE O_TYPEOF 
%token O_RSHIFTEQ O_RSHIFTFILLEQ O_ANDEQ O_XOREQ O_OREQ O_INCREMENT O_DECREMENT

%left O_INCREMENT O_DECREMENT O_DELETE O_TYPEOF '!' '~' UMINUS
%left '*' '/' '%'
%left '+' '-'
%left O_RSHIFT O_LSHIFT O_RSHIFTFILL
%left O_LE O_GE '<' '>' O_IN
%left O_EQ O_NE
%left '&'
%left '^'
%left '|'
%left O_AND
%left O_OR

/*  we expect if..then..else to produce a shift/reduce conflict
*/

%pure_parser

%%

program:	sourceElements
    ;

sourceElements:	sourceElement
    |			sourceElements sourceElement
    ;

sourceElement:
		statement
    |	functionDeclaration
    ;
	
primaryExpression:
		K_THIS
    |	IDENTIFIER
    |	K_NULL
    |	K_TRUE
    |	K_FALSE
    |	NUMBER
	|	INTEGER
	|	HEXNUMBER
    |	STRING
    |	objectLiteral
    |	'(' expression ')'
    ;

memberExpression:
		primaryExpression
    |	memberExpression '[' expression ']'
    |	memberExpression '.' IDENTIFIER
    ;

newExpression:
		memberExpression
    |	K_NEW newExpression
    ;

callExpression:
		memberExpression arguments
    |	callExpression arguments
    |	callExpression '[' expression ']'
    |	callExpression '.' IDENTIFIER
    ;

arguments:
		'(' ')'
    |	'(' argumentList ')'
    ;

argumentList:
		expression
    |	argumentList ',' expression
    ;

leftHandSideExpression:
		newExpression
    |	callExpression
    ;
	
expression:
		leftHandSideExpression
	|	leftHandSideExpression assignmentOperator expression
	|	'-' expression %prec UMINUS
	|	'!' expression
	|	'~' expression
	|	O_INCREMENT expression
	|	O_DECREMENT expression
	|	O_DELETE expression
	|	O_TYPEOF expression
	|	expression '%' expression
	|	expression '*' expression
	|	expression '/' expression
	|	expression '+' expression
	|	expression '-' expression
	|	expression O_RSHIFT expression
	|	expression O_RSHIFTFILL expression
	|	expression O_LSHIFT expression
	|	expression '<' expression
	|	expression '>' expression
	|	expression O_LE expression
	|	expression O_GE expression
	|	expression O_IN expression
	|	expression O_EQ expression
	|	expression O_NE expression
	|	expression '&' expression
	|	expression '^' expression
	|	expression '|' expression
	|	expression O_AND expression
	|	expression O_OR expression
	|	expression '?' expression ':' expression
	;
	
expressionOpt:
		expression
	|	/* empty */
	;

assignmentOperator:
		O_MULTIPLYEQ
    |	O_DIVIDEEQ
    |	O_MODEQ
    |	O_PLUSEQ
    |	O_MINUSEQ
    |	O_LSHIFTEQ
    |	O_RSHIFTEQ
    |	O_RSHIFTFILLEQ
    |	O_ANDEQ
    |	O_XOREQ
    |	O_OREQ
	|	'='
    ;

statement:
		block
    |	variableStatement
    |	emptyStatement
    |	ifStatement
    |	iterationStatement
    |	continueStatement
    |	breakStatement
    |	returnStatement
    |	expressionStatement
	|	switchStatement
    ;

block:
		'{' '}'
    |	'{' statementList '}'
    ;

statementList:
		statement
    |	statementList statement
    ;
	
statementListOpt:
		statementList
	|	/* empty */
	;

variableStatement:
		K_VAR variableDeclarationList ';'
    ;

variableDeclarationList:
		variableDeclaration
    |	variableDeclarationList ',' variableDeclaration
    ;

variableDeclaration:
    	IDENTIFIER
    |	IDENTIFIER initializer
    ;

initializer:
    	'=' expression
    ;

emptyStatement:
		';'
    ;

expressionStatement:
		expression ';'
    ;

ifStatement:
		K_IF '(' expression ')' statement K_ELSE statement
    |	K_IF '(' expression ')' statement
    ;

iterationStatement:
		K_DO statement K_WHILE '(' expression ')' ';'
	|	K_WHILE '(' expression ')' statement
    |	K_FOR '(' expressionOpt ';' expressionOpt ';' expressionOpt ')' statement
    |	K_FOR '(' K_VAR variableDeclaration ';' expressionOpt ';' expressionOpt ')' statement
    |	K_FOR '(' leftHandSideExpression O_IN expression ')' statement
    |	K_FOR '(' K_VAR variableDeclaration O_IN expression ')' statement
    ;

continueStatement:
		K_CONTINUE ';'
    ;

breakStatement:
		K_BREAK ';'	
    ;

returnStatement:
		K_RETURN expressionOpt ';'
    ;

switchStatement:
		K_SWITCH '(' expression ')' caseBlock
	;

caseBlock:
		'{' caseClausesOpt '}'
	|	'{' caseClausesOpt defaultClause caseClausesOpt '}'
	;

caseClausesOpt:
		caseClauses
	|	/* empty */
	;

caseClauses:
		caseClause
	|	caseClauses caseClause
	;

caseClause:
		K_CASE expression ':' statementListOpt
	;

defaultClause:
		K_DEFAULT ':' statementListOpt
	;

functionDeclaration:
		K_FUNCTION functionDefinition
	;

functionDefinition:
		IDENTIFIER '(' formalParameterListOpt ')' block
    ;

formalParameterListOpt:
		formalParameterList
    |	/* empty */
    ;

formalParameterList:
		IDENTIFIER
    |	formalParameterList ',' IDENTIFIER
    ;

objectLiteral:
		'[' propertyNameAndValueList ']'
    ;

propertyNameAndValueList:
		/* empty */
    |	propertyName ':' expression
    |	propertyNameAndValueList ',' propertyName ':' expression
    ;

propertyName:
		IDENTIFIER
    |	STRING
    |	NUMBER
    ;

%%

#endif