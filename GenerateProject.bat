@echo off

@REM set argC=0
@REM for %%x in (%*) do Set /A argC+=1

set architecture=x64
set configuration=Debug
FOR %%A IN (%*) DO (
	IF "%%A"=="x86" (
		set architecture=%%A
		echo Setting Build Architecture to "%%A".
	)

	IF "%%A"=="x64" (
		set architecture=%%A
		echo Setting Build Architecture to "%%A".
	)


	IF "%%A"=="Debug" (
		set configuration=%%A
		echo Setting Build Configuration to "%%A".
	)

	IF "%%A"=="Release" (
		set configuration=%%A
		echo Setting Build Configuration to "%%A".
	)

	IF "%%A"=="RelWithDebInfo" (
		set configuration=%%A
		echo Setting Build Configuration to "%%A".
	)
)

echo Selecting Build Settings "%configuration%" and "%architecture%".
set buildPath=build-%configuration%-Windows-%architecture%


IF NOT EXIST %~dp0\%buildPath% (
	echo Build Directory %buildPath% does not exist, creating it...
	mkdir %buildPath%
) ELSE (
	echo Build Directory %~dp0%buildPath% found.
)

cd %buildPath%

echo Starting CMake...
echo.

IF %architecture%==x64 (
	call cmake .. -DCMAKE_BUILD_TYPE=%configuration% -A x64 -DRL_ARCHITECTURE=%architecture%
)
IF %architecture%==x86 (
	call cmake .. -DCMAKE_BUILD_TYPE=%configuration% -A Win32 -DRL_ARCHITECTURE=%architecture%
)

echo.
echo Cmake finished.

cd ..

PAUSE
