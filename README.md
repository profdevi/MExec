# MExec
MExec takes a source file as an argument and then executes an interpreter or compiler based on the file extension on Windows.  This offers a simplified way of building a quick source file and not worrying about which interpreter or compiler is installed on your windows.

<hr />

 usage:  MExec [src file]+
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;copyright Comine.com  
  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Run one or more source files

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Available Languages:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Haxe  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Dart  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Lua  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Python  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;C           :  Tiny C Compiler tcc  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Java  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;C++         : Microsoft cl Compiler  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Julia  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Rust  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;C++         : LLVM clang++ Compiler  


<hr />

##  Example 1:  

&nbsp;&nbsp;&nbsp; C:>  MExec main.cpp   

&nbsp;&nbsp;&nbsp;Command will search for a C++ compiler and calls the compiler to produce main.exe executable and then executes the compiled program.  If the compiler is Microsoft cl.exe, the command executed will be : (C:> cl main.cpp -o main.exe), followed by the
execution(C:> main.exe ).  The output is paused till the user presses an enter key.

<hr />

You can use cmake for compiling the source code, or a quick compilation of the source code can be done with  

&nbsp;&nbsp;&nbsp;&nbsp;C:>   cl *.cpp -o MExec.exe

