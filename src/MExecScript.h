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
#ifndef MExecScript_h
#define MExecScript_h

/////////////////////////////////////////////////
#include "MStdLib.h"

/*
	Executes a single script file based on the file exetension.

		.hx		Haxe
		.dart	Dart
		.lua	Lua 
		.py		Python
		.c		tiny C Compiler
		.java	java
		.cpp	c++ MS Compiler CL
		.js		node Javascript
		.hs		Haskell Compiler
		.nim	nim compiler
		.jl     julia compiler
        .rs     rust compiler
*/

//******************************************************
//**  MExecScript class
//******************************************************
class MExecScript
	{
	////////////////////////////////////////////////
	
	////////////////////////////////////////////////
	void ClearObject(void);

	////////////////////////////////////////////////
	public:
	MExecScript(bool create=false);
	~MExecScript(void);
	bool Create(void);
	bool Destroy(void);
	bool PrintLanguages(const char *prefix="");
	bool PrintStatus(void);
	bool Exec(const char *filename);
	};

#endif // MExecScript_h

