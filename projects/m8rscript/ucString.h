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
    //String(double num, const char *format = NULL);

	~String() { }

    int				getLength() const	{ return (int)size(); }
    const char*		getString() const	{ return data(); }

	bool			isEmpty() const		{ return getString()[0] == '\0'; }

    operator const char *() const		{ return getString(); }
    //operator double() const				{ return toDouble(); }
    operator int32_t() const			{ return toInt32(); }
    operator uint32_t() const			{ return toUInt32(); }

	//double		toDouble() const
	//{ uint32_t next; return toDouble(next); }

	//double		toDouble(uint32_t& next) const;

	int32_t		toInt32(uint32_t radix = 0) const
	{ uint32_t next; return toInt32(next, radix); }

	int32_t		toInt32(uint32_t& next, uint32_t radix = 0) const;

	uint32_t	toUInt32(uint32_t radix = 0) const
	{ uint32_t next; return toUInt32(next, radix); }

	uint32_t	toUInt32(uint32_t& next, uint32_t radix = 0) const;

	//static bool toLegalNumber(const char* s, double& num);
	static bool toLegalInt32(const char* s, int32_t& num);

	String &		operator =(const String& str)
	{
		using namespace std;
		basic_string<char>::operator =(str);
		return *this;
	}

	String &		operator =(const std::basic_string<char>& str)
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

    //String &		operator =(double d)
	//{
	//	String s(d);
	//	*this = s;
	//	return *this;
	//}

    String &		operator +=(const String& str)
	{
		using namespace std;
		basic_string<char>::operator +=(str);
		return *this;
	}

    String &		operator +=(const char* str)
	{
		using namespace std;
		basic_string<char>::operator +=(str);
		return *this;
	}

	String&			append(char c)
	{
		using namespace std;
		basic_string<char>::operator +=(c);
		return *this;
	}

    char	operator [](int i) const
    {
		using namespace std;
		return basic_string<char>::operator [](i);
	}

    char	operator [](uint32_t i) const
    {
		using namespace std;
		return basic_string<char>::operator [](i);
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

    // Deletes the characters from startChar to endChar, inclusive,
    // from the string. If endChar is -1 (the default), all characters
    // from startChar until the end are deleted.
    void		deleteSubString(int startChar, int endChar = -1)
	{
		std::basic_string<char>::replace(startChar, endChar, "");
	}
	
	// replace the char at the given location with the passed char
	void replace(int i, uint8_t c) { std::basic_string<char>::replace(i, 1, (const char*) &c, 1); }

	// replace all occurances of 'from' to 'to' within given range
	String	replace(const String& from, const String& to, int32_t first = -1, int32_t last = -1);

    // Returns new string representing sub-string from startChar to
    // endChar, inclusive. If endChar is -1 (the default), the
    // sub-string from startChar until the end is returned.
    String	getSubString(int startChar, int endChar = -1) const;

	// perform simple filename resolution
    //String	resolveFileName(const String& fileBase) const;
	//String	getFileBase() const;
	//String	getFileTail() const;	

	bool		isspace(uint32_t index) const;
	String		tolower() const;
	String		toupper() const;
	int32_t		first(const String& str) const;
	int32_t		first(int c) const;
	int32_t		last(int c) const;

	// utility functions to do Javascript String-like functions
	static String			join(const Vector<String>& array, const String& sep=String(","));
	const Vector<String>&	split(const String& sep, Vector<String>& array) const;

	//static String format(const char* fmt, ...);
	//static String vformat(const char* fmt, va_list args);
	
	static const String& getEmptyString()	{ return mygEmptyString; }
	
private:
	static const String mygEmptyString;
};

}
