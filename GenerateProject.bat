@echo off

SET /A ARGS_COUNT=0    
FOR %%A in (%*) DO SET /A ARGS_COUNT+=1    

IF %ARGS_COUNT% == 0 (
    echo on
    CALL vendor\premake\premake5.exe vs2019
    echo off
    PAUSE
)

IF %ARGS_COUNT% == 1 (
    echo on
    CALL vendor\premake\premake5.exe %1
    echo off
    PAUSE
)


