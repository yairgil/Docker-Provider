echo "START:Adding and restoring dotnet packages"
cd /D "%~dp0..\build\windows\installer\certificategenerator"
dotnet add package Newtonsoft.json
dotnet add package BouncyCastle
dotnet restore CertificateGenerator.csproj
dotnet publish -c Release -r win10-x64
echo "COMPLETE:Adding and restoring dotnet packages"

echo "START:Getting go packages"
cd /D "%~dp0..\source\plugins\go\src"
c:\go\bin\go.exe get
echo "COMPLETE:Getting go packages"