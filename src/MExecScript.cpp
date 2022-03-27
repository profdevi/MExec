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

//v0.8 copyright Comine.com 20220325F0735
#include "MStdLib.h"
#include "MProcessJob.h"
#include "MBuffer.h"
#include "MFileSearch.h"
#include "MExecScript.h"


//******************************************************
//**  Module Elements
//******************************************************
struct GExtMap
	{
	const char *Language;
	const char *Extension;
	bool (*Status)(void);
	bool (*Execute)(const char *filename);
	};

//////////////////////////////////////////////////
static bool GGetFileExtension(const char *filepath,MStdArray<char> &extension);
static bool GGetClassName(const char* filepath, MStdArray<char>& classname);
static bool GGetExeName(const char* filepath, MStdArray<char>& exename);
static bool GDoesExeExist(const char* exename);
static bool GRunProcess(const char* cmd);

// List of Scripting files
static bool GStatusHaxe(void);
static bool GExecHaxe(const char* filename);
static bool GStatusDart(void);
static bool GExecDart(const char* filename);
static bool GStatusLua(void);
static bool GExecLua(const char* filename);
static bool GStatusPython(void);
static bool GExecPython(const char* filename);
static bool GStatusTCC(void);
static bool GExecTCC(const char* filename);
static bool GStatusJava(void);
static bool GExecJava(const char* filename);
static bool GStatusCPPCL(void);
static bool GExecCPPCL(const char* filename);
static bool GStatusNodeJS(void);
static bool GExecNodeJS(const char* filename);
static bool GStatusHaskell(void);
static bool GExecHaskell(const char* filename);
static bool GStatusNim(void);
static bool GExecNim(const char* filename);
static bool GStatusJulia(void);
static bool GExecJulia(const char* filename);
static bool GStatusRust(void);
static bool GExecRust(const char* filename);
static bool GStatusCPPCLangPP(void);
static bool GExecCPPCLangPP(const char* filename);



//////////////////////////////////////////////////
struct GExtMap GExtMapTable[]=
	{
	"Haxe",".hx",GStatusHaxe,GExecHaxe,
	"Dart",".dart",GStatusDart,GExecDart,
	"Lua",".lua",GStatusLua,GExecLua,
	"Python",".py",GStatusPython,GExecPython,
	"C           :  Tiny C Compiler tcc",".c",GStatusTCC,GExecTCC,
	"Java",".java",GStatusJava,GExecJava,
	"C++         : Microsoft cl Compiler",".cpp",GStatusCPPCL,GExecCPPCL,
	"TypeScript",".js",GStatusNodeJS,GExecNodeJS,
	"Haskell",".hs",GStatusHaskell,GExecHaskell,
	"Nim",".nim",GStatusNim,GExecNim,
	"Julia",".jl",GStatusJulia,GExecJulia,
    "Rust",".rs",GStatusRust,GExecRust,
	"C++         : LLVM clang++ Compiler",".cpp",GStatusCPPCLangPP,GExecCPPCLangPP,
	0,0,0,0
	};


//////////////////////////////////////////////////
static bool GGetFileExtension(const char* filepath, MStdArray<char>& extension)
	{
	if(filepath==0 || *filepath==0) { return false; }

	// Move to end of string.
	int index;
	for(index=0;filepath[index]!=0;++index){}

	int length=0;
	for(index=index-1; index>=0; --index)
		{
		length = length + 1;
		if(filepath[index]=='.') { break; }
		if(MStdIsAlphaNumeric(filepath[index])==false && filepath[index]!='_')
			{ return false; }
		}
	
	if(index==0) { return false; }
	
	if(extension.Create(length+1)==false)
		{
		return false;
		}

	for(int i=0; ; ++i)
		{
		extension[i]=filepath[index+i];
		if(extension[i]==0) { break; }
		}

	return true;
	}


//////////////////////////////////////////////////
static bool GGetClassName(const char* filepath, MStdArray<char>& classname)
	{
	MStdAssert(filepath!=0 && *filepath!=0);
	const char *filename=MStdPathGetFileName(filepath);
	MStdAssert(filename!=0);

	int index;
	for(index=0;filename[index]!=0;++index)
		{
		if(filename[index]=='.') { break; }
		}
	
	//index points '\0' or '.'

	if(classname.Create(index+2)==false)
		{
		return false;
		}

	for(int i=0;i<index;++i)
		{
		classname[i]=filename[i];
		}
	
	classname[index]=0;
	return true;
	}


/////////////////////////////////////////////////
static bool GGetExeName(const char* filepath, MStdArray<char>& exename)
	{
	return GGetClassName(filepath,exename);
	}


