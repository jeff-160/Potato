@echo off

cd /
cd /d %~dp0

set /p project_name=Enter project name:
set main_file=%project_name%.cpp
set compile_file=compile.ps1
set "tabchar=    " 

:CreateMain
    echo #include "Potato.hpp" > %main_file%
    echo:
    echo int main(){ >> %main_file%
    echo %tabchar%Engine engine("%project_name%"); >> %main_file%
    echo %tabchar%return 0; >> %main_file%
    echo } >> %main_file%
    goto CreateCompile


:CreateCompile
    echo echo Compiling...; > %compile_file%
    echo g++ 'main.cpp' -Llib -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o 'main'; >> %compile_file%
    echo if ($?) {echo Running...; .\\'main'} >> %compile_file%
    exit

goto CreateMain