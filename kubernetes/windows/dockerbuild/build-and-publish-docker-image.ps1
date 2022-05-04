<#
    .DESCRIPTION
     Builds the Windows Agent code and Docker Image and pushes the docker image to specified repo

    .PARAMETER image
        docker image. format should be <repo>/<image-name>:<tag>
#>
param(
    [Parameter(mandatory = $true)]
    [string]$image,
    [Parameter(mandatory = $true)]
    [bool]$multiArc= $true
)

$currentdir =  $PSScriptRoot
Write-Host("current script dir : " + $currentdir + " ")

if ($false -eq (Test-Path -Path $currentdir)) {
    Write-Host("Invalid current dir : " + $currentdir + " ") -ForegroundColor Red
    exit 1
}

if ([string]::IsNullOrEmpty($image)) {
    Write-Host "Image parameter shouldnt be null or empty" -ForegroundColor Red
    exit 1
}

$imageparts = $image.split(":")
if (($imageparts.Length -ne 2)){
    Write-Host "Image not in valid format. Expected format should be <repo>/<image>:<imagetag>" -ForegroundColor Red
    exit 1
}

$imagetag = $imageparts[1].ToLower()
$imagerepo = $imageparts[0]

if ($imagetag.StartsWith("win-") -eq $false)
{
    Write-Host "adding win- prefix image tag since its not provided"
    $imagetag = "win-$imagetag"
}

Write-Host "image tag used is :$imagetag"

Write-Host "start:Building the cert generator and out oms code via Makefile.ps1"
..\..\..\build\windows\Makefile.ps1
Write-Host "end:Building the cert generator and out oms code via Makefile.ps1"

$dockerFileDir = Split-Path -Path $currentdir
Write-Host("builddir dir : " + $dockerFileDir + " ")
if ($false -eq (Test-Path -Path $dockerFileDir)) {
    Write-Host("Invalid dockerFile Dir : " + $dockerFileDir + " ") -ForegroundColor Red
    exit 1
}

Write-Host "changing directory to DockerFile dir: $dockerFileDir"
Set-Location -Path $dockerFileDir

$updateImage = ${imagerepo} + ":" + ${imageTag}
if ($multiArc) {
    Write-Host "START:Triggering multi-arc docker image build for ltsc2019 & ltsc2022: $image"
    Write-Host "START:Triggering docker image build for ltsc2019: $image"
    $WINDOWS_VERSION="ltsc2019"
    $updateImageLTSC2019 = ${imagerepo} + ":" + ${imageTag} + "-" + ${WINDOWS_VERSION}
    docker build --isolation=hyperv -t $updateImageLTSC2019  --build-arg WINDOWS_VERSION=$WINDOWS_VERSION IMAGE_TAG=$imageTag  .
    Write-Host "END:Triggering docker image build for ltsc2019: $image"

    Write-Host "START:Triggering docker image build for ltsc2022: $image"
    $WINDOWS_VERSION="ltsc2022"
    $updateImageLTSC2022 = ${imagerepo} + ":" + ${imageTag} + "-" + ${WINDOWS_VERSION}
    docker build --isolation=hyperv -t $updateImageLTSC2022  --build-arg WINDOWS_VERSION=$WINDOWS_VERSION IMAGE_TAG=$imageTag  .
    Write-Host "END:Triggering docker image build for ltsc2022: $image"

    Write-Host "START:pushing docker image with base image ltsc2019 : $updateImageLTSC2019"
    docker push  $updateImageLTSC2019
    Write-Host "END:pushing docker image : $updateImageLTSC2019"

    Write-Host "START:pushing docker image with base image ltsc2022 : $updateImageLTSC2022"
    docker push  $updateImageLTSC2022
    Write-Host "END:pushing docker image : $updateImageLTSC2022"

    Write-Host "START:Triggering manigest for multi-arc docker image: $updateImage"
    docker manifest create $updateImage $updateImageLTSC2019 $updateImageLTSC2022
    docker manifest push $updateImage
    Write-Host "END:Triggering manifest for multi-arc docker image: $updateImage"

    Write-Host "END:Triggering multi-arc docker image build for ltsc2019 & ltsc2022: $image"
} else {

    Write-Host "START:Triggering docker image build: $image"
    docker build -t $updateImage  --build-arg IMAGE_TAG=$imageTag  .
    Write-Host "END:Triggering docker image build: $updateImage"

    Write-Host "START:pushing docker image : $updateImage"
    docker push  $updateImage
    Write-Host "END:pushing docker image : $updateImage"
}