/////////////////////////////////////////////////
static bool GDoesExeExist(const char *exename)
	{
	// Check if haxe.exe is in file path
	MFileSearch filesearch(true);
	if(filesearch.AddSearchEnvVar("PATH")==false)
		{
		return false;
		}

	MStringList pathlist;
	if(filesearch.Search(exename,pathlist)==false) { return false; }

	return true;
	}


///////////////////////////////////////////////////
static bool GRunProcess(const char *cmd)
	{
	// Run Process
	MProcessJob processjob;
	if(processjob.Create(cmd)==false)
		{
		return false;
		}

	processjob.Wait();
	return true;
	}


///////////////////////////////////////////////////
static bool GRunProcessWait(const char* command)
	{
	MStdPrintf("Running Command : %s\n",command);
	MStdPrintf("-------------------------------------\n");

	// Run Process
	MProcessJob processjob;
	if(processjob.Create(command)==false)
		{
		MStdPrintf("Error:  Unable to execute %s\n",command );
		return false;
		}

	processjob.Wait();

	MStdPrintf("-------------------------------------\n");
	// Flush
	MStdFileFlush(MStdGetStdOut());

	MStdPrintf("Press <ENTER> to continue....\n");

	MBuffer waitbuffer(1000);
	MStdGetString(waitbuffer.GetBuffer(),waitbuffer.GetSize()-2);
	
	return true;
	}


//////////////////////////////////////////////////
// Haxe Programming
//////////////////////////////////////////////////
static bool GStatusHaxe(void)
	{
	// Check if executable exists in PATH
	if(GDoesExeExist("haxe.exe")==false) { return false; }

	//=haxe is in file search path
	
	MStdPrintf("haxe  ");

	// Run Process
	if(GRunProcess("haxe --version")==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
static bool GExecHaxe(const char* filename)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("haxe.exe") == false) { return false; }

	//=haxe is in file search path

	// Get the class name for file
	MStdArray<char> classname;
	if(GGetClassName(filename,classname)==false)
		{
		return false;
		}

	// Build up command line 
	MBuffer buffer(1000);
	MStdSPrintf(buffer.GetBuffer(),buffer.GetSize()-2
			,"haxe --interp --main %s",classname.Get());

	if(GRunProcessWait(buffer.GetBuffer())==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
// Dart Programming
//////////////////////////////////////////////////
static bool GStatusDart(void)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("dart.exe") == false) { return false; }

	//=dart is in file search path

	// Run Process
	if(GRunProcess("dart --version")==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
static bool GExecDart(const char* filename)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("dart.exe") == false) { return false; }

	//=dart is in file search path

	// Build up command line 
	MBuffer buffer(1000);
	MStdSPrintf(buffer.GetBuffer(), buffer.GetSize() - 2
			, "dart %s", filename);

	if(GRunProcessWait(buffer.GetBuffer())==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
// Lua Programming
//////////////////////////////////////////////////
static bool GStatusLua(void)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("lua.exe") == false) { return false; }

	//=lua is in file search path

	// Run Process
	if(GRunProcess("lua -v")==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
static bool GExecLua(const char* filename)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("lua.exe") == false) { return false; }

	//=lua is in file search path

	// Build up command line 
	MBuffer buffer(1000);
	MStdSPrintf(buffer.GetBuffer(),buffer.GetSize()-2
			,"lua %s",filename);

	if(GRunProcessWait(buffer.GetBuffer())==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
// Python Programming
//////////////////////////////////////////////////
static bool GStatusPython(void)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("python.exe") == false) { return false; }

	//=python is in file search path

	// Run Process
	if(GRunProcess("python --version")==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
static bool GExecPython(const char* filename)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("python.exe") == false) { return false; }

	//=python is in file search path

	// Build up command line 
	MBuffer buffer(1000);
	MStdSPrintf(buffer.GetBuffer(),buffer.GetSize()-2
			,"python %s",filename);

	if(GRunProcessWait(buffer.GetBuffer())==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
// TCC Programming
//////////////////////////////////////////////////
static bool GStatusTCC(void)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("tcc.exe") == false) { return false; }

	//=Executable in path

	// Run Process
	if(GRunProcess("tcc -v")==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
static bool GExecTCC(const char* filename)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("tcc.exe") == false) { return false; }

	//=Executable in path

	// Build up command
	MBuffer buffer(1000);
	MStdSPrintf(buffer.GetBuffer(), buffer.GetSize() - 2
		, "tcc %s", filename);
	MStdPrintf("Compiling : %s\n", buffer.GetBuffer());

	if(GRunProcess(buffer.GetBuffer())==false)
		{
		MStdPrintf("Error: Compilation Failed\n");
		return false;
		}

	// Get Exe Name
	MStdArray<char> exename;
	if(GGetExeName(filename,exename)==false)
		{
		MStdPrintf("Error: Unable to get exe name for file");
		return false;
		}

	if(GRunProcessWait(exename.Get())==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
// Java Programming
//////////////////////////////////////////////////
static bool GStatusJava(void)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("javac.exe") == false) { return false; }

	//=javac is in file search path

	// Run Process
	if(GRunProcess("javac -version")==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
static bool GExecJava(const char* filename)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("javac.exe") == false) { return false; }

	//=javac is in file search path

	// Build up command
	MBuffer buffer(1000);
	MStdSPrintf(buffer.GetBuffer(), buffer.GetSize() - 2
		, "javac %s", filename);
	MStdPrintf("Compiling : %s\n", buffer.GetBuffer());

	if(GRunProcess(buffer.GetBuffer())==false)
		{
		MStdPrintf("Error: Compilation Failed\n");
		return false;
		}

	// Get Class Name
	MStdArray<char> classname;
	if(GGetClassName(filename,classname)==false)
		{
		MStdPrintf("Error: Unable to get class name for file");
		return false;
		}

	// Build
	MStdSPrintf(buffer.GetBuffer(), buffer.GetSize() - 2,
			"java -classpath . %s", classname.Get());

	if(GRunProcessWait(buffer.GetBuffer())==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
// C++ Microsoft CL 
//////////////////////////////////////////////////
static bool GStatusCPPCL(void)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("cl.exe") == false) { return false; }

	//=cl.exe is in file search path

	// Run Process
	if(GRunProcess("cl.exe")==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
static bool GExecCPPCL(const char* filename)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("cl.exe") == false) { return false; }

	//=cl is in file search path

	// Build up command
	MBuffer buffer(1000);
	MStdSPrintf(buffer.GetBuffer(), buffer.GetSize() - 2
			, "cl %s", filename);
	MStdPrintf("Compiling : %s\n", buffer.GetBuffer());

	if(GRunProcess(buffer.GetBuffer())==false)
		{
		MStdPrintf("Error: Compilation Failed\n");
		return false;
		}

	// Get Exe Name
	MStdArray<char> exename;
	if(GGetExeName(filename,exename)==false)
		{
		MStdPrintf("Error: Unable to get class name for file");
		return false;
		}
	// Build
	MStdSPrintf(buffer.GetBuffer(), buffer.GetSize() - 2,
			"%s", exename.Get());

	if(GRunProcessWait(buffer.GetBuffer())==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
// Node JavaScript Programming
//////////////////////////////////////////////////
static bool GStatusNodeJS(void)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("node.exe") == false) { return false; }

	//=node is in file search path

	MStdPrintf("Node ");

	// Run Process
	if(GRunProcess("node.exe --version")==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
static bool GExecNodeJS(const char* filename)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("node.exe") == false) { return false; }

	//=node is in file search path

	// Build up command line 
	MBuffer buffer(1000);
	MStdSPrintf(buffer.GetBuffer(),buffer.GetSize()-2
			,"node %s",filename);

	if(GRunProcessWait(buffer.GetBuffer())==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
// Haskell Programming
//////////////////////////////////////////////////
static bool GStatusHaskell(void)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("ghc.exe") == false) { return false; }

	//=glasgow compiler is in file search path

	// Run Process
	if(GRunProcess("ghc.exe --version")==false)
		{
		return false;
		}

	return true;
	}

	
