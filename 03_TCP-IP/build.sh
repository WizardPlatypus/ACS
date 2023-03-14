g++ server.cpp shared.cpp -o server.exe -lWs2_32
g++ client.cpp shared.cpp -o client.exe -lWs2_32 -lMswsock -lAdvApi32