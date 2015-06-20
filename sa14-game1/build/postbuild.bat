set build_conf=%1

set output_dir=bin\

call :copyToOutputDir resources

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
