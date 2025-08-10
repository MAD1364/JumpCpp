@echo off
echo ========================================
echo Cleaning up project folders...
echo ========================================
echo.
echo About to delete the following folders:
echo - Binaries
echo - Intermediates
echo.

if exist "..\Binaries" (
    echo Deleting Binaries folder...
    rmdir /s /q "..\Binaries"
    echo Binaries folder deleted successfully.
) else (
    echo Binaries folder not found - skipping.
)

if exist "..\Intermediate" (
    echo Deleting Intermediate folder...
    rmdir /s /q "..\Intermediate"
    echo Intermediate folder deleted successfully.
) else (
    echo Intermediate folder not found - skipping.
)

echo.
echo ========================================
echo Cleanup completed!
echo Goodbye!
echo ========================================
pause
