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

//v1.0 copyright Comine.com 20220422S2241
#include "MStdLib.h"
#include "MString.h"
#include "MFileSearch.h"
#include "MIExecLang.h"
#include "MProcessJob.h"
#include "MBuffer.h"
#include "MFile.h"
#include "MPathParser.h"
#include "MExecLangTinyCC.h"


//******************************************************
//**  Virtual Interface Implementation
//******************************************************
////////////////////////////////////////////
const char *MExecLangTinyCC::GetCompilerInfo(void)
	{
	return "Tiny CC Compiler";
	}


/////////////////////////////////////////////
bool MExecLangTinyCC::IsAvailable(void)
	{
	MFileSearch filesearch(true);
	if (filesearch.AddSearchEnvVar("PATH") == false)
		{
		return false;
		}

	MString path;
	if(filesearch.Search("tcc.exe",path)==false) { return false; }

	return true;
	}

////////////////////////////////////////////
const char *MExecLangTinyCC::GetFileExtension(void)
	{
	return ".c";
	}


////////////////////////////////////////////
bool MExecLangTinyCC::PrintVersion(void)
	{
	const char* tmpfile = "__MExecLangTinyCC__.tmp";
	MStdFileRemove(tmpfile);

	MBuffer cmdline(1000);
	MStdSPrintf(cmdline.GetBuffer(), cmdline.GetSize() - 1
			, "tcc.exe -v >%s", tmpfile);

	MFileOutput fileout;
	if(fileout.Create(tmpfile)==false)
		{
		return false;
		}

	MProcessJob procjob;
	if(procjob.Create(cmdline.GetBuffer(),INVALID_HANDLE_VALUE
				,fileout.GetHandle(),fileout.GetHandle() )==false)
		{
		return false;
		}

	procjob.Wait();
	fileout.Destroy();

	MStdArray<char> filetext;
	if (MStdFileReadText(tmpfile, filetext) == false)
		{
		return false;
		}

	//  Remove tmpfile
	MStdFileRemove(tmpfile);

	// Write zero to New Line 
	for(char *p=filetext.Get();*p!=0;++p)
		{
		if (*p == '\r' || *p == '\n')
			{
			*p=0;
			break;
			}
		}

	MStdPrintf("%s\n",filetext.Get() );
	return true;
	}


//////////////////////////////////////////
bool MExecLangTinyCC::Run(const char* name)
	{
	MPathParser pathparse(true);
	pathparse.SetFullFileName(name);
	if(MStdStrICmp(pathparse.GetFileExt(),".c")!=0)
		{
		return false;
		}

	// Remove all tmp files
	pathparse.SetFileExt(".exe");
	MStdFileRemove(pathparse.GetFullFileName());
	
	MBuffer cmdline(1000);
	MStdSPrintf(cmdline.GetBuffer(),cmdline.GetSize()-1,"tcc.exe %s -o %s"
			,name,pathparse.GetFullFileName() );

	MProcessJob proc;
	if(proc.Create(cmdline.GetBuffer())==false)
		{
		return false;
		}

	proc.Wait();
	proc.Destroy();

	// Check if file exe exists
	if(MStdFileExists(pathparse.GetFullFileName())==false)
		{ 
		MStdPrintf("****** Failed Compilation. Press Return.  ****** \n");

		char buf[100];
		MStdGetString(buf,sizeof(buf)-2);

		return false;
		}

	MStdPrintf("\n============ Running Executable %s ======== \n"
			,pathparse.GetFullFileName() );

	// Run Executable 
	if(proc.Create(pathparse.GetFullFileName())==false)
		{
		}

	proc.Wait();
	proc.Destroy();

	return true;
	}


//******************************************************
//**  MExecLangTinyCC class
//******************************************************
void MExecLangTinyCC::ClearObject(void)
	{
	}


////////////////////////////////////////////////
MExecLangTinyCC::MExecLangTinyCC(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MExecLangTinyCC::~MExecLangTinyCC(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MExecLangTinyCC::Create(void)
	{
	Destroy();
	return true;
	}


////////////////////////////////////////////////
bool MExecLangTinyCC::Destroy(void)
	{
	ClearObject();
	return true;
	}



