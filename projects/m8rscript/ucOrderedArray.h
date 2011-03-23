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

#include "ucVector.h"

namespace UC {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: OrderedArray
//
//////////////////////////////////////////////////////////////////////////////

template <class key, class val> class OrderedArray {
public:
	typedef struct {
		key k;
		val v;
	} Item;

    OrderedArray(uint32_t initialSize = 0) : myArray(initialSize) { }

    OrderedArray(const OrderedArray<key,val>& t)	{ *this = t; }

    ~OrderedArray() { }

    uint32_t getNum() const						{ return myArray.getNum(); }

    int32_t    insert(const key& k)
	{
		// find where this key goes
		int32_t i;
		if (!_find(k, i, true)) 
			myArray.insert(i);
		myArray[i].k = k;
		return i;
	}

    int32_t    insert(const key& k, const val& v)
	{
		int32_t i = insert(k);
		if (i < 0)
			return -1;
		myArray[i].v = v;
		return i;
	}
	void	clear()		{ myArray.clear(); }
    void    removeIndex(uint32_t i, uint32_t n=1)	{ myArray.removeIndex(i,n); }
    void    removeKey(const key& k)
	{
		int32_t i;
		if (_find(k, i, false))
			removeIndex(i);
	}

    OrderedArray<key,val>& operator =(const OrderedArray<key,val>& t)
    {
		myArray = t.myArray;
		return *this;
    }

    // Return a value with the given index
    val&   operator [](uint32_t i)				{ assert(i<getNum()); return myArray[i].v; }
    const val&	operator [](uint32_t i) const	{ assert(i<getNum()); return myArray[i].v; }
    const key& getKey(uint32_t i) const			{ assert(i<getNum()); return myArray[i].k; }

    int32_t find(const key& _val) const
    {
		int32_t i;
		return _find(_val, i, false) ? i : -1;
    }

private:
	bool _find(const key& k, int32_t& i, bool findInsertPoint) const
	{
		// find where this key goes. Return true if found, false if not.
		// If found, i contains the index, if not i contains the index
		// of the key just after k (i == getNum() if k > the last key)
		if (!findInsertPoint) {
			// optimize for small arrays
			switch(getNum()) {
				case 6: if (k == myArray[5].k) { i = 5; return true; }
				case 5: if (k == myArray[4].k) { i = 4; return true; }
				case 4: if (k == myArray[3].k) { i = 3; return true; }
				case 3: if (k == myArray[2].k) { i = 2; return true; }
				case 2: if (k == myArray[1].k) { i = 1; return true; }
				case 1: if (k == myArray[0].k) { i = 0; return true; }
				case 0: i = 0; return false;
			}
		}

		uint32_t num = getNum();
		if (num == 0) {
			i = 0;
			return false;
		}

		const Item* lo = &(myArray[0]);
		const Item* hi = &(myArray[num - 1]);
		const Item* mid;
		uint32_t half;
		const Item* result = NULL;
		bool found;

		while (1) {
			if (half = num / 2)
			{
				mid = lo + (num & 1 ? half : (half - 1));
				if (k == mid->k) {
					result = mid;
					found = true;
					break;
				}
				else if (k < mid->k) {
					hi = mid - 1;
					num = (num & 1) ? half : half-1;
				}
				else {
					lo = mid + 1;
					num = half;
				}
			}
			else {
				result = lo;
				if (k == lo->k)
					found = true;
				else {
					found = false;
					if (k > lo->k)
						result++;
				}

				break;
			}
		}

		i = (int32_t)(result - &(myArray[0]));
		return found;
	}

	Vector<Item>	myArray;
};

}
