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

#include <errno.h>
#include "ucString.h"
#include "ucOrderedArray.h"

#define STRING_WRITE_SIZE 1024

extern "C" {
extern size_t b64_pton(const char* src, unsigned char* target, size_t targsize);
}

namespace UC {
	//////////////////////////////////////////////////////////////////////////////
	//
	//  Class: DataStream
	//
	//  Common DataStream class
	//
	//////////////////////////////////////////////////////////////////////////////

	class IDataStreamHandler;
	class IDataStreamHandlerFactory;

	class DataStream {
	public:
		DataStream() :
			myFile( NULL),
			myErrorCode(OK),
			myBuffer(NULL),
			myHandler(NULL),
			myHandlerFactory(NULL),
			myIsHandlerAllocated(false),
			myIsDevice(false),
			myBufferSize(0),
			myBufferAlloc(0),
			myBufferPtr(NULL),
			myLookahead(-1)
		{
		}
		virtual ~DataStream()
		{ 
			close(); 
		}
		
		enum SeekType { ST_SET, ST_END, ST_CUR };

	/** Result enumeration - used by methods to return information.
	/** Codes from 1-1000 match their Unix counterpart errno codes.
	/** Codes starting at 1001 are custom error codes for Fusion.
	*/
	enum ResultType {	OK=0, 
						FAILED=1001,			// This MUST be the lowest custom error code
						BADPOINTER=1002, 
						BADARGUMENT=1003, 
						NOTPROCESSED=1004, 
						NOTFOUND=1005, 
						NOTENOUGHDATA=1006, 
						FINISHED=1007, 
						NOTIMPLEMENTEDYET=1008,
						NOTIMPLEMENTED=1009,
						NOCHANGE=1010,

						// these are device related error codes returned by DataStream
						BADDEV=1011,				// device string syntax is invalid
						DEVNOTSUP=1012,				// mimetype for device not in list
						PROTNOTSUP=1013,			// protocol not in list
						BADFILENAME=1014,			// invalid filename
						MUSTBEWRITESTRING=1015,		// tried to reopen a file that was not a write string

						// these are standard errno error codes. They have the same error
						// codes as in errno.h for simplicity. Therefore all the Fusion
						// errors start above this.
						PERM           = 1,
						NOENT          = 2,
						SRCH           = 3,
						INTR           = 4,
						IO             = 5,
						NXIO           = 6,
						TOOBIG         = 7,
						NOEXEC         = 8,
						BADF           = 9,
						CHILD          = 10,
						AGAIN          = 11,
						NOMEM          = 12,
						ACCES          = 13,
						FAULT          = 14,
						BUSY           = 16,
						EXIST          = 17,
						XDEV           = 18,
						_NODEV         = 19,
						NOTDIR         = 20,
						ISDIR          = 21,
						INVAL          = 22,
						NFILE          = 23,
						MFILE          = 24,
						NOTTY          = 25,
						FBIG           = 27,
						NOSPC          = 28,
						SPIPE          = 29,
						ROFS           = 30,
						MLINK          = 31,
						PIPE           = 32,
						DOM            = 33,
						RANGE          = 34,
						DEADLK         = 36,
						NAMETOOLONG    = 38,
						NOLCK          = 39,
						NOSYS          = 40,
						NOTEMPTY       = 41,
						ILSEQ          = 42
					};
					
		bool	open(const String& fileName, const String& mode)
		{ return open(fileName, mode.getString()); }

		bool		open(const String& fileName, const char* mode = "rb");
		bool		openString(const String& s)			{ return openBuffer((const uint8_t*) s.getString(), s.length()); }
		bool		openBuffer(const uint8_t* buf, size_t size);
		bool		openString();
		void		close();

		double	seek(double n=0, SeekType t=ST_CUR);

		void		flush();

		int32_t	getchar()
		{
			uint8_t c;
			int32_t size = read(&c, 1);
			if (size == 0) {
				return EOF;
			}
			if (size < 0) {
				return -1;
			}

			return (int32_t) c;
		}

