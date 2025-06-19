@echo off
REM Windows Test Runner for BIP39 Wallet Generator
REM Usage: run_tests.bat [--basic|--all]

setlocal EnableDelayedExpansion

REM Colors (if supported)
set GREEN=[92m
set RED=[91m
set YELLOW=[93m
set BLUE=[94m
set NC=[0m

REM Test counters
set TOTAL_TESTS=0
set PASSED_TESTS=0
set FAILED_TESTS=0

echo.
echo %BLUE%==================================================%NC%
echo %BLUE%Windows BIP39 Wallet Generator Test Runner%NC%
echo %BLUE%==================================================%NC%
echo.

REM Check if wallet_generator.exe exists
if not exist "wallet_generator.exe" (
    if not exist "build\Release\wallet_generator.exe" (
        echo %RED%Error: wallet_generator.exe not found%NC%
        echo Please build the project first:
        echo   mkdir build ^&^& cd build
        echo   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
        echo   cmake --build . --config Release
        pause
        exit /b 1
    ) else (
        set WALLET_EXE=build\Release\wallet_generator.exe
    )
) else (
    set WALLET_EXE=wallet_generator.exe
)

echo Using executable: !WALLET_EXE!
echo.

REM Function to run a test
:run_test
set TEST_NAME=%~1
set TEST_CMD=%~2
set /a TOTAL_TESTS+=1

echo %YELLOW%Running: %TEST_NAME%%NC%
echo Command: %TEST_CMD%
echo.

REM Run the command and capture exit code
%TEST_CMD% >nul 2>&1
if !errorlevel! equ 0 (
    echo %GREEN%✓ %TEST_NAME% PASSED%NC%
    set /a PASSED_TESTS+=1
) else (
    echo %RED%✗ %TEST_NAME% FAILED (exit code: !errorlevel!)%NC%
    set /a FAILED_TESTS+=1
)
echo.
goto :eof

REM Check command line argument
if "%1"=="--all" goto :run_all_tests
if "%1"=="--basic" goto :run_basic_tests
if "%1"=="" goto :run_basic_tests
goto :show_usage

:run_basic_tests
echo %BLUE%Running Basic Windows Tests%NC%
echo.

call :run_test "Basic Bitcoin Wallet" "!WALLET_EXE! \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""
call :run_test "Ethereum Wallet" "!WALLET_EXE! -n ethereum \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""
call :run_test "Multiple Wallets" "!WALLET_EXE! -n bitcoin -c 3 \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""
call :run_test "Custom Path" "!WALLET_EXE! -n bitcoin -p \"m/44'/0'/0'/0/5\" \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""
call :run_test "With Passphrase" "!WALLET_EXE! -P \"test\" \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""
call :run_test "Verbose Output" "!WALLET_EXE! -v \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""

goto :show_results

:run_all_tests
echo %BLUE%Running Comprehensive Windows Tests%NC%
echo.

REM Basic functionality tests
call :run_test "Help Command" "!WALLET_EXE! -h"
call :run_test "Bitcoin Wallet" "!WALLET_EXE! \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""
call :run_test "Ethereum Wallet" "!WALLET_EXE! -n ethereum \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""
call :run_test "Binance Wallet" "!WALLET_EXE! -n binance \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""
call :run_test "Litecoin Wallet" "!WALLET_EXE! -n litecoin \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""
call :run_test "Dogecoin Wallet" "!WALLET_EXE! -n dogecoin \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""

REM Advanced functionality
call :run_test "Multiple Wallets" "!WALLET_EXE! -n ethereum -c 5 \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""
call :run_test "All Networks" "!WALLET_EXE! -a \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""
call :run_test "Custom Derivation" "!WALLET_EXE! -n bitcoin -p \"m/44'/0'/0'/0/10\" \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""
call :run_test "With Passphrase" "!WALLET_EXE! -P \"TREZOR\" \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""
call :run_test "Verbose Mode" "!WALLET_EXE! -v -n ethereum \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""

REM Error handling tests
echo %BLUE%Testing Error Handling%NC%
echo.
!WALLET_EXE! -n invalid_network "test" >nul 2>&1
if !errorlevel! neq 0 (
    echo %GREEN%✓ Invalid Network Test PASSED (correctly failed)%NC%
    set /a PASSED_TESTS+=1
) else (
    echo %RED%✗ Invalid Network Test FAILED (should have failed)%NC%
    set /a FAILED_TESTS+=1
)
set /a TOTAL_TESTS+=1

!WALLET_EXE! >nul 2>&1
if !errorlevel! neq 0 (
    echo %GREEN%✓ Missing Mnemonic Test PASSED (correctly failed)%NC%
    set /a PASSED_TESTS+=1
) else (
    echo %RED%✗ Missing Mnemonic Test FAILED (should have failed)%NC%
    set /a FAILED_TESTS+=1
)
set /a TOTAL_TESTS+=1

goto :show_results

:show_results
echo.
echo %BLUE%==================================================%NC%
echo %BLUE%Test Results Summary%NC%
echo %BLUE%==================================================%NC%
echo.
echo Total Tests: !TOTAL_TESTS!
if !PASSED_TESTS! gtr 0 (
    echo %GREEN%Passed: !PASSED_TESTS!%NC%
)
if !FAILED_TESTS! gtr 0 (
    echo %RED%Failed: !FAILED_TESTS!%NC%
)

set /a SUCCESS_RATE=!PASSED_TESTS! * 100 / !TOTAL_TESTS!
if !FAILED_TESTS! equ 0 (
    echo %GREEN%Success Rate: !SUCCESS_RATE!%% - ALL TESTS PASSED! 🎉%NC%
    exit /b 0
) else (
    echo %RED%Success Rate: !SUCCESS_RATE!%% - Some tests failed ❌%NC%
    exit /b 1
)

:show_usage
echo Usage: %0 [--basic^|--all]
echo.
echo Options:
echo   --basic    Run basic functionality tests (default)
echo   --all      Run comprehensive test suite
echo.
echo Examples:
echo   %0           # Run basic tests
echo   %0 --basic   # Run basic tests
echo   %0 --all     # Run all tests
echo.
pause
exit /b 0