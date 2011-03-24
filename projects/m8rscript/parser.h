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

#include "ucMemory.h"
#include "ucAtomList.h"
#include "ucScanner.h"

namespace UC {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Parser
//
//  
//
//////////////////////////////////////////////////////////////////////////////

#define NO_BLOCK 255
#define BYTE_CODE_SIZE_INIT		50
#define BYTE_CODE_SIZE_GROW		50
#define BLOCK_ADDR_SIZE_INIT	10
#define BLOCK_ADDR_SIZE_GROW	5

// byte codes are 1 byte entries. Each entry either pushes something
// onto the stack or operates on things on the stack, removing and/or
// adding entries to the stack as a result.

class Parser {
public:
	Parser(uint8_t (*getByteFunc)())
	 : myScanner(&myAtomList, getByteFunc)
	 , myByteCode(BYTE_CODE_SIZE_INIT)
	 , myByteCodeIndex(0)
	 , myBlockAddr(BLOCK_ADDR_SIZE_INIT)
	 , myBlockAddrIndex(NO_BLOCK)
	{
		myScanner.getToken(myTokenValue);
	}
	
	void program()
	{
		while (!isDone()) {
			if (getToken() == K_FUNCTION) {
				advance();
				function(true);
			}
			else
				statement();
		}
	}

	void function(bool isStatement);
		
	void statementList()			{ while(statement()) ; }
	
	bool statement();
	
	bool expressionStatement()
	{
		if (!expression())
			return false;
		if (tokenError(';')) return false;
		add(OP_POP);
		return true;
	}
	
	bool ifStatement();
		
	bool compoundStatement()
	{
		if (getToken() == '{') {
			advance();
			statementList();
			return !tokenError('}');
		}
		return false;
	}

	bool switchStatement();	
	bool forStatement();	
	bool foreachStatement();	
	bool doStatement();	
	bool whileStatement();
	
	bool primaryExpression();	
	bool postfixExpression();	
	bool unaryExpression();
	bool expression(uint8_t level=0);
		
	uint8_t argumentExpressionList()
	{
		for (uint8_t n = 0; ; ++n) {
			if (!expression()) {
				if (n == 0)
					return 0;
				// error
				return 0;
			}
			
			addAtom(T_IDENTIFIER);
			advance();
			if (getToken() != ',')
				return n;
			advance();
		}
	}

	uint8_t formalParameterList()
	{
		for (uint8_t n = 0; ; ++n) {
			if (getToken() != T_IDENTIFIER) {
				if (n == 0)
					return 0;
				// error
				return 0;
			}
			
			addAtom(T_IDENTIFIER);
			advance();
			if (getToken() != ',')
				return n;
			advance();
		}
	}
	
	bool objectLiteral()
	{
		if (getToken() == '{') {
			advance();
			propertyNameAndValueList();
			if (tokenError('}')) return false;
			return true;
		}
		return false;
	}

	void varDeclList()
	{
		if (tokenError(T_IDENTIFIER)) return;
		addAtom(T_IDENTIFIER);
		advance();
		if (getToken() == '=') {
			advance();
			expression();
		}
	}
	
	void propertyNameAndValueList()
	{
		uint8_t n = 0;
		if (expression()) {
			n = 1;
			while(getToken() == ',') {
				if (!expression())
					return; // error
				if (getToken() == ':') {
					if (!expression())
						return; // error
				}
				n++;
			}
		}
		add(OP_ARG_LIST);
		add(n);
	}
	
	void enterBlock()				{ myBlockAddrIndex++; }
	void leaveBlock()
	{
		uint16_t size = myByteCodeIndex - myBlockAddr[myBlockAddrIndex] - 1;
		myByteCode[myBlockAddr[myBlockAddrIndex--]] = size;
	}
	
	bool op(uint8_t level, uint8_t token)
	{
		for (uint8_t* p = mygOps[level]; *p; p++)
			if (*p == token)
				return true;
		return false;
	}
	
	bool isDone()					{ return myTokenValue.token == C_EOF; }
	uint8_t getToken()				{ return myTokenValue.token; }
	void advance()
	{
		myScanner.getToken(myTokenValue);
	}
	
	void addAtom(uint8_t op)
	{
		uint16_t atom = myTokenValue.atom;
		makeSpace(3);
		uint8_t* p = &(myByteCode[myByteCodeIndex]);
		*p++ = op;
		*p++ = atom >> 8;
		*p++ = atom & 0xff;
		myByteCodeIndex += 3;
	}
		
	void addInt()
	{
		int32_t i = myTokenValue.integer;
		uint8_t* p = &(myByteCode[myByteCodeIndex]);
		if (i <= 32 && i >= -32) {
			makeSpace(1);
			*p++ = OP_INT0 | ((uint8_t) i) & 0x3f;
			myByteCodeIndex += 1;
		}
		else if (i <= 127 && i >= -128) {
			makeSpace(2);
			*p++ = OP_INT1;
			*p++ = (uint8_t) i;
			myByteCodeIndex += 2;
		}
		else if (i <= 32767 && i >= -32768) {
			union { int16_t i; uint8_t c[2]; } u;
			u.i = (int16_t) i;
			makeSpace(3);
			*p++ = OP_INT2;
			*p++ = u.c[0];
			*p++ = u.c[1];
			myByteCodeIndex += 3;
		}
		else {
			union { int32_t i; uint8_t c[2]; } u;
			u.i = i;
			makeSpace(5);
			*p++ = OP_INT4;
			*p++ = u.c[0];
			*p++ =  u.c[1];
			*p++ =  u.c[2];
			*p++ =  u.c[3];
			myByteCodeIndex += 5;
		}
	}
		
	void addFloat()
	{
		union { FPF f; uint8_t c[4]; } u;
		u.f = myTokenValue.number;
		makeSpace(5);
		uint8_t* p = &(myByteCode[myByteCodeIndex]);
		*p++ = T_FLOAT;
		*p++ = u.c[0];
		*p++ = u.c[1];
		*p++ = u.c[2];
		*p++ = u.c[3];
		myByteCodeIndex += 5;
	}
		
	void addFunctionCall(uint8_t nargs)
	{
		add(OP_FUNCTION_CALL);
		add(nargs);
	}
	
	void addJumpFalse(uint8_t i)
	{
	}
		
	void addJump(uint8_t i)
	{
	}
	
	void resolveJump(uint8_t i)
	{
	}
		
	void add(uint8_t token)
	{
		makeSpace(1);
		myByteCode[myByteCodeIndex++] = token;
	}
	
	void addEmptyBlock()
	{
		makeSpace(1);
		myByteCode[myByteCodeIndex++] = OP_EMPTY_BLOCK;
	}
	
	bool tokenError(uint8_t token);
	
	void* getScanner()	{ return (void*) &myScanner; }

private:
	void				makeSpace(uint8_t size)
	{
		if (myByteCode.getSize() < myByteCodeIndex+size)
			myByteCode.resize(myByteCode.getSize() + size);
	}
	
	AtomList			myAtomList;
	Scanner				myScanner;
	
	Array<uint8_t>		myByteCode;
	uint16_t			myByteCodeIndex;
	Array<uint16_t>		myBlockAddr;
	uint8_t				myBlockAddrIndex;			// can never nest more than 256 blocks deep
	
	static uint8_t*		mygOps[];
	
	TokenValue			myTokenValue;
};

}
