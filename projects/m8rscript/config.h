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

/** This is the minimum file to use Fusion
	This file should be included by all files that will use 
	or implement Fusion's interfaces
*/

#define FIXED_POINT_FLOAT

namespace UC {
	// Define Fusion version
    #define UC_VERSION_MAJOR 0
    #define UC_VERSION_MINOR 6
    #define UC_VERSION_NAME "MaruRoku"
} // end namespace

#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef FIXED_POINT_FLOAT
#define FPF int32_t
#define FPF_ADD(a,b)	(a+b)
#define FPF_SUB(a,b)	(a-b)
#define FPF_MUL(a,b)	(a*b/1000)
#define FPF_DIV(a,b)	(a*1000/b)
inline FPF FPF_MAKE(uint32_t i, uint32_t f, uint8_t dp, uint32_t e)
{
	int32_t num = i*1000;
	while (dp < 3) {
		++dp;
		f *= 10;
	}
	while (dp > 3) {
		--dp;
		f /= 10;
	}
	num += f;
	while (e > 0) {
		--e;
		num *= 10;
	}
	while (e < 0) {
		++e;
		num /= 10;
	}
	return num;
}

#define FPF_TO_INT(f)	(f/1000)
#define INT_TO_FPF(i)	(i*1000)

#else

#define FPF float
#define FPF_ADD(a,b)	(a+b)
#define FPF_SUB(a,b)	(a-b)
#define FPF_MUL(a,b)	(a*b)
#define FPF_DIV(a,b)	(a/b)
inline FPF FPF_MAKE(uint32_t i, uint32_t f, uint8_t dp, uint32_t e)
{
	float num = (float) f;
	while (dp-- > 0)
		num /= 10;
	num += (float) i;
	while (e > 0) {
		--e;
		num *= 10;
	}
	while (e < 0) {
		++e;
		num /= 10;
	}
	return num;
}

#define FPF_TO_INT(f)	((int32_t) f)
#define INT_TO_FPF(i)	((float) i)
#endif
