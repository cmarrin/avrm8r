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

namespace UC {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: AtomList
//
//  Class for lists of UTF8 string atoms
//
//////////////////////////////////////////////////////////////////////////////

// atom indexes are uint16_t, so make NO_ATOM be one past the end of the list
#define NO_ATOM 32767
#define MAX_ATOM 32766

typedef uint16_t RawAtom;

class AtomList {
public:
	AtomList() : myCurSize(0) { }
	
	uint16_t add(const char* s, uint16_t n=0);
	
	bool isEQ(uint16_t atom1, uint16_t atom2) const					{ return compare(atom1, atom2) == 0; }
	bool isNE(uint16_t atom1, uint16_t atom2) const					{ return compare(atom1, atom2) != 0; }
	bool isLT(uint16_t atom1, uint16_t atom2) const					{ return compare(atom1, atom2) < 0; }
	bool isLE(uint16_t atom1, uint16_t atom2) const					{ return compare(atom1, atom2) <= 0; }
	bool isGT(uint16_t atom1, uint16_t atom2) const					{ return compare(atom1, atom2) > 0; }
	bool isGE(uint16_t atom1, uint16_t atom2) const					{ return compare(atom1, atom2) >= 0; }

	bool isEQ(uint16_t atom, const char* s, uint16_t n=0) const		{ return compare(atom, s, n) == 0; }
	bool isNE(uint16_t atom, const char* s, uint16_t n=0) const		{ return compare(atom, s, n) != 0; }
	bool isLT(uint16_t atom, const char* s, uint16_t n=0) const		{ return compare(atom, s, n) < 0; }
	bool isLE(uint16_t atom, const char* s, uint16_t n=0) const		{ return compare(atom, s, n) <= 0; }
	bool isGT(uint16_t atom, const char* s, uint16_t n=0) const		{ return compare(atom, s, n) > 0; }
	bool isGE(uint16_t atom, const char* s, uint16_t n=0) const		{ return compare(atom, s, n) >= 0; }

private:
	int8_t compare(uint16_t atom1, uint16_t atom2) const;
	int8_t compare(uint16_t atom, const char* s, uint16_t n) const;
	uint16_t getSize(uint16_t atom) const;
	
	Array<char> myAtoms;
	uint16_t myCurSize;
};

}
