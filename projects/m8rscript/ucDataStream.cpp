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

//#include "FusionBasic.h"
#include "ucDataStream.h"

using namespace UC;

OrderedArray<String, IDataStreamHandlerFactory*> DataStream::mygProtocolHandlers;
OrderedArray<String, IDataStreamHandlerFactory*> DataStream::mygDeviceHandlers;

void DataStream::addProtocolHandler(const String& protocol, IDataStreamHandlerFactory* h)
{ mygProtocolHandlers.insert(protocol, h); }

void DataStream::removeProtocolHandler(const String& protocol)
{
	mygProtocolHandlers.removeKey(protocol);
	if (mygProtocolHandlers.getNum() == 0) {
		mygProtocolHandlers.clear();
	}
}

void DataStream::addDeviceHandler(const String& protocol, IDataStreamHandlerFactory* h)
{ mygDeviceHandlers.insert(protocol, h); }

void DataStream::removeDeviceHandler(const String& protocol)
{
	mygDeviceHandlers.removeKey(protocol);
	if (mygDeviceHandlers.getNum() == 0) {
		mygDeviceHandlers.clear();
	}
}

bool
DataStream::open(const String& fileName, const char* mode)
{
	close();

	myFileName = fileName;

	String path = getResolvedFileName();

	// check protocols
	if (path.getSubString(0, 6) == "device:") {
		// device: protocol. Format is:
		//
		//     device:[<mimetype>]<device string>
		//
		// lookup the device handler using mimetype. If not
		// found return an error.
		// there are 3 special devices: "@stdin", "@stdout" and "@stderr"
		// which simply associate this File object with one of the 3
		// stdio streams
		if (path.getSubString(7) == "@stdin") {
			myFile = stdin;
			return true;
		}
		else if (path.getSubString(7) == "@stdout") {
			myFile = stdout;
			return true;
		}
		else if (path.getSubString(7) == "@stderr") {
			myFile = stderr;
			return true;
		}

		if (path[7] != '[') {
			myErrorCode = BADDEV;
			return false;
		}

		int32_t end = path.last(']');
		if (end < 0) {
			myErrorCode = BADDEV;
			return false;
		}

		myMimeType = path.getSubString(8, end-1);

		int32_t dev = mygDeviceHandlers.find(myMimeType);
		if (dev < 0) {
			myErrorCode = DEVNOTSUP;
			myMimeType = "";
			return false;
		}

		myDeviceString = path.getSubString(end+1);
		myHandlerFactory = mygDeviceHandlers[dev];
		myHandler = myHandlerFactory->create();
		myIsHandlerAllocated = true;
		myIsDevice = true;
		char* mimetype = myHandler->open(getResolvedFileName().getString());
		if (mimetype != NULL) {
			setMimeType(mimetype);
			delete mimetype;
		}
		return mimetype != NULL;
	}

	// check data: protocol
	if (path.getSubString(0, 4) == "data:") {
		// get the mimetype
		int32_t i = path.first(';');
		if (i >= 0) {
			setMimeType(path.getSubString(5,i-1));
			i++;
		}
		else
			i = 5;
			
		int32_t j = path.first(',');
		int32_t k;
		if (j < 0)
			j = k = i;
		else {
			k = j+1;
			j--;
		}
			
		// between i and j is the data type, right now we only accept base64
		if (path.getSubString(i, j) != "base64") {
			myErrorCode = PROTNOTSUP;
			return false;
		}
		
		// now decode
		uint8_t* buf = new uint8_t[path.length()];
		int32_t size = decodeBase64(path.getString()+k, buf, path.length());
		if (size < 0) {
			myErrorCode = FAILED;
			return false;
		}
		
		return openBuffer(buf, size);
	}
	
	// check for protocols
	int32_t prot = path.first(':');
	if (prot >= 0) {
		String protocol = path.getSubString(0, prot-1);
		prot = mygProtocolHandlers.find(protocol);
		if (prot >= 0) {
			myProtocol = protocol;
			myHandlerFactory = mygProtocolHandlers[prot];
			myHandler = myHandlerFactory->create();
			myIsHandlerAllocated = true;
			char* mimetype = myHandler->open(getResolvedFileName().getString());
			if (mimetype != NULL ) {
				myErrorCode = OK;
				setMimeType(mimetype);
				if ( strlen(mimetype) > 0 )
				{
					delete mimetype;
				}
				return true;
			}
			else {
				myErrorCode = NOENT;
				return false;
			}
		}
	}

	errno = 0;
	if (getFileName().isEmpty()) {
		myFile = tmpfile();
	}
	else {
		myFile = fopen(String(getFileName()).getString(), mode);
	}

	myErrorCode = (ResultType) errno;

	if (myFile != NULL) {
		int32_t ext = fileName.last('.');
		if (ext >= 0)
			myExt = fileName.getSubString(ext+1);
		return true;
	}

	return false;
}

