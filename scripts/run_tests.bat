@echo off
REM Simple Windows Test Runner for BIP39 Wallet Generator
setlocal EnableDelayedExpansion

echo.
echo ==========================================
echo Windows BIP39 Wallet Generator Test Runner
echo ==========================================
echo.

REM Check if wallet_generator.exe exists
if not exist "wallet_generator.exe" (
    echo Error: wallet_generator.exe not found in current directory
    echo Current directory: %CD%
    dir *.exe
    pause
    exit /b 1
)

echo Using executable: wallet_generator.exe
echo.

REM Test counters
set TOTAL_TESTS=0
set PASSED_TESTS=0

REM Check command line argument
if "%1"=="--all" goto :run_all_tests
if "%1"=="--basic" goto :run_basic_tests
if "%1"=="" goto :run_basic_tests
goto :show_usage

:run_basic_tests
echo Running Basic Windows Tests
echo.

REM Test 1: Basic Bitcoin Wallet
echo Running: Basic Bitcoin Wallet
wallet_generator.exe "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about" >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! equ 0 (
    echo ✓ Basic Bitcoin Wallet PASSED
    set /a PASSED_TESTS+=1
) else (
    echo ✗ Basic Bitcoin Wallet FAILED
)

REM Test 2: Ethereum Wallet
echo Running: Ethereum Wallet
wallet_generator.exe -n ethereum "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about" >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! equ 0 (
    echo ✓ Ethereum Wallet PASSED
    set /a PASSED_TESTS+=1
) else (
    echo ✗ Ethereum Wallet FAILED
)

REM Test 3: Multiple Wallets
echo Running: Multiple Wallets
wallet_generator.exe -n bitcoin -c 3 "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about" >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! equ 0 (
    echo ✓ Multiple Wallets PASSED
    set /a PASSED_TESTS+=1
) else (
    echo ✗ Multiple Wallets FAILED
)

goto :show_results

:run_all_tests
echo Running Comprehensive Windows Tests
echo.

REM Test 1: Help Command
echo Running: Help Command
wallet_generator.exe -h >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! equ 0 (
    echo ✓ Help Command PASSED
    set /a PASSED_TESTS+=1
) else (
    echo ✗ Help Command FAILED
)

REM Test 2: Bitcoin Wallet
echo Running: Bitcoin Wallet
wallet_generator.exe "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about" >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! equ 0 (
    echo ✓ Bitcoin Wallet PASSED
    set /a PASSED_TESTS+=1
) else (
    echo ✗ Bitcoin Wallet FAILED
)

REM Test 3: Ethereum Wallet
echo Running: Ethereum Wallet
wallet_generator.exe -n ethereum "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about" >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! equ 0 (
    echo ✓ Ethereum Wallet PASSED
    set /a PASSED_TESTS+=1
) else (
    echo ✗ Ethereum Wallet FAILED
)

REM Test 4: Binance Wallet
echo Running: Binance Wallet
wallet_generator.exe -n binance "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about" >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! equ 0 (
    echo ✓ Binance Wallet PASSED
    set /a PASSED_TESTS+=1
) else (
    echo ✗ Binance Wallet FAILED
)

REM Test 5: Litecoin Wallet
echo Running: Litecoin Wallet
wallet_generator.exe -n litecoin "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about" >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! equ 0 (
    echo ✓ Litecoin Wallet PASSED
    set /a PASSED_TESTS+=1
) else (
    echo ✗ Litecoin Wallet FAILED
)

REM Test 6: Multiple Wallets
echo Running: Multiple Wallets
wallet_generator.exe -n ethereum -c 5 "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about" >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! equ 0 (
    echo ✓ Multiple Wallets PASSED
    set /a PASSED_TESTS+=1
) else (
    echo ✗ Multiple Wallets FAILED
)

REM Test 7: All Networks
echo Running: All Networks
wallet_generator.exe -a "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about" >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! equ 0 (
    echo ✓ All Networks PASSED
    set /a PASSED_TESTS+=1
) else (
    echo ✗ All Networks FAILED
)

REM Test 8: Custom Derivation
echo Running: Custom Derivation
wallet_generator.exe -n bitcoin -p "m/44'/0'/0'/0/10" "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about" >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! equ 0 (
    echo ✓ Custom Derivation PASSED
    set /a PASSED_TESTS+=1
) else (
    echo ✗ Custom Derivation FAILED
)

REM Test 9: With Passphrase
echo Running: With Passphrase
wallet_generator.exe -P "TREZOR" "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about" >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! equ 0 (
    echo ✓ With Passphrase PASSED
    set /a PASSED_TESTS+=1
) else (
    echo ✗ With Passphrase FAILED
)

REM Test 10: Verbose Mode
echo Running: Verbose Mode
wallet_generator.exe -v -n ethereum "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about" >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! equ 0 (
    echo ✓ Verbose Mode PASSED
    set /a PASSED_TESTS+=1
) else (
    echo ✗ Verbose Mode FAILED
)

echo.
echo Testing Error Handling
echo.

REM Test 11: Invalid Network (should fail)
echo Running: Invalid Network Test
wallet_generator.exe -n invalid_network "test" >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! neq 0 (
    echo ✓ Invalid Network Test PASSED (correctly failed)
    set /a PASSED_TESTS+=1
) else (
    echo ✗ Invalid Network Test FAILED (should have failed)
)

REM Test 12: Missing Mnemonic (should fail)
echo Running: Missing Mnemonic Test
wallet_generator.exe >nul 2>&1
set /a TOTAL_TESTS+=1
if !errorlevel! neq 0 (
    echo ✓ Missing Mnemonic Test PASSED (correctly failed)
    set /a PASSED_TESTS+=1
) else (
    echo ✗ Missing Mnemonic Test FAILED (should have failed)
)

goto :show_results

:show_results
echo.
echo ==========================================
echo Test Results Summary
echo ==========================================
echo.

REM Calculate failed tests
set /a FAILED_TESTS=!TOTAL_TESTS! - !PASSED_TESTS!

echo Total Tests: !TOTAL_TESTS!
echo Passed: !PASSED_TESTS!
echo Failed: !FAILED_TESTS!

if !TOTAL_TESTS! gtr 0 (
    set /a SUCCESS_RATE=!PASSED_TESTS! * 100 / !TOTAL_TESTS!
) else (
    set SUCCESS_RATE=0
)

if !FAILED_TESTS! equ 0 (
    echo Success Rate: !SUCCESS_RATE!%% - ALL TESTS PASSED!
    exit /b 0
) else (
    echo Success Rate: !SUCCESS_RATE!%% - Some tests failed
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
exit /b 0