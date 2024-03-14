include = -I.\SDL2PXSInclude\ -I.\SDL2Include\ -L.\lib\ -L.\ -lmingw32 -lSDL2main -lSDL2 -llineAlgorithm
flags = -std=c++20 -O3 -Wall -Werror

testAll :
	make buildDll
	make buildExe
	cls

buildDll :
	g++ .\src\SDL2PXSMain.cpp -o SDL2PXSMain.o -c $(flags) $(include)
	g++ .\src\SDL2PXS.cpp -o SDL2PXS.o -c $(flags) $(include)
	g++ SDL2PXSMain.o SDL2PXS.o -o SDL2PXS.dll -shared $(dllFlags) $(flags) $(include)
	del "SDL2PXSMain.o"
	del "SDL2PXS.o"

buildExe :
	g++ .\src\main.cpp -o main -lSDL2PXS $(exeFlags) $(flags) $(include)
	.\main.exe