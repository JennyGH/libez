@ECHO OFF
SETLOCAL
SET PROJECT_ROOT=%CD%
CALL:Format "%PROJECT_ROOT%\\src"
CALL:Format "%PROJECT_ROOT%\\tests"
GOTO:EOF

:Format
SET SOURCE_DIR=%1
FOR /r %SOURCE_DIR% %%i IN (*.h) DO (
    @CALL clang-format -i "%%i" --style=file --verbose
)
FOR /r %SOURCE_DIR% %%i IN (*.c) DO (
    @CALL clang-format -i "%%i" --style=file --verbose
)
FOR /r %SOURCE_DIR% %%i IN (*.cpp) DO (
    @CALL clang-format -i "%%i" --style=file --verbose
)
GOTO:EOF

ENDLOCAL