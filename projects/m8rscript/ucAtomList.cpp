
#include "ucAtomList.h"

using namespace UC;

#define SIZE_ATOMLIST_INIT	50
#define SIZE_ATOMLIST_GROW	50



/*
		void copyIn(T* p, uint16_t n, uint16_t offset=0)		{ memmove(myMemory.get()+offset, p, n); }
		void copyOut(T* p, uint16_t n, uint16_t offset=0)		{ memmove(p, myMemory.get()+offset, n); }
		int8_t compare(T* p, uint16_t n, uint16_t offset=0)		{ return (int8_t) memcmp(p, myMemory.get()+offset, n); }
		
		int8_t compare(const Ptr<T>& o, uint16_t n, uint16_t offset=0, uint16_t offseto=0)
		{
			return (int8_t) memcmp(myMemory.get()+offset, o.myMemory.get()+offseto, n);
		}
		
		uint16_t findByte(uint8_t b, uint16_t offset)
		{
			void* start = myMemory.get()+offset;
			void* p = memchr(start, b, getSize()-offset);
			return p ? ((char*) p- (char*) start) : NO_HEAP_ENTRY;
		}
		
*/






uint16_t
AtomList::add(const char* s, uint16_t n)
{
	if (!s)
		return NO_ATOM;
		
	if (n == 0)
		n = strlen(s);
		
	// make space for terminating '\0'
	n++;
	
	uint16_t totSize = 0;
	
	if (myAtoms.getSize() == 0) {
		// initial alloc
		totSize = (n > SIZE_ATOMLIST_INIT) ? n : SIZE_ATOMLIST_INIT;
		myAtoms.resize(totSize);
		myCurSize = 0;
	}
	else
		totSize = myAtoms.getSize();
	
	if (myCurSize + n > totSize) {
		// resize
		totSize += (n > SIZE_ATOMLIST_GROW) ? n : SIZE_ATOMLIST_GROW;
		myAtoms.resize(totSize);
	}
	
	// store
	uint16_t ret = myCurSize;
	memmove(&(myAtoms[myCurSize]), s, n-1);
	memmove(&(myAtoms[myCurSize+n-1]), "", 1);
	myCurSize += n;
	return ret;
}

uint16_t
AtomList::getSize(uint16_t atom) const
{
	const char* start = &(myAtoms[atom]);
	const char* p = (char*) memchr(start, '\0', myAtoms.getSize()-atom);
	return p ? (p - start) : 0;
}

int8_t
AtomList::compare(uint16_t atom, const char* s, uint16_t n) const
{
	if (!s)
		return (atom == NO_ATOM) ? 0 : 1;
	
	if (n == 0)
		n = strlen(s);
		
	uint16_t selfLen = getSize(atom);
	if (selfLen != n)
		return selfLen > n;
	
	int8_t cmp = (int8_t) memcmp(&(myAtoms[atom]), s, n);
	return (cmp == 0)  ? 0 : ((cmp > 0) ? 1 : -1);
}
