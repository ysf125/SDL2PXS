include = -I.\include\ -I.\src\ -L.\lib\ -L.\ -lmingw32 -lSDL2main -lSDL2 -llineAlgorithm
flags = -std=c++20 -O3 -Wall -Werror

test-all :
	make build-dll
	make build-exe
	.\main.exe

build-dll :
	g++ .\src\SDL2PXS.cpp -o SDL2PXS.o -c $(dllFlags) $(flags) $(include)
	g++ SDL2PXS.o -o SDL2PXS.dll -shared $(dllFlags) $(flags) $(include)
	del "SDL2PXS.o"

build-exe :
	g++ .\src\main.cpp -o main -lSDL2PXS $(exeFlags) $(flags) $(include)