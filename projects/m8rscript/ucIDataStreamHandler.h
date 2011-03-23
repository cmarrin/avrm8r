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
#include "ucDataStream.h"

namespace UC {

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
		virtual double	seek(double n=0, DataStream::SeekType t=ST_CUR) = 0;
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

