#  Build script to build the .NET and Go lang code for the Windows agent.
#  It does following  tasks
#  1. Builds the certificate generator code in .NET and copy the binaries in zip file to ..\..\kubernetes\windows\omsagentwindows
#  2. Builds the out_oms plugin code in go lang  into the shared object(.so) file and copy the out_oms.so file  to ..\..\kubernetes\windows\omsagentwindows
#  3. copy the files under installer directory to ..\..\kubernetes\windows\omsagentwindows

$dotnetcoreframework = "netcoreapp2.2"

Write-Host("Building Certificate generator code...")
$currentdir =  $PWD
if ($false -eq (Test-Path -Path $currentdir)) {
    Write-Host("Invalid current dir : " + $currentdir + " ") -ForegroundColor Red
    exit
}

$builddir = Split-Path -Path $currentdir
if ($false -eq (Test-Path -Path $builddir)) {
    Write-Host("Invalid build dir : " + $builddir + " ") -ForegroundColor Red
    exit
}
$srcdir = Split-Path -Path $builddir
if ($false -eq (Test-Path -Path $srcdir)) {
    Write-Host("Invalid docker provider root source dir : " + $srcdir + " ") -ForegroundColor Red
    exit
}
$certsrcdir = Join-Path -Path $srcdir -ChildPath "kubernetes\windows\certificategenerator"
if ($false -eq (Test-Path -Path $certsrcdir)) {
    Write-Host("Invalid certificate generator source dir : " + $srcdir + " ") -ForegroundColor Red
    exit
}
Write-Host("set the cerificate generator source code director : " + $certsrcdir + " ...")
Set-Location -Path $certsrcdir

Write-Host("Adding dotnet packages Newtonsoft.json and BouncyCastle ...")
dotnet add package Newtonsoft.json
dotnet add package BouncyCastle
Write-Host("Successfully added dotnet packages") -ForegroundColor Green
dotnet build  -f $dotnetcoreframework
Write-Host("Building Certificate generator code and ...") -ForegroundColor Green

Write-Host("Publish release and win10-x64 binaries of certificate generator code  ...")
dotnet publish -c Release -r win10-x64
Write-Host("Successfully published certificate generator code binaries") -ForegroundColor Green

$certreleasebinpath =  Join-Path -PATH $certsrcdir -ChildPath "bin\Release\$dotnetcoreframework\win10-x64\publish\*.*"
if ($false -eq (Test-Path -Path $certreleasebinpath)) {
    Write-Host("certificate release bin path doesnt exist : " + $certreleasebinpath + " ") -ForegroundColor Red
    exit
}
$publishdir = Join-Path -Path $srcdir -ChildPath "kubernetes\windows\omsagentwindows"
if ($true -eq (Test-Path -Path $publishdir)) {
    Write-Host("publish dir exist hence deleting: " + $publishdir + " ")
    Remove-Item -Path $publishdir  -Recurse -Force
}
Write-Host("creating publish dir exist: " + $publishdir + " ")
New-Item -Path $publishdir -ItemType "directory" -Force

$certreleasepublishpath = Join-Path  -Path $publishdir -ChildPath "certificategenerator.zip"

Write-Host("Compressing and copying the certificate generator release binaries ...")
Compress-Archive -Path  $certreleasebinpath -DestinationPath $certreleasepublishpath  -Force
Write-Host("Successfully copied compressed certificate generator release binaries") -ForegroundColor Green

# build  the shared object (.so) for out oms go plugin code
$outomsgoplugindir = Join-Path -Path $srcdir -ChildPath "source\code\go\src\plugins"
Write-Host("Building Out_OMS go plugin code...")
if ($false -eq (Test-Path -Path $outomsgoplugindir)) {
    Write-Host("Invalid Out oms go plugin code dir : " + $outomsgoplugindir + " ") -ForegroundColor Red
    exit
}
Set-Location -Path $outomsgoplugindir
Write-Host("getting latest go modules ...")
go get
Write-Host("successfyullt got latest go modules") -ForegroundColor Green
go build -o out_oms.so .
Write-Host("Successfully build Out_OMS go plugin code") -ForegroundColor Green

Write-Host("copying out_oms.so file to : $publishdir")
Copy-Item -Path (Join-path -Path $outomsgoplugindir -ChildPath "out_oms.so")  -Destination $publishdir -Force
Write-Host("successfully copied out_oms.so file to : $publishdir") -ForegroundColor Green
Set-Location $currentdir

$installerdir = Join-Path -Path $builddir -ChildPath "windows\installer"
Write-Host("copying installer files conf and scripts from :" + $installerdir + "  to  :" + $publishdir + " ...")
Copy-Item  -Path $installerdir  -Destination $publishdir -Recurse -Force
Write-Host("successfully copied installer files conf and scripts from :" + $installerdir + "  to  :" + $publishdir + " ") -ForegroundColor Green
