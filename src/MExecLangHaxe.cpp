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
#include "MPathParser.h"
#include "MExecLangHaxe.h"


//******************************************************
//**  Virtual Interface Implementation
//******************************************************
////////////////////////////////////////////
const char *MExecLangHaxe::GetCompilerInfo(void)
	{
	return "Haxe Interpreter";
	}


/////////////////////////////////////////////
bool MExecLangHaxe::IsAvailable(void)
	{
	MFileSearch filesearch(true);
	if (filesearch.AddSearchEnvVar("PATH") == false)
		{
		return false;
		}

	MString path;
	if(filesearch.Search("haxe.exe",path)==false) { return false; }

	return true;
	}

////////////////////////////////////////////
const char *MExecLangHaxe::GetFileExtension(void)
	{
	return ".hx";
	}


////////////////////////////////////////////
bool MExecLangHaxe::PrintVersion(void)
	{
	MStdPrintf("Haxe ");
	MProcessJob procjob;
	if(procjob.Create("haxe --version")==false)
		{
		return false;
		}

	procjob.Wait();
	return true;
	}


//////////////////////////////////////////
bool MExecLangHaxe::Run(const char* name)
	{
	MPathParser pathparse(true);
	pathparse.SetFullFileName(name);

	if(MStdStrICmp(pathparse.GetFileExt(),".hx")!=0)
		{
		return false;
		}

	MBuffer cmdline(1000);
	MStdSPrintf(cmdline.GetBuffer(),cmdline.GetSize()-1
			,"haxe --interp --main %s %s"
			,pathparse.GetFileName(),name);

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
//**  MExecLangHaxe class
//******************************************************
void MExecLangHaxe::ClearObject(void)
	{
	}


////////////////////////////////////////////////
MExecLangHaxe::MExecLangHaxe(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MExecLangHaxe::~MExecLangHaxe(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MExecLangHaxe::Create(void)
	{
	Destroy();
	return true;
	}


////////////////////////////////////////////////
bool MExecLangHaxe::Destroy(void)
	{
	ClearObject();
	return true;
	}



