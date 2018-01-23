@echo off

set MATLAB=E:\Program Files\MATLAB\R2016a

cd .

if "%1"=="" (E:\PROGRA~1\MATLAB\R2016a\bin\win64\gmake -f Gauss_projection_Positive_rtw.mk all) else (E:\PROGRA~1\MATLAB\R2016a\bin\win64\gmake -f Gauss_projection_Positive_rtw.mk %1)
@if errorlevel 1 goto error_exit

exit /B 0

:error_exit
echo The make command returned an error of %errorlevel%
An_error_occurred_during_the_call_to_make
