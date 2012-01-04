//
//  SevenSegmentDisplay.cpp
//
//  Created by Chris Marrin on 1/4/12.
//

/*
Copyright (c) 2009-2012 Chris Marrin (chris@marrin.com)
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    - Redistributions of source code must retain the above copyright notice, this 
      list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright notice, 
      this list of conditions and the following disclaimer in the documentation 
      and/or other materials provided with the distribution.

    - Neither the name of Marrinator nor the names of its contributors may be 
      used to endorse or promote products derived from this software without 
      specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
DAMAGE.
*/

#include "SevenSegmentDisplay.h"

#include <avr/pgmspace.h>

extern const uint8_t g_charTable[] PROGMEM;
const uint8_t g_charTable[] = {
    0x00, // 0x20	 
    0x00, // 0x21	! ** Not Supported **
    0x22, // 0x22	"
    0x00, // 0x23	# ** Not Supported **
    0x00, // 0x24	$ ** Not Supported **
    0x00, // 0x25	% ** Not Supported **
    0x00, // 0x26	& ** Not Supported **
    0x02, // 0x27	'
    0x39, // 0x28	(
    0x0F, // 0x29	)
    0x00, // 0x2A	* ** Not Supported **
    0x00, // 0x2B	+ ** Not Supported **
    0x04, // 0x2C	,
    0x40, // 0x2D	-
    0x00, // 0x2E	. ** Not Supported **
    0x00, // 0x2F	/ ** Not Supported **
    0x3f, // 0x30	0
    0x06, // 0x31	1
    0x5b, // 0x32	2
    0x4f, // 0x33	3
    0x66, // 0x34	4
    0x6d, // 0x35	5
    0x7d, // 0x36	6
    0x07, // 0x37	7
    0x7f, // 0x38	8
    0x6f, // 0x39	9
    0x00, // 0x3A	: ** Not Supported **
    0x00, // 0x3B	; ** Not Supported **
    0x27, // 0x3C	< Right half of M
    0x48, // 0x3D	=
    0x1E, // 0x3E	> Right half of W
    0x53, // 0x3F	?
    0x74, // 0x40	@ lower case h
    0x77, // 0x41	A
    0x7c, // 0x42	B
    0x39, // 0x43	C
    0x5e, // 0x44	D
    0x79, // 0x45	E
    0x71, // 0x46	F
    0x3d, // 0x47	G
    0x76, // 0x48	H
    0x04, // 0x49	I
    0x1E, // 0x4A	J
    0x72, // 0x4B	K
    0x38, // 0x4C	L
    0x33, // 0x4D	M
    0x54, // 0x4E	N
    0x5c, // 0x4F	O
    0x73, // 0x50	P
    0x67, // 0x51	Q
    0x50, // 0x52	R
    0x6d, // 0x53	S
    0x78, // 0x54	T
    0x3e, // 0x55	U
    0x1c, // 0x56	V
    0x3c, // 0x57	W
    0x64, // 0x58	X
    0x6e, // 0x59	Y
    0x5b, // 0x5A	Z
    0x39, // 0x5B	[
    0x00, // 0x5C	\ ** Not Supported **
    0x0F, // 0x5D	]
    0x00, // 0x5E	^ ** Not Supported **
    0x08, // 0x5F	_
};
using namespace m8r;

uint8_t
SevenSegmentDisplay::glyphForChar(uint8_t c)
{
    if (c < 0x20 || c > 0x7f)
        return 0;
        
    if (c > 0x5f)
        c -= 0x20;
        
    return pgm_read_byte_near(&(g_charTable[c-0x20]));
}
