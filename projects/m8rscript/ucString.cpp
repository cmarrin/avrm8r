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


#include <errno.h>
#include <cstdarg>

#include "config.h"
#include "ucString.h"
#include "ucVector.h"

using namespace UC;

const String String::mygEmptyString;

//extern bool _vformat(String& s, const char* fmt, va_list args);

#if 0
String::String(double num, const char *fmt)
{
	if (IS_NAN(num))
		*this = "NaN";
	else if (num == M_INF)
		*this = "Infinity";
	else if (num == -M_INF)
		*this = "-Infinity";
	else if (fmt == NULL)
		*this = format("%.12g", num);
	else 
		*this = format(fmt, num);
}

double
String::toDouble(uint32_t& next) const
{
	char* endptr;
	double num = strtod(getString(), &endptr);
	next = (uint32_t) (endptr-getString());
	if (errno == ERANGE || num == HUGE_VAL || num == -HUGE_VAL)
		num = M_NAN;

	return num;
}
#endif

int32_t
String::toInt32(uint32_t& next, uint32_t radix) const
{
	char* endptr;
	int32_t num = strtol(getString(), &endptr, radix);
	next = (uint32_t) (endptr-getString());
	return num;
}

uint32_t
String::toUInt32(uint32_t& next, uint32_t radix) const
{
	char* endptr;
	int32_t num = strtoul(getString(), &endptr, radix);
	next = (uint32_t) (endptr-getString());
	return num;
}

#if 0
bool
String::toLegalNumber(const char* s, double& num)
{
	char* endptr;
	double _num = strtod(s, &endptr);
	while(*endptr != '\0') {
		if (!::isspace(*endptr++))
			return false;
	}
	num = _num;
	return true;
}
#endif

bool
String::toLegalInt32(const char* s, int32_t& num)
{
	char* endptr;
	int32_t _num = (int32_t) strtol(s, &endptr, 0);
	while(*endptr != '\0') {
		if (!::isspace(*endptr++))
			return false;
	}
	num = _num;
	return true;
}

String
String::getSubString(int start, int end) const
{
	// if params are negative, it means "from the end - 1".
	// The reason for the -1 is so that sending -1 means "the
	// last char".
	int		l = getLength();

	// negative numbers mean "from the end". Where -1 means the
	// end of the string (one past the last char)
	if (start < 0) {
		start += l+1;
		if (start < 0)
			start = 0;
	}
	else if (start > l)
		start = l;

	if (end < 0) {
		end += l;
		if (end < 0)
			end = -1;
	}
	else if (end > l)
		end = l;

	// At this point start should be less than or equal to end, positive and
	// indexing into the string for the substr call below.  If not, return the
	// empty string.
	if ( end < start ) {
		return String();
	}
	
	return String(substr(start, end-start+1).data());
}
//
// Deletes the specified characters from the string. If endChar is
// -1 (the default), all characters from startChar until the end
// are deleted.
//

#if 0
String 
String::format(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	String s = vformat(fmt, args);
	va_end(args);
	return s;
}

String
String::vformat(const char* fmt, va_list args)
{
	String s;
	_vformat(s, fmt, args);
	return s;
}
#endif

bool
String::isspace(uint32_t index) const
{
	return ::isspace((*this)[index]) != 0;
}

String
String::tolower() const
{
	char* s = strdup(getString());
	for (char* p = s; *p != 0; ++p)
		*p = ::tolower(*p);

	String us(s);
	free(s);
	return us;
}

String
String::toupper() const
{
	char* s = strdup(getString());
	for (char* p = s; *p != 0; ++p)
		*p = ::toupper(*p);

	String us(s);
	free(s);
	return us;
}

int32_t
String::first(const String& str) const
{
	const char* found = ::strstr(getString(), str.getString());
	return found ? ((int32_t) (found-getString())) : -1;
}

int32_t
String::first(int c) const
{
	if (c == 0)
		return -1;

	const char* found = strchr(getString(), c);
	return found ? ((int32_t) (found-getString())) : -1;
}

int32_t
String::last(int c) const
{
	if (c == 0)
		return -1;

	const char* found = strrchr(getString(), c);
	return found ? ((int32_t) (found-getString())) : -1;
}

String
String::join(const Vector<String>& array, const String& sep)
{
	// create a new string which is each element of array separated by sep
	if (array.getNum() == 0)
		return String();

	String s = array[0];
	for (uint32_t i = 1; i < array.getNum(); ++i) {
		s += sep;
		s += array[i];
	}

	return s;
}

const Vector<String>&
String::split(const String& sep, Vector<String>& array) const
{
	// split this by sep and place elements into array
	String rem(*this);
	for ( ; ; ) {
		int32_t next = rem.first(sep);
		if (next < 0) {
			array.append(rem);
			return array;
		}

		if (next == 0)
			array.append(String());
		else
			array.append(rem.getSubString(0, next-1));
		rem = rem.getSubString(next+sep.getLength());
	}

	return array;
}

String
String::replace(const String& from, const String& to, int32_t first, int32_t last)
{
	if (first < 0)
		first = 0;

	if (last < 0)
		last = getLength();

	String s;

	while (1) {
		const char* fnd = strstr(getString()+first, from.getString());
		if (fnd == NULL || fnd - getString() > last)
			break;

		s += getSubString(first, ((int32_t) (fnd - getString())) - 1);
		s += to;
		first = ((int32_t) (fnd - getString())) + from.getLength();
	}

	s += getSubString(first);
	return s;
}
