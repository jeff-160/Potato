echo Compiling...; 
g++ 'main.cpp' -Llib -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o 'main'; 
if ($?) {echo Running...; .\\'main'} 
