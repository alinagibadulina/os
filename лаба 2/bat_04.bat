@echo off
set /a a=%1, b=%2, c=%3

echo a=%a%, b=%b%, c=%c%

set /a r1=a-b, r2=a+b, r3=c/b, r4=c*b, r5=(a-b)*(b-a)

echo a-b=%r1%
echo a+b=%r2%
echo c/b=%r3%
echo c*b=%r4%
echo (a-b)*(b-a)=%r5%
