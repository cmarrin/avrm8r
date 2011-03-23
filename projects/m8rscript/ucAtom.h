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

#include "ucOrderedArray.h"
#include "ucString.h"

namespace UC {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Atom
//
//  Class for UTF8 string atoms
//
//////////////////////////////////////////////////////////////////////////////

// atom indexes are uint16_t, so make NO_ATOM be one past the end of the list
#define NO_ATOM 32767
#define MAX_ATOM 32766

typedef uint16_t RawAtom;

class Atom  {
public:
	Atom()									{ myAtom = NO_ATOM; }
	Atom(const String& str)					{ myAtom = NO_ATOM; add(str.getString(), -1); }
	Atom(const char* str, int32_t n = -1)	{ myAtom = NO_ATOM; add(str, n); }
	Atom(RawAtom a)							{ *this = a; }
	Atom(const Atom& a)						{ *this = a; }

	~Atom() { unref(); }

    size_t			getLength() const		{ return (myAtom == NO_ATOM) ? 0 : mygAtoms.getKey(myAtom).getLength(); }
    const String&	getString() const		{ return (myAtom == NO_ATOM) ? String::getEmptyString() : mygAtoms.getKey(myAtom); }

	bool			isEmpty() const			{ return myAtom == NO_ATOM; }

    operator const String&() const			{ return getString(); }
    operator double() const					{ return (double) (getString()); }
    operator int32_t() const				{ return (int32_t) (getString()); }
    operator uint32_t() const				{ return (uint32_t) (getString()); }

	Atom&		operator =(const Atom& a)	{ myAtom = a.myAtom; ref(); return *this; }
	
	friend bool operator ==(const Atom &a, const char *s)	{ return s ? (a.getString() == s) : (a.myAtom == NO_ATOM); }
	friend bool operator ==(const char *s, const Atom &a)	{ return a == s; }
    friend bool operator ==(const Atom &a1, const Atom &a2)	{ return a1.myAtom == a2.myAtom; }

	friend bool operator !=(const Atom &a, const char *s)	{ return s ? (a.getString() == s) : (a.myAtom != NO_ATOM); }
	friend bool operator !=(const char *s, const Atom &a)	{ return a != s; }
    friend bool operator !=(const Atom &a1, const Atom &a2)	{ return a1.myAtom != a2.myAtom; }

	friend bool operator <(const Atom &a, const char *s)	{ return s ? (a.getString() < s) : false; }
	friend bool operator <(const char *s, const Atom &a)	{ return a > s; }
    friend bool operator <(const Atom &a1, const Atom &a2)	{ return a1.myAtom < a2.myAtom; }

	friend bool operator <=(const Atom &a, const char *s)	{ return s ? (a.getString() <= s) : (a.myAtom == NO_ATOM); }
	friend bool operator <=(const char *s, const Atom &a)	{ return a >= s; }
    friend bool operator <=(const Atom &a1, const Atom &a2)	{ return a1.myAtom <= a2.myAtom; }

	friend bool operator >(const Atom &a, const char *s)	{ return s ? (a.getString() > s) : true; }
	friend bool operator >(const char *s, const Atom &a)	{ return a < s; }
    friend bool operator >(const Atom &a1, const Atom &a2)	{ return a1.myAtom > a2.myAtom; }

	friend bool operator >=(const Atom &a, const char *s)	{ return s ? (a.getString() >= s) : true; }
	friend bool operator >=(const char *s, const Atom &a)	{ return a <= s; }
    friend bool operator >=(const Atom &a1, const Atom &a2)	{ return a1.myAtom >= a2.myAtom; }

private:
	static OrderedArray<String, uint16_t> mygAtoms;
	
	void ref()
	{
		if (myAtom != NO_ATOM)
			mygAtoms[myAtom]++;
	}
	
	void unref()
	{
		if (myAtom != NO_ATOM) {
			assert(mygAtoms[myAtom] > 0);
			if (mygAtoms[myAtom]-- == 0)
				mygAtoms.removeIndex(myAtom);
		}
	}
	
	void add(const char* s, int32_t n);
	
	RawAtom myAtom;
};

}
