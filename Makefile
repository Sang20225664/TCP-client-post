all: server client

server: TCP_Server/server.c \
#         TCP_Server/message/message.c
	cc $^ -o server

client: TCP_Client/client.c \
#         TCP_Client/message/message.c
	cc $^ -o client

clean:
	rm -f server client
