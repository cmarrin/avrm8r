/*-------------------------------------------------------------------------
This source file is a part of Fusion

For the latest info, see http://www.emma3d.org/

Copyright (c) 2006, Murat Aktihanoglu, Chris Marrin, Rob Myers
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

    - Redistributions of source code must retain the above copyright notice, 
	  this list of conditions and the following disclaimer.
	  
    - Redistributions in binary form must reproduce the above copyright 
	  notice, this list of conditions and the following disclaimer in the 
	  documentation and/or other materials provided with the distribution.
	  
    - Neither the name of the <ORGANIZATION> nor the names of its 
	  contributors may be used to endorse or promote products derived from 
	  this software without specific prior written permission.
	  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------*/

#include "config.h"
#include "ucScanner.h"

using namespace UC;

static const char gKeywords[] =
	"function\0"
	"new\0"
	"delete\0"
	"typeof\0"
	"in\0"
	"var\0"
	"true\0"
	"false\0"
	"null\0"
	"do\0"
	"while\0"
	"for\0"
	"foreach\0"
	"if\0"
	"else\0"
	"switch\0"
	"case\0"
	"default\0"
	"break\0"
	"continue\0"
	"return\0"
	"this\0"
	"\0"
;

static const char gOperators[] =
	">>=\0"
	">>>=\0"
	"<<=\0"
	"+=\0"
	"-=\0"
	"*=\0"
	"/=\0"
	"%=\0"
	"&=\0"
	"^=\0"
	"|=\0"
	">>\0"
	">>>\0"
	"<<\0"
	"++\0"
	"--\0"
	"&&\0"
	"||\0"
	"<=\0"
	">=\0"
	"==\0"
	"!=\0"
	"\0"
;

/*
	invalid chars: '#', '\'
	"!"		0x21
	"%"		0x25
	"&"		0x26
	"("		0x28
	")"		0x29
	"*"		0x2a
	"+"		0x2b
	","		0x2c
	"-"		0x2d
	"."		0x2e
	"/"		0x2f
	
	":"		0x3a
	";"		0x3b
	"<"		0x3c
	"="		0x3d
	">"		0x3e
	"?"		0x3f
	
	"["		0x5b
	"]"		0x5d
	"^"		0x5e
	
	"{"		0x7b
	"|"		0x7c
	"}"		0x7d
	"~"		0x7e
	
;
*/

