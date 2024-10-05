@echo off

:: Config
set project_root=%~dp0%
set exe_name=pix16.exe
set build_hash=0

pushd %project_root%
    FOR /F "tokens=*" %%g IN ('more %project_root%\.git\refs\heads\main') do (set build_hash=%%g)

    :: Build executable
    if exist build (rmdir /s /q build)
    mkdir build

    pushd build
        pushd ..\assets
            rc.exe /r -nologo resources.rc
        popd
        
        IF %errorlevel% NEQ 0 (popd && goto end)

        set flags=/Ox -FC -fp:fast -fp:except- /Zc:strictStrings-
        cl.exe -nologo /MD -DDEBUG=0 -DBUILD_HASH="%build_hash%" -DSHIP_MODE=1 /I ..\src %flags% ..\src\pix16_win32.cpp ..\assets\resources.res /link -subsystem:windows -incremental:no -opt:ref -OUT:%exe_name%
        IF %errorlevel% NEQ 0 (popd && goto end)

        :: Cleanup
        del /s /q *.exp *.lib *.obj *.pdb
    popd

    :: Create zip
    tar.exe -a -c -f pix16_win32.zip build/*

:end
popd
exit /B %errorlevel%
