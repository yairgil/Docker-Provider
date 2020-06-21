<#
    .DESCRIPTION
     Builds the Windows Agent code and Docker Image and pushes the docker image to specified repo

    .PARAMETER image
        docker image. format should be <repo>/<image-name>:<tag>
#>
param(
    [Parameter(mandatory = $true)]
    [string]$image
)

Write-Host "TODO: Implement windows docker image"