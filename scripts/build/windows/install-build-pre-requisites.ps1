function Install-Go {
    $tempDir =  $env:TEMP
    if ($false -eq (Test-Path -Path $tempDir)) {
        Write-Host("Invalid TEMP dir PATH : " + $tempDir + " ") -ForegroundColor Red
        exit
    }

    $tempGo = Join-Path -Path $tempDir -ChildPath "gotemp"
    Write-Host("creating gotemp dir : " + $tempGo + " ")
    New-Item -Path $tempGo -ItemType "directory" -Force -ErrorAction Stop
    if ($false -eq (Test-Path -Path $tempGo)) {
        Write-Host("Invalid tempGo : " + $tempDir + " ") -ForegroundColor Red
        exit
    }

   $url = "https://dl.google.com/go/go1.14.1.windows-amd64.msi"
   $output = Join-Path -Path $tempDir -ChildPath "go1.14.1.windows-amd64.msi"
   Write-Host("downloading go msi into directory path : " + $output + "  ...")
   Invoke-WebRequest -Uri $url -OutFile $output -ErrorAction Stop
   Write-Host("downloading of go msi into directory path : " + $output + "  completed")

   # install go lang
   Write-Host("installing go ...")
   Start-Process msiexec.exe -Wait -ArgumentList '/I ' + $output + '/quiet'
   Write-Host("installing go completed")

   Write-Host "updating PATH variable"
   $GoPath = Join-Path -Path $env:SYSTEMDRIVE -ChildPath "GO"
   $path = $env:PATH + ";=" + $GoPath
   [System.Environment]::SetEnvironmentVariable("PATH", $path, "PROCESS")
   [System.Environment]::SetEnvironmentVariable("PATH", $path, "USER")
}

function Build-Dependencies {
    $tempDir =  $env:TEMP
    if ($false -eq (Test-Path -Path $tempDir)) {
        Write-Host("Invalid TEMP dir PATH : " + $tempDir + " ") -ForegroundColor Red
        exit
    }

    $tempDependencies = Join-Path -Path $tempDir -ChildPath "gcctemp"
    Write-Host("creating temp dir exist: " + $tempDependencies + " ")
    New-Item -Path $tempDependencies -ItemType "directory" -Force -ErrorAction Stop
    if ($false -eq (Test-Path -Path $tempDependencies)) {
        Write-Host("Invalid temp Dir : " + $tempDependencies + " ") -ForegroundColor Red
        exit
    }


    $destinationPath = Join-Path -Path $env:SYSTEMDRIVE -ChildPath "gcc"
    Write-Host("downloading gcc core, runtime and bin utils : " + $destinationPath + "  ...")
    $gccCoreUrl = "http://downloads.sourceforge.net/project/tdm-gcc/TDM-GCC%205%20series/5.1.0-tdm64-1/gcc-5.1.0-tdm64-1-core.zip"
    $gccCorePath =  Join-Path -Path $tempDependencies -ChildPath "gcc.zip"
    Invoke-WebRequest -Uri $gccCoreUrl -OutFile $gccCorePath -ErrorAction Stop
    Expand-Archive -LiteralPath $gccCorePath -DestinationPath $destinationPath -Force

    $gccRuntimeUrl = "http://downloads.sourceforge.net/project/tdm-gcc/MinGW-w64%20runtime/GCC%205%20series/mingw64runtime-v4-git20150618-gcc5-tdm64-1.zip"
    $gccRuntimePath =  Join-Path -Path $tempDependencies -ChildPath "runtime.zip"
    Invoke-WebRequest -Uri $gccRuntimeUrl -OutFile $gccRuntimePath -ErrorAction Stop
    Expand-Archive -LiteralPath $gccRuntimePath -DestinationPath $destinationPath -Force

    $gccBinUtilsUrl = "http://downloads.sourceforge.net/project/tdm-gcc/GNU%20binutils/binutils-2.25-tdm64-1.zip"
    $gccBinUtilsPath =  Join-Path -Path $tempDependencies -ChildPath "binutils.zip"
    Invoke-WebRequest -Uri $gccBinUtilsUrl -OutFile $gccBinUtilsPath -ErrorAction Stop
    Expand-Archive -LiteralPath $gccBinUtilsUrl -DestinationPath $destinationPath -Force
    Write-Host("downloading and extraction of gcc core, runtime and bin utils completed")

    # set gcc environment variable
    $gccBinPath = Join-Path -Path $destinationPath -ChildPath "bin"

    Write-Host "updating PATH variable"
    $gccBinPath = Join-Path -Path $destinationPath -ChildPath "bin"
    $path = $env:PATH + ";=" + $gccBinPath
    [System.Environment]::SetEnvironmentVariable("PATH", $path, "PROCESS")
    [System.Environment]::SetEnvironmentVariable("PATH", $path, "USER")
}

