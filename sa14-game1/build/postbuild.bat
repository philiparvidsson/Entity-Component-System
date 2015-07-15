set build_conf=%1
set output_dir=bin\
set pak_pw=n3m3s1s!

call :copyToOutputDir doc
build\pak-tool.exe -p -k "%pak_pw%" "resources" "%output_dir%resources.pak"

goto :eof

:: -----------------------------------------------
:copyToOutputDir
    if exist %1\nul (
        xcopy "%1" "%output_dir%\%1\" /c /e /y
    ) else (
        copy "%1" "%output_dir%"
    )
    exit /b
:: -----------------------------------------------