		void		ungetchar(int32_t c)
		{
			assert(myLookahead == -1);
			myLookahead = c;
		}

		bool		read(double& v);
		int32_t	read(void* buf, uint32_t size);
		bool		readline(String& s, int32_t n);
		int32_t	write(const void* buf, uint32_t size);
		int32_t	write(const char* buf)			{ return (int32_t)write((void*) buf, (uint32_t)strlen(buf)); }
		int32_t	write(double d);
		int32_t	write(int32_t i);
		bool		setvbuf(char* buf, int mode, size_t size)
		{
			if (myFile != NULL) {
				return ::setvbuf(myFile, buf, mode, size) == 0;
			}
			return false;
		}

		ResultType		getErrorCode()						{ return myErrorCode; }
		static String		getErrorString(ResultType code);

		const String&		getExtension() const				{ return myExt; }
		const String&		getMimeType() const					{ return myMimeType; }
		void				setMimeType(const String& mimetype)	{ myMimeType = mimetype; }
		const String&		getDeviceString() const				{ return myDeviceString; }
		bool				isDevice() const					{ return myIsDevice; }
		const String&		getProtocol() const					{ return myProtocol; }
		bool				isOpen() const
		{
			return myFile || myBuffer || myHandler;
		}

		void				setFileBase(const String& f)		{ myFileBase = f; }
		const String&		getFileName() const					{ return myFileName; }
		const String&		getFileBase() const					{ return myFileBase; }
		const String&		getOpenMode() const					{ return myOpenMode; }
		String			getResolvedFileName() const
		{ return myFileName.resolveFileName(myFileBase); }

		static void addProtocolHandler(const String& protocol, IDataStreamHandlerFactory* h);
		static void removeProtocolHandler(const String& protocol);
		static void addDeviceHandler(const String& protocol, IDataStreamHandlerFactory* h);
		static void removeDeviceHandler(const String& protocol);
		
		const uint8_t*	getBuffer()			{ return myBuffer; }

	protected:
		static int32_t				decodeBase64(const char* s, uint8_t* buf, size_t bufsize)
		{
			return (int32_t) b64_pton(s, buf, bufsize);
		}

	private:
		FILE*		myFile;
		ResultType	myErrorCode;
		String		myExt;
		String		myMimeType;
		String		myDeviceString;
		bool		myIsDevice;
		String		myProtocol;

		String		myFileBase;
		String		myFileName;
		String		myOpenMode;

		uint8_t*	myBuffer;
		uint8_t*	myBufferPtr;
		uint32_t	myBufferSize, myBufferAlloc;
		int32_t		myLookahead;

		IDataStreamHandler*			myHandler;
		IDataStreamHandlerFactory*	myHandlerFactory;
		bool						myIsHandlerAllocated;

		static OrderedArray<String, IDataStreamHandlerFactory*> mygProtocolHandlers;
		static OrderedArray<String, IDataStreamHandlerFactory*> mygDeviceHandlers;
	};

	//////////////////////////////////////////////////////////////////////////////
	//
	//  Class: IDataStreamHandler
	//
	//  Interface for writing handlers for unsupported stream types.
	//
	//////////////////////////////////////////////////////////////////////////////

	class IDataStreamHandler {
	public:
		virtual char*	open(const char* filename) = 0;
		virtual bool	close() = 0;
		virtual double	seek(double n=0, DataStream::SeekType t=DataStream::ST_CUR) = 0;
		virtual int32_t	read(void* buf, uint32_t size) = 0;
		virtual int32_t	write(const void* buf, uint32_t size) = 0;
	};

	//////////////////////////////////////////////////////////////////////////////
	//
	//  Class: IDataStreamHandlerFactory
	//
	//  Factory interface for returning IDataStreamHandler class instances.
	//
	//////////////////////////////////////////////////////////////////////////////

	class IDataStreamHandlerFactory {
	public:
		virtual IDataStreamHandler* create() = 0;
		virtual void destroy(IDataStreamHandler*) = 0;
	};
	
}

