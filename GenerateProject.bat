@echo off

@REM set argC=0
@REM for %%x in (%*) do Set /A argC+=1

set architecture=x64
set configuration=Debug
FOR %%A IN (%*) DO (
	IF "%%A"=="Win32" (
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

call cmake .. -DCMAKE_BUILD_TYPE=%configuration% -A %architecture% -DGLFW_INSTALL=0 -DGLFW_BUILD_DOCS=0

echo.
echo Cmake finished.

cd ..

PAUSE