////////////////////////////////////////////////////////////////
static bool GExecHaskell(const char* filename)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("ghc.exe") == false) { return false; }

	//=ghc.exe is in file search path

	// Build up command
	MBuffer buffer(1000);
	MStdSPrintf(buffer.GetBuffer(), buffer.GetSize() - 2
			, "ghc %s", filename);
	MStdPrintf("Compiling : %s\n", buffer.GetBuffer());

	if(GRunProcess(buffer.GetBuffer())==false)
		{
		MStdPrintf("Error: Compilation Failed\n");
		return false;
		}

	// Get Exe Name
	MStdArray<char> exename;
	if(GGetExeName(filename,exename)==false)
		{
		MStdPrintf("Error: Unable to get class name for file");
		return false;
		}
	// Build
	MStdSPrintf(buffer.GetBuffer(), buffer.GetSize() - 2,
			"%s", exename.Get());

	if(GRunProcessWait(buffer.GetBuffer())==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
// Nim Programming Language
//////////////////////////////////////////////////
static bool GStatusNim(void)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("nim.exe") == false) { return false; }

	//=nim compiler is in file search path

	// Run Process
	if(GRunProcess("nim.exe --version")==false)
		{
		return false;
		}

	return true;
	}

	
////////////////////////////////////////////////////////////////
static bool GExecNim(const char* filename)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("nim.exe") == false) { return false; }

	//=nim.exe is in file search path

	// Build up command
	MBuffer buffer(1000);
	MStdSPrintf(buffer.GetBuffer(), buffer.GetSize() - 2
			, "nim compile --run %s", filename);
	MStdPrintf("Compiling and running: %s\n", buffer.GetBuffer());

	if(GRunProcessWait(buffer.GetBuffer())==false)
		{
		MStdPrintf("Error: Compilation Failed\n");
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
// Julia Programming Language
//////////////////////////////////////////////////
static bool GStatusJulia(void)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("julia.exe") == false) { return false; }

	//=nim compiler is in file search path

	// Run Process
	if(GRunProcess("julia.exe --version")==false)
		{
		return false;
		}

	return true;
	}

	
