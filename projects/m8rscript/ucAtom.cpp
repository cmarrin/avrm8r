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

#include "ucAtom.h"

using namespace UC;

OrderedArray<String, uint16_t> Atom::mygAtoms;

// Atom numbers are one greater than their actual index in the
// list, so we can accomodate an atom of 0 as NO_ATOM
void
Atom::add(const char* s, int32_t n)
{
	// short circuit if we are setting to an empty atom
	if (!s || n == 0 || (n == -1 && s[0] == '\0')) {
		if (myAtom != NO_ATOM) {
			unref();
			myAtom = NO_ATOM;
		}
		return;
	}
	
	// for maximum efficiency do a quick test if we can avoid
	// creating a string
	if (n < 0 && *this == s)
		return;

	String str(s,n);
	
	// do another quick test with the newly created string
	if (*this == str.getString())
		return;
	
	// toss the old atom
	if (myAtom != NO_ATOM)
		unref();
	
	// see if it already exists
	int32_t i = mygAtoms.find(str);
	if (i >= 0) {
		myAtom = i;
		ref();
		return;
	}
	
	// does not exist, add it
	if (mygAtoms.getNum() >= MAX_ATOM) {
		fprintf(stderr, "TOO MANY ATOMS!!!\n");
		return;
	}
	
	myAtom = mygAtoms.insert(str, 0);
	ref();
}

