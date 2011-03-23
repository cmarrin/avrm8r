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

#include "config.h"

namespace UC {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: BasePtr
//
//  Wrapper for a memory pointer
//
//////////////////////////////////////////////////////////////////////////////

#define NO_HEAP_ENTRY			255

class Block;

	class Memory  {
	public:
		Memory(uint16_t size=0)		{ myIndex = NO_HEAP_ENTRY; if (size) alloc(size); ref(); }
		Memory(const Memory& m)		{ myIndex = NO_HEAP_ENTRY; *this = m; }
		~Memory()					{ unref(); }
		
		Memory& operator=(const Memory& m)
		{
			if (myIndex == m.myIndex)
				return *this;
			if (myIndex != NO_HEAP_ENTRY)
				unref();
			myIndex = m.myIndex;
			ref();
			return *this;
		}
		
		void makeEmpty()
		{
			unref();
			myIndex = NO_HEAP_ENTRY;
		}
		
		static void init(char* heapStart, char* heapEnd)	{ mygHeapStart = heapStart; mygHeapEnd = heapEnd; }

		void*			get()					{ return (myIndex == NO_HEAP_ENTRY) ? NULL : mygHeapEntries[myIndex].ptr; }
		const void*		get() const				{ return (myIndex == NO_HEAP_ENTRY) ? NULL : mygHeapEntries[myIndex].ptr; }
		bool			resize(uint16_t size)	{ return resize(myIndex, size); }
		uint16_t		getSize() const			{ return getSize(myIndex); }
		
	protected:
		void			ref();
		void			unref();

	private:
		void			alloc(uint16_t size);
		static bool		resize(uint8_t index, uint16_t size);
		static uint16_t	getSize(uint8_t index);

		static char*		mygHeapStart;
		static char*		mygHeapEnd;
		static char*		mygHeapNext;

		struct MyHeapEntry { uint8_t ref; char* ptr; };
		
		static MyHeapEntry*	mygHeapEntries;
		static uint8_t		mygNumHeapEntries;

		uint8_t				myIndex;
	};

	template<class T>
	class Array {
	public:
		explicit Array(uint16_t size=0)
		 : myMemory(size*sizeof(T))		{ }
		Array(const Array& a)			{ myMemory = a.myMemory; }
		~Array()						{ }
		void resize(uint16_t size)		{ myMemory.resize(size*sizeof(T)); }
		uint16_t getSize() const		{ return myMemory.getSize()/sizeof(T); }
		
		const T&	operator [](int i) const	{ return *((const T*) (myMemory.get()) + i); }
		T&			operator [](int i)			{ return *((T*) (myMemory.get()) + i); }
		
	private:
		Memory myMemory;
	};

	template<class T>
	class Ptr {
	public:
		explicit Ptr()
		 : myMemory(sizeof(T))			{ }
		Ptr(const Ptr& o)				{ myMemory = o.myMemory; }
		~Ptr()							{ }
		T* operator->()					{ return (T*) myMemory.get(); }
		const T* operator->() const		{ return (const T*) myMemory.get(); }
		T& operator*()					{ return &((T*) myMemory.get()); }
		const T& operator*() const		{ return &((const T*) myMemory.get()); }

	private:
		Memory myMemory;
	};
};