bool
DataStream::openBuffer(const uint8_t* buf, size_t size)
{
	close();
	myBufferSize = (uint32_t) size;
	myBuffer = new uint8_t[myBufferSize+1];
	myBufferAlloc = myBufferSize;
	memcpy(myBuffer, buf, size);
	myBufferPtr = myBuffer;
	return true;
}

bool
DataStream::openString()
{
	close();
	
	// start with a reasonable buffer
	myBufferAlloc = STRING_WRITE_SIZE;
	myBuffer = new uint8_t[myBufferAlloc+1];
	myBufferSize = 0;
	myBufferPtr = myBuffer;
	return true;
}

void
DataStream::close()
{
	flush();

	if (myHandler != NULL) {
		assert(myFile == NULL);
		assert(myBuffer == NULL);

		myHandler->close();
		if ( myIsHandlerAllocated && 
			// on shut down, this may be called after the protocol handlers are removed
			0 != mygProtocolHandlers.getNum() )
		{
			myHandlerFactory->destroy(myHandler);
			myHandler = NULL;
			myHandlerFactory = NULL;
		}
	}

	if (myFile != NULL) {
		assert(myBuffer == NULL);
		assert(!myIsDevice);

		if (myFile != stdin && myFile != stdout && myFile != stderr) {
			fclose(myFile);
		}
		myFile = NULL;
	}

	if (myBuffer != NULL) {
		assert(!myIsDevice);

		delete [] myBuffer;
		myBuffer = NULL;
	}

	myExt = "";
	myMimeType = "";
	myFileName = "";
	myOpenMode = "";
	myErrorCode = OK;
	myDeviceString = "";
	myIsDevice = false;
	myProtocol = "";
}

void
DataStream::flush()
{
	if (myFile != NULL) {
		fflush(myFile);
	}
}

bool
DataStream::read(double& v)
{
	// format is:
	//    [whitespace][+|-][0-9]*[.]{0-9}*[e|E[+|-]{0-9}*]
	String s;

	uint8_t c = getchar();

	while (isspace(c)) {
		c = getchar();
	}

	if (c == '-' || c == '+') {
		s.append(c);
		c = getchar();
	}

	while (isdigit(c)) {
		s.append(c);
		c = getchar();
	}

	if (c == '.') {
		s.append(c);
		c = getchar();
	}

	while (isdigit(c)) {
		s.append(c);
		c = getchar();
	}

	if (c == 'e' || c == 'E') {
		s.append(c);
		c = getchar();
	}

	if (c == '-' || c == '+') {
		s.append(c);
		c = getchar();
	}

	while (isdigit(c)) {
		s.append(c);
		c = getchar();
	}

	ungetchar(c);

	if (s.getLength() == 0)
		return false;

	uint32_t next;
	v = s.toDouble(next);
	return next == s.getLength();
}

bool
DataStream::readline(String& s, int32_t n)
{
	// Return a string that terminates either when BUFSIZ characters are read,
	// when a line feed is read, or when eof is reached. If line feed is seen,
	// it is returned with the string. Return true if the read 
	// is successful. Return false on error. If eof the return value will be
	// true and the string will be empty. 
	s = "";

	if (myLookahead >= 0) {
		s.append(myLookahead);
		myLookahead = -1;
		if (s[0] == '\n') {
			return true;
		}
	}

	if (myHandler != NULL) {
		char c;
		int32_t totalRead = 0;
		while ( totalRead < n )
		{
			if ( !myHandler->read(&c, 1) )
			{
				break;
			}

			s.append(c);
			totalRead++;
			if ( '\n' == c || totalRead >= BUFSIZ )
			{
				break;
			}
		}
		return ( totalRead > 0 );
	}

	if (myBuffer != NULL) {
		myErrorCode = NOTIMPLEMENTEDYET;
		return false;
	}

	if (myFile == NULL) {
		myErrorCode = BADF;
		return false;
	}

	if (n <= 0)
		n = BUFSIZ;

	if (n <= BUFSIZ) {
		char buf[BUFSIZ];
		if (fgets(buf, n, myFile) != NULL)
			s += buf;
	}
	else {
		char* buf = new char[n];
		if (fgets(buf, n, myFile) != NULL)
			s += buf;
		delete [ ] buf;
	}

	myErrorCode = (ResultType) ferror(myFile);
	clearerr(myFile);
	return myErrorCode == OK;
}

