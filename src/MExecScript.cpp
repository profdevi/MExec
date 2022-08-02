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

//v0.9 copyright Comine.com 20220802T1113
#include "MStdLib.h"
#include "MProcessJob.h"
#include "MBuffer.h"
#include "MFileSearch.h"
#include "MPathParser.h"
#include "MExecScript.h"
#include "MIExecLang.h"

// Language List
#include "MExecLangClangCPP.h"
#include "MExecLangDart.h"
#include "MExecLangHaxe.h"
#include "MExecLangJava.h"
#include "MExecLangJulia.h"
#include "MExecLangLua.h"
#include "MExecLangMSCPP.h"
#include "MExecLangPython.h"
#include "MExecLangRust.h"
#include "MExecLangTinyCC.h"

//******************************************************
//**  Module Elements
//******************************************************
static MExecLangClangCPP GExecClang;
static MExecLangDart GExecDart;
static MExecLangHaxe GExecHaxe;
static MExecLangJava GExecJava;
static MExecLangJulia GExecJulia;
static MExecLangLua GExecLua;
static MExecLangMSCPP GExecMSCPP;
static MExecLangPython GExecPython;
static MExecLangRust GExecRust;
static MExecLangTinyCC GExecTinyCC;

//////////////////////////////////////////////
static MIExecLang *GLangList[]
	{
	&GExecClang,
	&GExecDart,
	&GExecHaxe,
	&GExecJava,
	&GExecJulia,
	&GExecLua,
	&GExecMSCPP,
	&GExecPython,
	&GExecRust,
	&GExecTinyCC,
	nullptr
	};



//******************************************************
//**  MExecScript class
//******************************************************
void MExecScript::ClearObject(void)
	{
	}


////////////////////////////////////////////////
MExecScript::MExecScript(bool create)
	{
	ClearObject();
	if(create==true && Create()==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MExecScript::~MExecScript(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MExecScript::Create(void)
	{
	Destroy();

	for(int i=0;GLangList[i]!=nullptr;++i)
		{
		if(GLangList[i]->Create()==false)
			{
			Destroy();
			return false;
			}
		}
	return true;
	}


////////////////////////////////////////////////
bool MExecScript::Destroy(void)
	{
	ClearObject();
	for(int i=0;GLangList[i]!=nullptr;++i)
		{
		GLangList[i]->Destroy();
		}

	return true;
	}


//////////////////////////////////////////////////
bool MExecScript::PrintStatus(void)
	{
	for(int i=0;GLangList[i]!=nullptr;++i)
		{
		MStdPrintf("Executable: %-30s",GLangList[i]->GetCompilerInfo() );
		MStdPrintf(" Ext: %-5s ",GLangList[i]->GetFileExtension() );
		if(GLangList[i]->IsAvailable()==true)
			{
			MStdPrintf("     [FOUND]");
			}

		MStdPrintf("\n");
		}

	return true;
	}

/////////////////////////////////////////////////
bool MExecScript::Exec(const char* filename)
	{
	// Skip a file that does not exist
	if(MStdFileExists(filename)==false) { return false; }

	// Skip if file is binary
	if(MStdFileIsBinary(filename)==true) { return false; }

	MPathParser pathparser;
	if(pathparser.Create()==false)
		{
		return false;
		}

	MStdArray<char> abspath;
	if(MStdPathGetAbsolute(filename,abspath)==false)
		{
		return false;
		}

	if(pathparser.SetPath(abspath.Get())==false)
		{
		return false;
		}

	const char *pfileext=pathparser.GetFileExt();
	if(pfileext==nullptr)
		{
		return false;
		}

	for(int i=0;GLangList[i]!=nullptr;++i)
		{
		if(MStdStrICmp(pfileext,GLangList[i]->GetFileExtension())!=0) { continue; }

		if(GLangList[i]->IsAvailable()==false) { continue; }

		//= Found Language to Execute

		if(GLangList[i]->Run(filename)==true) { return true; }
		}
	
	return false;
	}

    


