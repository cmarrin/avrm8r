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

#include <string>
#include <stdarg.h>
#include "ucVector.h"

namespace UC {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: String
//
//  Class for UTF8 string manipulation and conversion between UTF8 and Unicode
//
//////////////////////////////////////////////////////////////////////////////

class String : public std::basic_string<char> {
public:

	String()		{ }
	String(const String& str) : std::basic_string<char>(str) { }
	String(const char* str, int32_t n = -1)
	{
		if (str == NULL)
			assign("");
		else if (n < 0)
			assign(str);
		else
			assign(str, n);
	}

	~String() { }

    int				getLength() const	{ return (int)size(); }
    const char*		getString() const	{ return data(); }

	bool			isEmpty() const		{ return getString()[0] == '\0'; }

	String &		operator =(const String& str)
	{
		using namespace std;
		basic_string<char>::operator =(str);
		return *this;
	}

	String &		operator =(const char* str)
	{
		using namespace std;
		basic_string<char>::operator =(str ? str : "");
		return *this;
	}

	friend bool operator ==(const String &str, const char *s)			{ return s ? (str.compare(s) == 0) : false; }
	friend bool operator ==(const char *s, const String &str)			{ return s ? (str.compare(s) == 0) : false; }
    friend bool operator ==(const String &str1, const String &str2)	{ return str1.compare(str2.getString()) == 0; }

	friend bool operator !=(const String &str, const char *s)			{ return s ? (str.compare(s) != 0) : true; }
	friend bool operator !=(const char *s, const String &str)			{ return s ? (str.compare(s) != 0) : true; }
    friend bool operator !=(const String &str1, const String &str2)	{ return str1.compare(str2.getString()) != 0; }

	friend bool operator <(const String &str, const char *s)			{ return s ? (str.compare(s) < 0) : false; }
	friend bool operator <(const char *s, const String &str)			{ return s ? (str.compare(s) > 0) : true; }
    friend bool operator <(const String &str1, const String &str2)	{ return str1.compare(str2.getString()) < 0; }

	friend bool operator <=(const String &str, const char *s)			{ return s ? (str.compare(s) <= 0) : false; }
	friend bool operator <=(const char *s, const String &str)			{ return s ? (str.compare(s) >= 0) : true; }
    friend bool operator <=(const String &str1, const String &str2)	{ return str1.compare(str2.getString()) <= 0; }

	friend bool operator >(const String &str, const char *s)			{ return s ? (str.compare(s) > 0) : true; }
	friend bool operator >(const char *s, const String &str)			{ return s ? (str.compare(s) < 0) : false; }
    friend bool operator >(const String &str1, const String &str2)	{ return str1.compare(str2.getString()) > 0; }

	friend bool operator >=(const String &str, const char *s)			{ return s ? (str.compare(s) >= 0) : true; }
	friend bool operator >=(const char *s, const String &str)			{ return s ? (str.compare(s) <= 0) : false; }
    friend bool operator >=(const String &str1, const String &str2)	{ return str1.compare(str2.getString()) >= 0; }
	
	static const String& getEmptyString() { return mygEmptyString; }

private:
	static const String mygEmptyString;
};

}
