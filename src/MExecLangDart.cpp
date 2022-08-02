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

//v1.0 copyright Comine.com 20220423S0752
#include "MStdLib.h"
#include "MString.h"
#include "MFileSearch.h"
#include "MIExecLang.h"
#include "MProcessJob.h"
#include "MBuffer.h"
#include "MExecLangDart.h"


//******************************************************
//**  Virtual Interface Implementation
//******************************************************
////////////////////////////////////////////
const char *MExecLangDart::GetCompilerInfo(void)
	{
	return "Dart Interpreter";
	}


/////////////////////////////////////////////
bool MExecLangDart::IsAvailable(void)
	{
	MFileSearch filesearch(true);
	if (filesearch.AddSearchEnvVar("PATH") == false)
		{
		return false;
		}

	MString luapath;
	if(filesearch.Search("dart.exe",luapath)==false) { return false; }

	return true;
	}

////////////////////////////////////////////
const char *MExecLangDart::GetFileExtension(void)
	{
	return ".lua";
	}


////////////////////////////////////////////
bool MExecLangDart::PrintVersion(void)
	{
	MProcessJob procjob;
	if(procjob.Create("dart --version")==false)
		{
		return false;
		}

	procjob.Wait();
	return true;
	}


//////////////////////////////////////////
bool MExecLangDart::Run(const char* name)
	{
	MBuffer cmdline(1000);
	MStdSPrintf(cmdline.GetBuffer(),cmdline.GetSize()-1,"dart %s",name);

	MProcessJob proc;
	if(proc.Create(cmdline.GetBuffer())==false)
		{
		return false;
		}

	proc.Wait();
	proc.Destroy();

	return true;
	}


//******************************************************
//**  MExecLangDart class
//******************************************************
void MExecLangDart::ClearObject(void)
	{
	}


////////////////////////////////////////////////
MExecLangDart::MExecLangDart(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MExecLangDart::~MExecLangDart(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MExecLangDart::Create(void)
	{
	Destroy();
	return true;
	}


////////////////////////////////////////////////
bool MExecLangDart::Destroy(void)
	{
	ClearObject();
	return true;
	}



