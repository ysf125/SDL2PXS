include = -I.\SDL2PXSInclude\ -I.\SDL2Include\ -L.\lib\ -L.\ -lmingw32 -lSDL2main -lSDL2 -llineAlgorithm
flags = -std=c++20 -O3 -Wall -Werror

testAll :
	make buildDll
	make buildExe
	cls

buildDll :
	g++ .\src\SDL2PXS.cpp -o SDL2PXS.o -c $(flags) $(include)
	g++ .\src\plane2D.cpp -o plane2D.o -c $(flags) $(include)
	g++ SDL2PXS.o plane2D.o -o SDL2PXS.dll -shared $(dllFlags) $(flags) $(include)
	del "plane2D.o"
	del "SDL2PXS.o"

buildExe :
	g++ .\src\main.cpp -o main -lSDL2PXS $(exeFlags) $(flags) $(include)
	.\main.exe