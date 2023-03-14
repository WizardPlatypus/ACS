g++ server.cpp -o server.exe -lWs2_32
g++ client.cpp -o client.exe -lWs2_32 -lMswsock -lAdvApi32