%{

#include "config.h"
#include "ucParse.hpp"
#include "ucParser.h"
#include "ucAtomList.h"

#define YY_EXTRA_TYPE UC::Parser*
#define YY_NO_INPUT
#define YY_NO_UNISTD_H

#ifndef EINTR
#define	EINTR		4		/* Interrupted system call */
#define	EINVAL		22		/* Invalid argument */
#define	ENOMEM		12		/* Cannot allocate memory */
static int	fileno(FILE *)				{ return 0; }
#endif
extern "C" {
	static int yyinput(void* yyscanner)		{ return 0; }
	static int yywrap(yyscan_t yyscanner)	{ return 1; }
	static bool isatty(int)					{ return false; }
}

%}

%option reentrant
%option yylineno
%option bison-bridge

D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+

%{
#include <stdio.h>

%}

%%
"/*"			{
					char c, c1;

				loop:
					while ((c = yyinput(yyscanner)) != '*' && c != 0)
						putchar(c);

					if ((c1 = yyinput(yyscanner)) != '/' && c != 0)
					{
						unput(c1);
						goto loop;
					}

					if (c != 0)
						putchar(c1);
				}


"function"		{ return(K_FUNCTION); }
"true"			{ return(K_TRUE); }
"false"			{ return(K_FALSE); }
"null"			{ return(K_NULL); }
"this"			{ return(K_THIS); }
"in"			{ return(K_IN); }
"delete"		{ return(K_DELETE); }
"var"			{ return(K_VAR); }
"new"			{ return(K_NEW); }
"break"			{ return(K_BREAK); }
"case"			{ return(K_CASE); }
"continue"		{ return(K_CONTINUE); }
"default"		{ return(K_DEFAULT); }
"do"			{ return(K_DO); }
"else"			{ return(K_ELSE); }
"for"			{ return(K_FOR); }
"if"			{ return(K_IF); }
"return"		{ return(K_RETURN); }
"switch"		{ return(K_SWITCH); }
"typeof"		{ return(K_TYPEOF); }
"while"			{ return(K_WHILE); }

{L}({L}|{D})*	{ return(T_IDENTIFIER); }

0[xX]{H}+		{ return(T_INTEGER); }
{D}+			{ return(T_INTEGER); }
{D}+{E}			{ return(T_FLOAT); }
{D}*"."{D}+({E})?	{ return(T_FLOAT); }
{D}+"."{D}*({E})?	{ return(T_FLOAT); }

\"(\\.|[^\\"])*\"	{ return(T_STRING); }
\'(\\.|[^\\'])*\'	{ return(T_STRING); }

">>="			{ return(O_RSHIFTEQ); }
">>>="			{ return(O_RSHIFTFILLEQ); }
"<<="			{ return(O_LSHIFTEQ); }
"+="			{ return(O_ADDEQ); }
"-="			{ return(O_SUBEQ); }
"*="			{ return(O_MULEQ); }
"/="			{ return(O_DIVEQ); }
"%="			{ return(O_MODEQ); }
"&="			{ return(O_ANDEQ); }
"^="			{ return(O_XOREQ); }
"|="			{ return(O_OREQ); }
">>"			{ return(O_RSHIFT); }
">>>"			{ return(O_RSHIFTFILL); }
"<<"			{ return(O_LSHIFT); }
"++"			{ return(O_INC); }
"--"			{ return(O_DEC); }
"&&"			{ return(O_LAND); }
"||"			{ return(O_LOR); }
"<="			{ return(O_LE); }
">="			{ return(O_GE); }
"=="			{ return(O_EQ); }
"!="			{ return(O_NE); }
";"				{ return(';'); }
"{"				{ return('{'); }
"}"				{ return('}'); }
","				{ return(','); }
":"				{ return(':'); }
"="				{ return('='); }
"("				{ return('('); }
")"				{ return(')'); }
"["				{ return('['); }
"]"				{ return(']'); }
"."				{ return('.'); }
"&"				{ return('&'); }
"!"				{ return('!'); }
"~"				{ return('~'); }
"-"				{ return('-'); }
"+"				{ return('+'); }
"*"				{ return('*'); }
"/"				{ return('/'); }
"%"				{ return('%'); }
"<"				{ return('<'); }
">"				{ return('>'); }
"^"				{ return('^'); }
"|"				{ return('|'); }
"?"				{ return('?'); }

[ \t\v\n\f]		{ }
.				{ /* ignore bad characters */ }

%%






























