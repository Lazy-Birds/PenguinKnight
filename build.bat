@echo off

set project_root=%~dp0%
set exe_name=pix16_debug.exe
set build_hash=0
set ship_mode=0

pushd %project_root%
    ::FOR /F "tokens=*" %%g IN ('more %project_root%\.git\refs\heads\main') do (set build_hash=%%g)

    if exist build/pix16.exe (rmdir /s /q build)
    
    if not exist build (mkdir build)

    pushd build

        set flags=/Od -Oi -Zo -FC -Z7 -fp:fast -fp:except- /Zc:strictStrings- -Gm- -GR- -GS- -Gs9999999
        cl.exe -nologo /MD -DDEBUG=1 -DSHIP_MODE=0 /I ..\src %flags%  ..\src\pix16_win32.cpp /link -subsystem:windows -incremental:no -opt:ref -OUT:%exe_name%
        IF %errorlevel% NEQ 0 (popd && goto end)

        :: Run
        .\%exe_name%
    popd 

popd