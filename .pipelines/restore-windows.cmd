echo "START:Adding and restoring dotnet packages"
cd /D "%~dp0..\build\windows\installer\certificategenerator"
dotnet add package Newtonsoft.json
dotnet add package BouncyCastle
dotnet restore CertificateGenerator.csproj
dotnet publish -c Release -r win10-x64
echo "END:Adding and restoring dotnet packages"

echo "START:set environment variables"
setx path "%path%;c:\gcc\bin;c:\go\bin"
set PATH="%PATH%;c:\gcc\bin;c:\go\bin"
echo "END:set environment variables"

echo "START:Getting go packages"
cd /D "%~dp0..\source\plugins\go\src"
c:\go\bin\go.exe get
echo "END:Getting go packages"