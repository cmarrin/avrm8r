
#include "ucMemory.h"

using namespace UC;

char*					Memory::mygHeapStart;
char*					Memory::mygHeapEnd;
char*					Memory::mygHeapNext;

Memory::MyHeapEntry*	Memory::mygHeapEntries;
uint8_t					Memory::mygNumHeapEntries;

#define NUM_HEAP_ENTRIES_INIT	10
#define NUM_HEAP_ENTRIES_GROW	10

void
Memory::alloc(uint16_t size)
{
	if (!mygHeapEntries) {
		// initial alloc
		mygHeapEntries = (MyHeapEntry*) mygHeapStart;
		mygNumHeapEntries = NUM_HEAP_ENTRIES_INIT+1;
		mygHeapNext = mygHeapStart + sizeof(MyHeapEntry) * mygNumHeapEntries;
		
		// first entry is the HeapEntry list itself
		mygHeapEntries[0].ref = 1;
		mygHeapEntries[0].ptr = (char*) mygHeapEntries;
		mygHeapEntries[1].ref = NO_HEAP_ENTRY;
	}
	
	myIndex = NO_HEAP_ENTRY;
	for (uint8_t i = 1; i < mygNumHeapEntries; ++i)
		if (mygHeapEntries[i].ref == 0 || mygHeapEntries[i].ref == NO_HEAP_ENTRY) {
			myIndex = i;
			break;
		}
		
	if (myIndex == NO_HEAP_ENTRY) {
		// extend the entry table
		if (!resize(0, sizeof(MyHeapEntry) * (mygNumHeapEntries + NUM_HEAP_ENTRIES_GROW)))
			return;
			
		mygHeapEntries[mygNumHeapEntries].ref = NO_HEAP_ENTRY;
		myIndex = mygNumHeapEntries;
		mygNumHeapEntries += NUM_HEAP_ENTRIES_GROW;
	}
	
	if (mygHeapEntries[myIndex].ref == 0) {
		// there is free memory here. resize to fit
		if (!resize(myIndex, size)) {
			myIndex = NO_HEAP_ENTRY;
			return;
		}
	}
	else {
		// we are at a new index, extend the heap for it
		if (mygHeapNext+size > mygHeapEnd) {
			myIndex = NO_HEAP_ENTRY;
			return;
		}
		mygHeapEntries[myIndex].ptr = mygHeapNext;
		mygHeapNext += size;
	}
}

bool
Memory::resize(uint8_t index, uint16_t size)
{
	if (!mygHeapEntries || index == NO_HEAP_ENTRY)
		return false;
		
	if (size == 0) {
		index = NO_HEAP_ENTRY;
		return true;
	}
	
	uint16_t curSize = getSize(index);
	if (curSize > size) {
		// grow
		uint16_t delta = curSize - size;
		if (mygHeapNext+delta > mygHeapEnd)
			return false;
			
		if (index != mygNumHeapEntries && mygHeapEntries[index+1].ref != NO_HEAP_ENTRY) {
			// we have to move
			memmove(mygHeapEntries[index+1].ptr+delta, mygHeapEntries[index+1].ptr,
					mygHeapEnd - mygHeapEntries[index+1].ptr);
			for (uint8_t i = index+1; i < mygNumHeapEntries; ) {
				mygHeapEntries[i++].ptr += delta;
				if (mygHeapEntries[i].ref == NO_HEAP_ENTRY)
					break;
			}
		}
		mygHeapNext += delta;
	}
	else if (curSize < size) {
		// shrink
		uint16_t delta = size - curSize;
			
		if (index != mygNumHeapEntries && mygHeapEntries[index+1].ref != NO_HEAP_ENTRY) {
			// we have to move
			memmove(mygHeapEntries[index+1].ptr-delta, mygHeapEntries[index+1].ptr,
					mygHeapEnd - mygHeapEntries[index+1].ptr);
			for (uint8_t i = index+1; i < mygNumHeapEntries; ) {
				mygHeapEntries[i++].ptr -= delta;
				if (mygHeapEntries[i].ref == NO_HEAP_ENTRY)
					break;
			}
		}
		mygHeapNext -= delta;
	}
	
	return true;
}

void
Memory::ref()
{
	if (!mygHeapEntries || myIndex == NO_HEAP_ENTRY)
		return;
	
	assert(mygHeapEntries[myIndex].ref < 255);
	mygHeapEntries[myIndex].ref++;
}

void
Memory::unref()
{
	if (!mygHeapEntries || myIndex == NO_HEAP_ENTRY)
		return;
	
	assert(mygHeapEntries[myIndex].ref > 0);
	mygHeapEntries[myIndex].ref--;
	
#ifdef DEBUG
	uint16_t size = getSize();
	memset(0, size);
#endif
}

uint16_t
Memory::getSize(uint8_t index)
{
	if (!mygHeapEntries || index == NO_HEAP_ENTRY)
		return 0;
		
	if (index+1 == mygNumHeapEntries || mygHeapEntries[index+1].ref == NO_HEAP_ENTRY)
		return mygHeapNext - mygHeapEntries[index].ptr;
	
	return mygHeapEntries[index+1].ptr - mygHeapEntries[index].ptr;
}
