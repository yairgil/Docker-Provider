FROM mcr.microsoft.com/powershell:latest

COPY log-lines-to-write.txt /log-lines-to-write.txt
COPY print-lines.ps1 /print-lines.ps1

ENTRYPOINT ["pwsh", "-c", "/print-lines.ps1"]