static inline bool isDigit(uint8_t c)		{ return c >= '0' && c <= '9'; }
static inline bool isHexDigit(uint8_t c)	{ return isDigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'); }
static inline bool isUpper(uint8_t c)		{ return (c >= 'A' && c <= 'Z'); }
static inline bool isLower(uint8_t c)		{ return (c >= 'a' && c <= 'z'); }
static inline bool isLetter(uint8_t c)		{ return isUpper(c) || isLower(c); }
static inline bool isIdFirst(uint8_t c)		{ return isLetter(c) || c == '$' || c == '_'; }
static inline bool isIdOther(uint8_t c)		{ return isDigit(c) || isIdFirst(c); }
static inline bool isSpecial(uint8_t c)		{ return !isDigit(c) && !isIdFirst(c) && c >= 0x21 && c <= 0x7e; }
static inline uint8_t toLower(uint8_t c)	{ return isUpper(c) ? (c-'A'+'a') : c; }

uint8_t
Scanner::getChar()
{
	return myGetByteFunc();
}

static uint8_t scanList(const char* list, const char* token, uint16_t n)
{
	// list is a sequence of null terminated strings. The table ends when a bare \0 is seen.
	// find the pass string in that list and return the order of the string matched, or 0 if
	// no match
	uint8_t i = 0;
	
	for (const char* p = list; *p != '\0'; ++p) {
		// test a word
		bool found = true;
		for (const char* s = token; *s != '\0'; ++s) {
			if (*s != *p++) {
				found = false;
				break;
			}
		}
		
		if (found)
			return i;
			
		// not found skip to the next word
		while (*p++ != '\0')
			;
		++i;
	}
	
	return C_EOF;
}

void
Scanner::getToken(TokenValue& token)
{
	int c = (m_lastChar == 255) ? getChar() : m_lastChar;
	m_lastChar = 255;
	token.token = C_EOF;
	
	if (c == C_EOF)
		return;
	
	if (isDigit(c)) {
		// scanning a number <digit>+['.'<digit>*]?[['E'|'e']['+'|'-']?<digit>+]?
		//            state:    0      1     2         3       4        5         6
		// or
		//                   '0'['x'|'X']<hexdigit>+
		uint8_t c1 = (uint8_t) c;
		c = getChar();
		if (c == C_EOF)
			return;
		
		// check for hex
		if (c1 == '0' && (c == 'x' || c == 'X')) {
			int32_t n = 0;
			while (isHexDigit(c = getChar())) {
				n *= 16;
				n += isDigit(c) ? (c-'0') : (toLower(c)-'a'+10);
			}
			
			if (c != C_EOF)
				m_lastChar = (uint8_t) c;
				
			token.integer = n;
			token.token = T_INTEGER;
			return;
		}
		else {
			uint8_t state = 0;
			bool needChar = false;
			uint32_t i = 0;
			uint32_t f = 0;
			uint8_t dp = 0;
			int32_t e = 0;
			bool negExp = false;
			
			while (1) {
				switch(state) {
					case 0:	// digits before decimal
						if (isDigit(c)) {
							i = i*10+(c-'0');
							needChar = true;
						}
						else
							state = 1;
						break;
					case 1: // decimal point
						if (c == '.') {
							needChar = true;
							state = 2;
						}
						else
							state = 3;
						break;
					case 2: // digits after decimal
						if (isDigit(c)) {
							f = f*10+(c-'0');
							dp++;
							needChar = true;
						}
						else
							state = 3;
						break;
					case 3: // exponent
						if (c == 'e' || c == 'E') {
							needChar = true;
							state = 4;
						}
						else
							state = 6;
						break;
					case 4: // exponent + or -
						if (c == '+' || c == '-') {
							if (c == '-')
								negExp = true;
							needChar = true;
						}
						state = 5;
						break;
					case 5: // exponent number
						if (isDigit(c)) {
							e = e*10+(c-'0');
							needChar = true;
						}
						else
							state = 6;
						break;
				}
				
				if (needChar) {
					needChar = false;
					c = getChar();
					if (c == C_EOF) {
						if (state == 0 || state == 2 || state == 5)
							state = 6;
						else {
							token.token = E_ERROR;
							return;
						}
					}
				}
				
				if (state == 6) {
					if (e == 0 && f == 0 && dp == 0) {
						// return integer
						token.integer = i;
						token.token = T_INTEGER;
						return;
					}
					else {
						if (negExp)
							e = -e;
						token.number = FPF_MAKE(i, f, dp, e);
						token.token = T_FLOAT;
						return;
					}
				}
			}
		}
	}
	else if (isIdFirst(c)) {
		// scanning identifier or keyword <idfirst><idother>*
		myTokenString.resize(MAX_ID_LENGTH);
		char* p = &(myTokenString[0]);
		uint16_t n = MAX_ID_LENGTH;
	
		while (isIdOther(c = getChar()) && n-- > 1)
			*p++ = (char) c;
			
		if (c != C_EOF)
			m_lastChar = (uint8_t) c;
			
		// make it an atom
		const char* s = &(myTokenString[0]);
		uint16_t len = p-s;
		
		// see if this is a keyword
		uint8_t keyword = scanList(gKeywords, p, len);
		if (keyword != C_EOF)
			token.token = keyword+KEYWORD_OFFSET;
		else {
			token.atom = myAtomList->add(s, len);
			token.token = T_IDENTIFIER;
		}
		return;
	}
	else if (c == '\'' || c == '"') {
		// scan string
		char* p = &(myTokenString[0]);
		uint8_t match = (uint8_t) c;
		
		--p;	// toss quote
		
		while(1) {
			c = getChar();
			if (c != match) {
				if (c == '\\') {
					c = getChar();
					switch(c) {
						case 'n':	c = '\n'; break;
						case 't':	c = '\t'; break;
						case 'r':	c = '\r'; break;
						case '\'':	c = '\''; break;
						case '"':	c = '"'; break;
						// handle the rest later
					}
				}
				
				uint16_t n = p-&(myTokenString[0]);
				if (myTokenString.getSize() < n - 2)
					myTokenString.resize(myTokenString.getSize() + MAX_ID_LENGTH);
					
				p = &(myTokenString[n]);
				*p++ = (char) c;
			}
		}
		
		char* s = &(myTokenString[0]);
		token.atom = myAtomList->add(s, p-s);
		token.token = T_STRING;
		return;
	}
	else if (isSpecial(c)) {
		// scanning special char sequence <special>+
		char s[5];
		char* p = s;
		uint8_t n = 4;
		*p++ = c;
		
		while (isSpecial(c = getChar()) && n-- > 0)
			*p++ = (char) c;
		
		*p = '\0';
		
		if (c != C_EOF)
			m_lastChar = (uint8_t) c;
			
		if (s[1] == '\0') {
			// single character case
			token.token = (s[0] == '#' || s[0] == '\\') ? C_EOF : s[0];
			return;
		}

		token.token = scanList(gOperators, &(s[0]), p-&(s[0])) + OPERATOR_OFFSET;
		return;
	}
	
	token.token = C_EOF;
}