////////////////////////////////////////////////////////////////
static bool GExecJulia(const char* filename)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("julia.exe") == false) { return false; }

	//=julia.exe is in file search path

	// Build up command
	MBuffer buffer(1000);
	MStdSPrintf(buffer.GetBuffer(), buffer.GetSize() - 2
			, "julia %s", filename);
	MStdPrintf("Running: %s\n", buffer.GetBuffer());

	if(GRunProcessWait(buffer.GetBuffer())==false)
		{
		MStdPrintf("Error: Run Failed\n");
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
// Rust Programming
//////////////////////////////////////////////////
static bool GStatusRust(void)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("rustc.exe") == false) { return false; }

	//=Executable in path

	// Run Process
	if(GRunProcess("rustc --version")==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
static bool GExecRust(const char* filename)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("rustc.exe") == false) { return false; }

	//=Executable in path

	// Build up command
	MBuffer buffer(1000);
	MStdSPrintf(buffer.GetBuffer(), buffer.GetSize() - 2
		, "rustc %s", filename);
	MStdPrintf("Compiling : %s\n", buffer.GetBuffer());

	if(GRunProcess(buffer.GetBuffer())==false)
		{
		MStdPrintf("Error: Compilation Failed\n");
		return false;
		}

	// Get Exe Name
	MStdArray<char> exename;
	if(GGetExeName(filename,exename)==false)
		{
		MStdPrintf("Error: Unable to get exe name for file");
		return false;
		}

	if(GRunProcessWait(exename.Get())==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
// clang++ 
//////////////////////////////////////////////////
static bool GStatusCPPCLangPP(void)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("clang++.exe") == false) { return false; }

	//=cl.exe is in file search path

	// Run Process
	if(GRunProcess("clang++.exe --version")==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
static bool GExecCPPCLangPP(const char* filename)
	{
	// Check if executable exists in PATH
	if (GDoesExeExist("clang++.exe") == false) { return false; }

	//=cl is in file search path

	// Get Exe Name
	MStdArray<char> exename;
	if(GGetExeName(filename,exename)==false)
		{
		MStdPrintf("Error: Unable to get class name for file");
		return false;
		}

	// Build up command
	MBuffer buffer(1000);
	MStdSPrintf(buffer.GetBuffer(), buffer.GetSize() - 2
			, "clang++ %s -o %s.exe", filename,exename.Get() );
	MStdPrintf("Compiling : %s\n", buffer.GetBuffer());

	if(GRunProcess(buffer.GetBuffer())==false)
		{
		MStdPrintf("Error: Compilation Failed\n");
		return false;
		}

	// Build run executable
	MStdSPrintf(buffer.GetBuffer(), buffer.GetSize() - 2,
			"%s", exename.Get());

	if(GRunProcessWait(buffer.GetBuffer())==false)
		{
		return false;
		}

	return true;
	}


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
	return true;
	}


////////////////////////////////////////////////
bool MExecScript::Destroy(void)
	{
	ClearObject();
	return true;
	}


//////////////////////////////////////////////////
bool MExecScript::PrintLanguages(const char *prefix)
	{
	for(int i=0;GExtMapTable[i].Extension!=0;++i)
		{
		MStdPrintf("%s%s\n",prefix,GExtMapTable[i].Language);
		}

	return true;
	}


//////////////////////////////////////////////////
bool MExecScript::PrintStatus(void)
	{
	for(int i=0;GExtMapTable[i].Extension!=0;++i)
		{
		(*GExtMapTable[i].Status)();
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

	MStdArray<char> extension;
	if(GGetFileExtension(filename,extension)==false)
		{
		MStdPrintf("**Unable to get file extension for %s\n",filename);
		return false;
		}

	// Search for extension
	for(int i=0;GExtMapTable[i].Execute!=0;++i)
		{
		if(MStdStrCmp(GExtMapTable[i].Extension,extension.Get())!=0) { continue; }

		//=We Found the extension.
		if((*GExtMapTable[i].Execute)(filename)==true)
			{
			return true;
			}

		//=Failed execution.  Try again.
		}

	return false;
	}

    


