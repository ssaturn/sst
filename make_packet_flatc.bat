flatc.exe -c --no-prefix ./fbs/packet.fbs
flatc.exe -c --no-prefix ./fbs/protocol.fbs
flatc.exe -n ./fbs/packet.fbs
flatc.exe -n ./fbs/protocol.fbs
pause
