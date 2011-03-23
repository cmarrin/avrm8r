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

#include <vector>

namespace UC {

	//////////////////////////////////////////////////////////////////////////////
	//
	//  Class: Vector
	//
	//////////////////////////////////////////////////////////////////////////////

	template <class type> class Vector : public std::vector<type> {
	public:
		Vector(uint32_t initialSize = 0)
		{ Vector::reserve(initialSize); }

		Vector(const Vector<type>& t)	{ *this = t; }

		~Vector() { }

		uint32_t getNum() const						{ return (uint32_t)Vector::size(); }
		void    setNum(uint32_t n)					{ Vector::resize(n); }
		type&	append()
		{
			size_t i = std::vector<type>::size();
			std::vector<type>::resize( i + 1 );
			return std::vector<type>::operator [](i);
		}
		type&	append(const type& v)
		{
			int32_t i = (int32_t)std::vector<type>::size();
			std::vector<type>::insert(Vector::end(), v);
			return std::vector<type>::operator [](i);
		}

		// This is really 'make sure index i is allocated
		// There is no vector::insert(i)
		void    insert(uint32_t i)
		{
			std::vector<type>::insert(Vector::begin()+i, type());
		}
		void    insert(uint32_t i, const type& v)
		{
			std::vector<type>::insert(Vector::begin()+i, v);
		}
		void	clear()
		{
			std::vector<type>::clear();
		}
		void    removeIndex(uint32_t i, uint32_t n=1)
		{
			std::vector<type>::erase(Vector::begin()+i, Vector::begin()+i+n);
		}
		void	removeItem(type& v)
		{
			int32_t i = find(v);
			if (i >= 0)
				removeIndex(i);
		}

		void    insert(uint32_t i, const Vector<type>& v)
		{
			for (uint32_t n = getNum(); n > 0; ++i, --n)
				insert(i, v[i]);
		}

		Vector<type>& operator =(const Vector<type>& t)
		{
			std::vector<type>::operator =(t);
			return *this;
		}

		// Return a value with the given index
		type&   operator [](uint32_t i)
		{
			return std::vector<type>::operator [](i);
		}

		const type&	operator [](uint32_t i) const
		{
			return std::vector<type>::operator [](i);
		}

		// functions to perform Javascript-like array ops
		int32_t find(const type& val) const
		{
			for (uint32_t i = 0; i < getNum(); ++i) {
				if ((*this)[i] == val)
					return (int32_t) i;
			}
			return -1;
		}

	};

}