#if 0

%{

#include "ucParseHandler.h"
#include "ucParser.hpp"
#include "ucDataStream.h"
#include "ucAtomList.h"

#define YY_EXTRA_TYPE UC::ParseHandler*

static char *getStringConstant(void* yyscanner, char delimiter);

extern "C" {
	static int yywrap(yyscan_t yyscanner) { return 1; }
}

%}

%option reentrant
%option yylineno
%option bison-bridge

integer			(0|[1-9][0-9]*)
mantissa		(({integer}\.[0-9]*)|(\.[0-9]+)|({integer}))
exponent		([Ee][\+\-]?[0-9]+)

number			{mantissa}{exponent}?

hexnumber		(0[xX][0-9a-fA-F]*)

/* Whitespace pattern without the newline (so we can count line numbers) */
ws				([ \t\r\n])

/* Legal characters to start an identifier */
idStartChar		([a-zA-Z\$_\x80-\xFF])
/* Legal other characters in an identifier */
idRestChar		({idStartChar}|[0-9])

%x COMMENT

%%

function		{ return K_FUNCTION; }
true			{ return K_TRUE; }
false			{ return K_FALSE; }
null			{ return K_NULL; }
break			{ return K_BREAK; }
continue		{ return K_CONTINUE; }
else			{ return K_ELSE; }
for				{ return K_FOR; }
if				{ return K_IF; }
in				{ return O_IN; }
new				{ return K_NEW; }
return			{ return K_RETURN; }
do				{ return K_DO; }
while			{ return K_WHILE; }
this			{ return K_THIS; }
delete			{ return O_DELETE; }
typeof			{ return O_TYPEOF; }
var				{ return K_VAR; }
switch			{ return K_SWITCH; }
case			{ return K_CASE; }
default			{ return K_DEFAULT; }

{idStartChar}{idRestChar}*	{
								yylval->atom = yyextra->getAtomList().get(yytext);
								return IDENTIFIER; 
							}

{integer}		{ yylval->integer = atol(yytext); return INTEGER; }
{number}		{ yylval->number = atof(yytext); return NUMBER; }
{hexnumber}		{
					char* stopString;
					yylval->number = (double) strtoul(yytext, &stopString, 0);
					return HEXNUMBER;
				}

"/*"			BEGIN COMMENT;
<COMMENT>"*/"	BEGIN 0;
<COMMENT>.|\n	;

"//".*\n		;



\"				{
					/* single and double quote enclosed string constants */
					char *string = getStringConstant(yyscanner, '\"');
					yylval->string = string;
					return STRING;
				}

"'"				{
					/* single and double quote enclosed string constants */
					char *string = getStringConstant(yyscanner, '\'');
					yylval->string = string;
					return STRING;
				}

">="			{ return O_GE; }
"=="			{ return O_EQ; }
"<="			{ return O_LE; }
"!="			{ return O_NE; }
">>"			{ return O_RSHIFT; }		
"<<"			{ return O_LSHIFT; }		
">>>"			{ return O_RSHIFTFILL; }	
"&&"			{ return O_AND; }
"||"			{ return O_OR; }
"+="			{ return O_PLUSEQ; } 
"-="			{ return O_MINUSEQ; } 
"*="			{ return O_MULTIPLYEQ; } 
"/="			{ return O_DIVIDEEQ; } 
"%="			{ return O_MODEQ; } 
"<<="			{ return O_LSHIFTEQ; }		
">>="			{ return O_RSHIFTEQ; }		
">>>="			{ return O_RSHIFTFILLEQ; }	
"&="			{ return O_ANDEQ; }		
"^="			{ return O_XOREQ; }		
"|="			{ return O_OREQ; }		
"++"			{ return O_INCREMENT; } 
"--"			{ return O_DECREMENT; } 

.				return yytext[0]; /* anything else */

%%

static char *getStringConstant(void* yyscanner, char delimiter)
{
    int len = 0;
    int maxlen = 64;
    char cChar;
    char *startChar = new char[maxlen+1];
    while ((cChar = yyinput(yyscanner)) != delimiter) {
		if (cChar == EOF)
			return NULL;

		if (len == maxlen) {
			char* tmpStr = new char[2*maxlen+1];
			memcpy(tmpStr, startChar, maxlen);
			maxlen *= 2;
			delete startChar;
			startChar = tmpStr;
		}
		startChar[len++] = cChar;
    }
    startChar[len] = '\0';

    return startChar;
}

#endif