int32_t
DataStream::read(void* buf, uint32_t size)
{
	if (size == 0) {
		return 0;
	}

	if (myLookahead >= 0) {
		((char*) buf)[0] = myLookahead;
		size--;
		buf = ((char*) buf) + 1;
	}

	if (myHandler != NULL) {
		size = myHandler->read(buf, size);
	}
	else if (myBuffer != NULL) {
		uint32_t remain = myBufferSize-(myBufferPtr-myBuffer);
		if (size > remain)
			size = remain;
		if (size > 0) {
			memcpy(buf, myBufferPtr, size);
			myBufferPtr += size;
		}
	}
	else if (myFile != NULL) {
		errno = 0;
		size = (int32_t) fread(buf, 1, size, myFile);
		myErrorCode = (ResultType) errno;
		if (myErrorCode != 0) {
			return -1;
		}
	}
	else {
		myErrorCode = BADF;
		return -1;
	}

	if (myLookahead >= 0) {
		size++;
		myLookahead = -1;
	}

	return size;
}

int32_t
DataStream::write(const void* buf, uint32_t size)
{
	assert(myLookahead == -1);

	if (myHandler != NULL)
		return myHandler->write(buf, size);

	if (myBuffer != NULL) {
		// for now we can only append to the string
		size_t curlen = myBufferPtr - myBuffer;
		uint32_t remain = myBufferAlloc - curlen;
		if (size > remain) {
			// alloc some more
			uint32_t needed = STRING_WRITE_SIZE;
			if (size > remain+needed)
				needed = size-remain;
			
			myBufferAlloc += needed;
			uint8_t* newstr = new uint8_t[myBufferAlloc+1];
			memcpy(newstr, myBuffer, curlen);
			myBufferSize = (uint32_t) curlen;
			delete [ ] myBuffer;
			myBuffer = newstr;
			myBufferPtr = myBuffer + curlen;
		}
		
		memcpy(myBufferPtr, (char*) buf, size);
		myBufferPtr += size;
		*myBufferPtr = '\0';
		myBufferSize += size;
		myErrorCode = (ResultType) OK;
		return size;
	}

	if (myFile == NULL) {
		myErrorCode = BADF;
		return -1;
	}

	errno = 0;
	int32_t r = (int32_t) fwrite(buf, 1, size, myFile);
	myErrorCode = (ResultType) errno;
	return (myErrorCode != 0) ? -1 : r;
}

int32_t
DataStream::write(double d)
{
	char buf[50];
	return write(buf, sprintf(buf, "%f", d));
}

int32_t
DataStream::write(int32_t i)
{
	char buf[20];
	return write(buf, sprintf(buf, "%d", (int) i));
}

double
DataStream::seek(double n, SeekType t)
{
	// deal with lookahead
	if (myLookahead >= 0) {
		if (t == ST_CUR) {
			n--;
		}
		myLookahead = -1;
	}

	if (myHandler != NULL)
		return myHandler->seek(n, t);

	if (myBuffer != NULL) {
		switch(t) {
			case ST_SET:
				if (n > (int32_t) myBufferSize)
					n = myBufferSize;
				myBufferPtr = myBuffer + (int32_t) n;
				break;
			case ST_END:
				if ((int32_t) n > (int32_t) myBufferSize)
					n = (double) myBufferSize;
				myBufferPtr = myBuffer + (myBufferSize - (int32_t) n);
				break;
			case ST_CUR:
				{
					uint32_t remain = myBufferSize-(myBufferPtr-myBuffer);
					if ((int32_t) n > (int32_t) remain)
						n = (double) remain;
					myBufferPtr += (int32_t) n;
				}
				break;
		}
		return (double) (myBufferPtr-myBuffer);
	}

	if (myFile == NULL) {
		myErrorCode = BADF;
		return -1;
	}

	int origin = SEEK_CUR;
	switch(t) {
		case ST_SET: origin = SEEK_SET; break;
		case ST_END: origin = SEEK_END; break;
		case ST_CUR: origin = SEEK_CUR; break;
	}
	
	errno = 0;
	bool r = fseek(myFile, (long) n, origin) == 0;
	myErrorCode = (ResultType) (r ? 0 : errno);
	long pos = ftell(myFile);
	return (myErrorCode != 0) ? -1 : (double) pos;
}

String
DataStream::getErrorString(ResultType code)
{
	if (code == OK) {
		return "no error";
	}

	if (code < FAILED)
		return strerror(code);

	switch(code) {
		case FAILED:				return "failed";
		case BADPOINTER:			return "bad pointer";
		case BADARGUMENT:			return "bad argument";
		case NOTPROCESSED:			return "not processed";
		case NOTFOUND:				return "not found";
		case NOTENOUGHDATA:			return "not enough data";
		case FINISHED:				return "finished";
		case NOTIMPLEMENTEDYET:		return "not implemented yet";
		case NOTIMPLEMENTED:		return "not implemented";
		case NOCHANGE:				return "no change";
		case BADDEV:				return "device string syntax is invalid";
		case DEVNOTSUP:				return "mimetype for device not in list";
		case PROTNOTSUP:			return "protocol not in list";
		case BADFILENAME:			return "invalid filename";
		case MUSTBEWRITESTRING:		return "file must be a write string";
	}

	String s("unknown error code(");
	s += code;
	s += ")";
	return s;
}
