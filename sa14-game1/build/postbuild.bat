set build_conf=%1
set bin_dir=bin\
set pak_pw=n3m3s1s!

if %build_conf%==Debug set pak_pw=

call :copyToBinDir doc

if not exist %bin_dir%\pak\nul mkdir %bin_dir%\pak

build\pak-tool -p -k "%pak_pw%" "resources\fonts" "%bin_dir%pak\fonts.pak"
build\pak-tool -p -k "%pak_pw%" "resources\images" "%bin_dir%pak\images.pak"
build\pak-tool -p -k "%pak_pw%" "resources\shaders" "%bin_dir%pak\shaders.pak"

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
