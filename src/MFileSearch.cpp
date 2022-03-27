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

//v1.6 copyright Comine.com 20191114R1052
#include "MStdLib.h"
#include "MString.h"
#include "MStringList.h"
#include "MFileOps.h"
#include "MPathParser.h"
#include "MDynamicString.h"
#include "MProcEnvironTable.h"
#include "MFileSearch.h"


//******************************************************
//**  Module Elements
//******************************************************
static bool GCleanDirName(const char *dir,MStdArray<char> &dirclean);
	
////////////////////////////////////////////////
static bool GCleanDirName(const char* dir, MStdArray<char>& dirclean)
	{
	MStdAssert(dir!=0 && *dir!=0);
	const int length=MStdStrLen(dir);
	if(dirclean.Create(length+2)==false)
		{
		return false;
		}

	// Strip Double Quotes and single quotes
	int itarget;
	itarget=0;
	for(int i=0; ;++i)
		{
		dirclean[itarget]=dir[i];
		if(dir[i]==0) { break; }
		if(dir[i]=='\'' || dir[i]=='\"') { continue; }
		itarget=itarget+1;
		}

	return true;
	}
	

//******************************************************
//**  MFileSearch class
//******************************************************
void MFileSearch::ClearObject(void)
	{
	}


////////////////////////////////////////////////
MFileSearch::MFileSearch(bool create)
	{
	ClearObject();
	if(create==true && Create()==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MFileSearch::~MFileSearch(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MFileSearch::Create(void)
	{
	Destroy();

	if(mDirList.Create()==false)
		{
		return false;
		}

	return true;
	}


////////////////////////////////////////////////
bool MFileSearch::Destroy(void)
	{
	mDirList.Destroy();
	ClearObject();
	return true;
	}


///////////////////////////////////////////////
bool MFileSearch::Print(void)
	{
	const char *dir;
	for(mDirList.ReadReset(); (dir=mDirList.ReadString())!=0; )
		{
		MStdPrintf("Dir : %s\n",dir);
		}
	
	return true;
	}

////////////////////////////////////////////////
bool MFileSearch::AddSearchDir(const char *directory)
	{

	MStdArray<char> dirclean;
	if(GCleanDirName(directory,dirclean)==false)
		{
		return false;
		}
	
	if(mDirList.AddString(dirclean.Get())==false)
		{
		return false;
		}

	return true;
	}


////////////////////////////////////////////////
bool MFileSearch::AddSearchEnvVar(const char *envvariable)	// Search Environment table
	{
	// Set Default to PATH envariable if NULL
	if(envvariable==NULL)
		{
		return false;
		}

	MProcEnvironTable envtable;
	if(envtable.Create()==false)
		{
		return false;
		}

	// Search for path variable
	MString pathinfo;
	if(envtable.Get(envvariable,pathinfo)==false)
		{
		return false;
		}

	// Unable to get path info
	const char *path=pathinfo.Get();
	MDynamicString string;
	if(string.Create(1024)==false)
		{
		return false;
		}

	for(int i=0; ;++i)
		{
		if(path[i]==0)
			{
			if(string.GetLength()!=0)
				{  return mDirList.AddString(string.Get());  }

			return true;
			}

		if(path[i]==';')
			{
			if(string.GetLength()!=0)
				{
				// Clean up directory name
				MStdArray<char> dirclean;
				if(GCleanDirName(string.Get(),dirclean)==false)
					{
					return false;
					}

				if(mDirList.AddString(dirclean.Get())==false) { return false; }
				}

			string.Clear(); continue;
			}

		string.Insert(path[i]);
		}
	}


/////////////////////////////////////////////////
bool MFileSearch::Search(const char *file,MString &fullpath)
	{
	mDirList.ReadReset();

	const char *seachdir;  MFileOps fileops(true);
	while((seachdir=mDirList.ReadString())!=NULL)
		{
		MPathParser testpath(true);
		if(testpath.SetDir(seachdir)==false)
			{
			return false;
			}

		if(testpath.SetFullFileName(file)==false)
			{
			return false;
			}

		if(fileops.Exists(testpath.GetPath())==false) { continue; }

		//=We have found the file
		if(fullpath.Create(testpath.GetPath())==false)
			{
			return false;
			}
		
		return true;
		}

	return false;	
	}


/////////////////////////////////////////////////////
bool MFileSearch::Search(const char *file,MStringList &pathlist)
	{
	mDirList.ReadReset();

	// init path list
	if(pathlist.Create()==false)
		{
		return false;
		}

	const char *seachdir;  MFileOps fileops(true);
	bool retvalue=false;
	while((seachdir=mDirList.ReadString())!=NULL)
		{
		MPathParser testpath(true);
		if(testpath.SetDir(seachdir)==false)
			{
			return false;
			}

		if(testpath.SetFullFileName(file)==false)
			{
			return false;
			}

		if(fileops.Exists(testpath.GetPath())==false) { continue; }

		//=We have found the file

		if(pathlist.AddString(testpath.GetPath())==false)
			{
			return false;
			}

		retvalue=true;
		}

	return retvalue;
	}


