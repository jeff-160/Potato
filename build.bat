@echo off

cd /
cd /d %~dp0

set /p "project_name=Enter project name:"

set "main_file=%project_name%.cpp"
set compile_file=compile.ps1
set "tabchar=    " 

:CreateMain
    echo #include "Potato.hpp" > "%main_file%"
    echo: >> "%main_file%"
    echo int main(){ >> "%main_file%"
    echo %tabchar%Potato::Init();>>"%main_file%"
    echo %tabchar%Potato::Engine engine("%project_name%"); >> "%main_file%"
    echo %tabchar%engine.SetStory({ >> "%main_file%"
    echo: >> "%main_file%"
    echo %tabchar%}); >> "%main_file%"
    echo %tabchar%engine.Run(); >> "%main_file%"
    echo %tabchar%return 0; >> "%main_file%"
    echo } >> "%main_file%"

:CreateCompile
    echo echo Compiling...; > %compile_file%
    echo g++ '%project_name%.cpp' -Llib -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o '%project_name%'; >> %compile_file%
    echo if ($?) {echo Running...; .\\'%project_name%'} >> %compile_file%
    exit

call :CreateMain
call :CreateCompile