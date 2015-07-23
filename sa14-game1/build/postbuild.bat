set build_conf=%1

set bin_dir=bin\

set build_num_file=source\base\buildnum.h
set build_num_constant=BuildNum

set pak_pw=n3m3s1s!
if %build_conf%==Debug set pak_pw=

call :incrementBuildNum

call :copyToBinDir doc

build\pak-tool -p -k "%pak_pw%" "resources" "%bin_dir%data.pak"

goto :eof

:: -----------------------------------------------
:copyToBinDir
    if exist %1\nul (
        xcopy "%1" "%bin_dir%\%1\" /c /e /y
    ) else (
        copy "%1" "%bin_dir%"
    )
    exit /b
:: -----------------------------------------------


:: Increment build number in file 'buildnum.h'.
:: -----------------------------------------------
:incrementBuildNum
    set /p def_build_num=<%build_num_file%
    for /f "tokens=3" %%a in ("%def_build_num%") do set build_num=%%a
    set /a build_num+=1
    set def_build_num=#define %build_num_constant% %build_num%
    (echo %def_build_num%)>%build_num_file%
    (echo // do not modify! managed by build/postbuild.bat)>>%build_num_file%
    exit /b
:: -----------------------------------------------