function Install-DotNetCoreSDK() {
    $tempDir =  $env:TEMP
    if ($false -eq (Test-Path -Path $tempDir)) {
        Write-Host("Invalid TEMP dir : " + $tempDir + " ") -ForegroundColor Red
        exit
    }

    $dotNetSdkTemp = Join-Path -Path $tempDir -ChildPath "dotNetSdk"
    Write-Host("creating dotNetSdkTemp dir : " + $dotNetSdkTemp + " ")
    New-Item -Path $dotNetSdkTemp -ItemType "directory" -Force -ErrorAction Stop
    if ($false -eq (Test-Path -Path $dotNetSdkTemp)) {
        Write-Host("Invalid dotNetSdkTemp : " + $tempDir + " ") -ForegroundColor Red
        exit
    }

   $url = "https://download.visualstudio.microsoft.com/download/pr/4e88f517-196e-4b17-a40c-2692c689661d/eed3f5fca28262f764d8b650585a7278/dotnet-sdk-3.1.301-win-x64.exe"
   $output = Join-Path -Path $dotNetSdkTemp -ChildPath "dotnet-sdk-3.1.301-win-x64.exe"

   Write-Host("downloading .net core sdk 3.1: " + $dotNetSdkTemp + "  ...")
   Invoke-WebRequest -Uri $url -OutFile $output -ErrorAction Stop
   Write-Host("downloading .net core sdk 3.1: " + $dotNetSdkTemp + " completed")

   # install dotNet core sdk
   Write-Host("installing .net core sdk 3.1 ...")
   Start-Process msiexec.exe -Wait -ArgumentList '/I ' + $output + '/quiet'
   Write-Host("installing .net core sdk 3.1 completed")
}

function Install-Docker() {
    $tempDir =  $env:TEMP
    if ($false -eq (Test-Path -Path $tempDir)) {
        Write-Host("Invalid TEMP dir PATH : " + $tempDir + " ") -ForegroundColor Red
        exit
    }

    $dockerTemp = Join-Path -Path $tempDir -ChildPath "docker"
    Write-Host("creating docker temp dir : " + $dockerTemp + " ")
    New-Item -Path $dockerTemp -ItemType "directory" -Force -ErrorAction Stop
    if ($false -eq (Test-Path -Path $dockerTemp)) {
        Write-Host("Invalid dockerTemp : " + $tempDir + " ") -ForegroundColor Red
        exit
    }

   $url = "https://download.docker.com/win/stable/Docker%20Desktop%20Installer.exe"
   $output = Join-Path -Path $dockerTemp -ChildPath "docker-desktop-installer.exe"
   Write-Host("downloading docker-desktop-installer: " + $dockerTemp + "  ...")
   Invoke-WebRequest -Uri $url -OutFile $output -ErrorAction Stop
   Write-Host("downloading docker-desktop-installer: " + $dockerTemp + "  completed")

   # install docker
   Write-Host("installing docker for desktop ...")
   Start-Process msiexec.exe -Wait -ArgumentList '/I ' + $output + '/quiet'
   Write-Host("installing docker for desktop completed")
}

Write-Host "Install GO 1.14.1 version"
Install-Go
Write-Host "Install Build dependencies"
Build-Dependencies

Write-Host "Install .NET core sdk 3.1"
Install-DotNetCoreSDK

Write-Host "Install Docker"
Install-Docker

Write-Host "successfully installed required pre-requisites" -ForegroundColor Green
