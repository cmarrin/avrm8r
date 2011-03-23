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

#pragma once

#include "config.h"
#include "ucAtomList.h"
#include "ucMemory.h"

namespace UC {

// These values must match the strings in Scanner.cpp

// keywords go from 1-32, where the control codes and space go
#define KEYWORD_OFFSET	1

#define K_FUNCTION	1
#define K_NEW		2
#define K_DELETE	3
#define K_TYPEOF	4
#define K_IN		5
#define K_VAR		6

#define FIRST_CONST_TERM 7
#define K_TRUE		7
#define K_FALSE		8
#define K_NULL		9
#define K_THIS		10
#define LAST_CONST_TERM 10

#define K_DO		11
#define K_WHILE		12
#define K_FOR		13
#define K_FOREACH	14
#define K_IF		15
#define K_ELSE		16
#define K_SWITCH	17
#define K_CASE		18
#define K_DEFAULT	19
#define K_BREAK		20
#define K_CONTINUE	21
#define K_RETURN	22

// 48-57 are digits and can be used
#define T_FLOAT			48
#define T_IDENTIFIER	49
#define T_STRING		50
#define T_INTEGER		51

// operators go at 65-90, upper case letters
#define OPERATOR_OFFSET	65

#define FIRST_ASSIGNMENT_OPERATOR 65
#define O_RSHIFTEQ		65
#define O_RSHIFTFILLEQ	66
#define O_LSHIFTEQ		67
#define O_ADDEQ			68
#define O_SUBEQ			69
#define O_MULEQ			70
#define O_DIVEQ			71
#define O_MODEQ			72
#define O_ANDEQ			73
#define O_XOREQ			74
#define O_OREQ			75
#define LAST_ASSIGNMENT_OPERATOR 75

#define O_RSHIFT		76
#define O_RSHIFTFILL	77
#define O_LSHIFT		78
#define O_INC			79
#define O_DEC			80
#define O_LAND			81
#define O_LOR			82
#define O_LE			83
#define O_GE			84
#define O_EQ			85
#define O_NE			86
#define O_BLOCK_START	87
#define O_OBJ_START		88

// 97-122 are lowercase letters and can be used
#define OP_POP			97
#define OP_BLOCK		98				// A block follows. Next byte is the length of the block, followed by the opcodes of that block
#define OP_EMPTY_BLOCK	99
#define OP_ARG_LIST		100
#define OP_INT1			101
#define OP_INT2			102
#define OP_INT4			103
#define OP_REF			104
#define OP_POST_INC		105
#define OP_POST_DEC		106
#define OP_FUNCTION_CALL 107

#define E_ERROR			191

#define OP_INT0			0xC0			// from 0xC0 (192) to 0xFF (255) is used for 6 bit int opcode

#define C_EOF			255

#define MAX_ID_LENGTH	32

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Scanner
//
//  
//
//////////////////////////////////////////////////////////////////////////////

typedef struct {
	union { int32_t integer; FPF number; uint16_t atom; };
	uint8_t token;
} TokenValue;	

class Scanner  {
public:
	Scanner(AtomList* atomList, uint8_t (*getByteFunc)())
	 : m_lastChar(255)
	 , myAtomList(atomList)
	 , myGetByteFunc(getByteFunc)
	{ }
	
	~Scanner()
	{ }

	void	getToken(TokenValue& token);
	
private:
	uint8_t getChar();

	uint8_t		m_lastChar;
	Array<char> myTokenString;
	AtomList*	myAtomList;
	uint8_t		(*myGetByteFunc)();
};

}
