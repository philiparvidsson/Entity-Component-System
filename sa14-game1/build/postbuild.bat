set BuildConf=%1
set BinDir=bin\
set PakPw=n3m3s1s!

:: We leave the .pak-files unencrypted in debug build mode.
if %BuildConf%==Debug set PakPw=

call :IncrementBuildNum "source\buildnum.h" BuildNum
call :CopyToBinDir doc
call :CreatePak "%PakPw%" "resources" "%BinDir%data.pak"

goto :eof

:: -----------------------------------------------
:IncrementBuildNum
    set /p DefBuildNum=<%1
    for /f "tokens=3" %%a in ("%DefBuildNum%") do set BuildNum=%%a
    set /a BuildNum+=1
    set DefBuildNum=#define %2 %BuildNum%
    (echo %DefBuildNum%)>%1
    (echo // do not modify! managed by build/postbuild.bat)>>%1
    exit /b
:: -----------------------------------------------

:: -----------------------------------------------
:CopyToBinDir
    if exist %1\nul (
        xcopy "%1" "%BinDir%\%1\" /c /e /y
    ) else (
        copy "%1" "%BinDir%"
    )
    exit /b
:: -----------------------------------------------

:: -----------------------------------------------
:CreatePak
echo %1
echo %2
echo %3
    build\pak-tool -p -k %1 %2 %3
:: -----------------------------------------------
    exit /b
