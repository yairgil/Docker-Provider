echo "Finishing up with main.ps1..."
echo "Adding in a second line"

notepad.exe


@REM C:\opt\fluent-bit\bin\fluent-bit.exe -c "C:\etc\fluent-bit\fluent-bit.conf" -e "C:\opt\omsagentwindows\out_oms.so"

@REM fluentd --reg-winsvc i --reg-winsvc-auto-start --winsvc-name fluentdwinaks --reg-winsvc-fluentdopt '-c C:/etc/fluent/fluent.conf -o C:/etc/fluent/fluent.log'