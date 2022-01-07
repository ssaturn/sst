protoc-3.7.1-windows-x86_64.exe -I proto --csharp_out=shared.net/packet proto/protocol.proto
protoc-3.7.1-windows-x86_64.exe -I proto --csharp_out=shared.net/packet proto/packet_login.proto
protoc-3.7.1-windows-x86_64.exe -I proto --cpp_out=shared/packet proto/protocol.proto
protoc-3.7.1-windows-x86_64.exe -I proto --cpp_out=shared/packet proto/packet_login.proto