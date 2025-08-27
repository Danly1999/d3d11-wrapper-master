@echo off
set CERT_NAME="My Test Certificate"
set PASSWORD="mypassword123"
set DLL_PATH="d3d11.dll"

echo Creating root certificate...
makecert -r -pe -n "CN=%CERT_NAME% Root" -ss root -sr LocalMachine -a sha256 -cy authority -sv root.pvk root.cer

echo Creating code signing certificate...
makecert -pe -n "CN=%CERT_NAME% Code Signing" -ss my -sr LocalMachine -a sha256 -cy end -sky signature -ic root.cer -iv root.pvk -sv code.pvk code.cer

echo Creating PFX file...
pvk2pfx -pvk code.pvk -spc code.cer -pfx code.pfx -po %PASSWORD%

echo Installing certificates...
certutil -addstore -f "Root" root.cer
certutil -addstore -f "My" code.cer

echo Signing DLL...
signtool sign /f code.pfx /p mypassword123 /fd SHA256 /tr http://timestamp.digicert.com /td SHA256 "d3d11.dll"

echo Verification...
signtool verify /v /pa %DLL_PATH%

echo Done!
pause