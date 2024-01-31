includeDLL = -I.\include\ -L.\lib\ -lmingw32 -lSDL2main -lSDL2
flags = -std=c++20 -O3 -Wall -mwindows 

all :
	g++ .\src\main.cpp .\src\SDL2PXS.cpp -o main $(flags) $(includeDLL)
	.\main.exe
build :
	g++ -c .\src\SDL2PXS.cpp -o SDL2PXS.o $(flags) $(includeDLL)
	ar cr SDL2PXS.lib SDL2PXS.o
	del "SDL2PXS.o"
	g++ .\src\main.cpp SDL2PXS.lib -o main $(flags) $(includeDLL)