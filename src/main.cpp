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
#include "MCommandArg.h"
#include "MExecScript.h"


//******************************************************
//* Module Elements
//******************************************************
static const char *GApplicationName="MExec";	// Used in Help
static const char *GApplicationVersion="0.8";	// Used in Help

////////////////////////////////////////////////////
static void GDisplayHelp(void);

////////////////////////////////////////////////////
int main(int argn,const char *argv[])
	{
	MCommandArg args(argn,argv);

	///////////////////////////////////////////////
	if(args.GetArgCount()<2)
		{
		GDisplayHelp();
		return 0;
		}

	if(args.CheckRemoveHelp()==true)
		{
		GDisplayHelp();
		return 0;
		}

	MExecScript execscript;
	if(execscript.Create()==false)
		{
		MStdPrintf("**System Failed\n");
		return 1;
		}

	// Check if arg show status
	if(args.CheckRemoveArg("-s")==true)
		{
		execscript.PrintStatus();
		return 0;
		}

	const int argcount=args.GetArgCount();
	for(int i=1;i<argcount;i=i+1)
		{
		if(execscript.Exec(args.GetArg(i))==true) { break; }
		}

	return 0;
	}


////////////////////////////////////////////////////
static void GDisplayHelp(void)
	{
	MStdPrintf(	"\n"
				"   usage:  %s [-?|-s]\n"
				"           v%s copyright Comine.com\n"
				"\n"
				"   Run a source file of languages\n"
				"\n"
				"      -s   :  Run status of languages\n"
				"\n"
				,GApplicationName,GApplicationVersion);

	MStdPrintf(	"   Available Languages:\n");
	MExecScript exec(true);
	exec.PrintLanguages("\t\t");
	MStdPrintf("\n");
	}


