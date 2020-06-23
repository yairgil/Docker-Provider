cd /D "%~dp0..\build\windows\installer\certificategenerator"
dotnet add package Newtonsoft.json
dotnet add package BouncyCastle
dotnet publish -c Release -r win10-x64
dotnet restore CertificateGenerator.csproj || exit /b 1