set build_conf=%1

set output_dir=bin\

call :copyToOutputDir shaders

goto :eof

:: -----------------------------------------------
:copyToOutputDir
    if exist %1\nul (
        xcopy "%1" "%output_dir%" /cey
    ) else (
        copy "%1" "%output_dir%"
    )
    exit /b
:: -----------------------------------------------
