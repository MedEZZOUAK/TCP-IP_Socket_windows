cd client
gcc -o client client.c -lws2_32
cd ..
cd server
gcc -o server server.c -lws2_32