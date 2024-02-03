include = -I.\include\ -L.\lib\ -lmingw32 -lSDL2main -lSDL2 -llineAlgorithm 
flags = -std=c++20 -O3 -Wall -Werror

test :
	g++ .\src\main.cpp .\src\SDL2PXS.cpp -o main $(flags) $(include)
	.\main.exe
build :
	g++ -c .\src\SDL2PXS.cpp -o SDL2PXS.o $(flags) $(include)
	ar cr SDL2PXS.lib SDL2PXS.o
	del "SDL2PXS.o"
	g++ .\src\main.cpp SDL2PXS.lib -o main -mwindows $(flags) $(include)