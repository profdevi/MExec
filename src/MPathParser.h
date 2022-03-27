/*

Copyright (C) 2011-2022, Comine.com ( comine.com@gmail.com )
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
- The the names of the contributors of this project may not be used to 
  endorse or promote products derived from this software without specific 
  prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
`AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

//v1.10 copyright Comine.com 20160718M0708
#ifndef MPathParser_h
#define MPathParser_h

/////////////////////////////////////////////////////
#include "MBuffer.h"

//******************************************************
//**  MPathParser class
//******************************************************
class MPathParser
	{
	////////////////////////////////////////////////
	MBuffer mBuffer;			// Memory buffer to hold all data
	char *mDrive;				// Holds the Drive Letter
	char *mDir;					// Holds the Directory Name
	char *mFileName;			// Holds the FileName
	char *mFileExt;				// Holds the file ext
	char *mPath;				// Holds the Path
	char *mFullFileName;		// Holds Full File name
	
	////////////////////////////////////////////////
	void ClearObject(void);
	void MakePathNames(void);	// Make a pathname

	////////////////////////////////////////////////
	public:
	MPathParser(bool init=false);
	~MPathParser(void);
	bool Create(void);
	bool Destroy(void);
	bool SetDrive(const char *drive);					// Change Drive
	bool SetDir(const char *dir);						// Set the Directory
	bool SetDriveDir(const char *dir);					// Set the Dir Path with drive
	bool SetFileName(const char *filename);				// Set the Filename
	bool SetFileExt(const char *extension);				// Set the File Extension
	bool SetPath(const char *path);						// Set the File path
	bool SetFullFileName(const char *filename);			// Set Filename as "One.txt";
	const char *GetDrive(void);							// Get Drive
	const char *GetDir(void);							// Get File Dir
	const char *GetFileName(void);						// Get File Name
	const char *GetFileExt(void);						// Get File Extension
	const char *GetPath(void);							// Get File Path
	const char *GetFullFileName(void);					// Get Full File name(ie "one.txt")
	bool Clear(void);									// Clear all the elements
	bool SetFileNameInExeHome(const char *filename);	// "help.hlp"
	};

#endif // MPathParser_h

