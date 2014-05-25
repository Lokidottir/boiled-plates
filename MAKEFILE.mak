#Boiled Plates makefile, primarily used for creating test programs.
CC=g++
CC_FLAGS=-static -static-libgcc -static-libstdc++ -Wall -std=c++11
#INCLUDES=

all: maths-test

maths-test: 
	$(CC) $(CC_FLAGS) test_env/test_maths.cpp -o maths-test

clean:
	rm -rf *.exe

C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files (x86)\Intel\iCLS Client\;C:\Program Files\Intel\iCLS Client\;C:\Program Files\Common Files\Microsoft Shared\Windows Live;C:\Program Files (x86)\Common Files\Microsoft Shared\Windows Live;%SystemRoot%\system32;%SystemRoot%;%SystemRoot%\System32\Wbem;%SYSTEMROOT%\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\Windows Live\Shared;C:\Program Files\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files\mingw64\bin;C:\Program Files (x86)\QuickTime\QTSystem\;C:\Program Files (x86)\Git\cmd;C:\Program Files (x86)\clisp-2.49;C:\D\dmd2\windows\bin;C:\D\dm\bin;C:\Program Files\msys\bin