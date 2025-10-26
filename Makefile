CC = cc
CFLAGS = -Wall -g

# === DEFAULT TARGET ===
all: server client

# === SERVER ===
SERVER_SRC = \
    TCP_Server/server.c \
    TCP_Server/protocol/protocol.c \
    TCP_Server/auth/auth.c \
    TCP_Server/user/user.c
SERVER_OBJ = $(SERVER_SRC:.c=.o)

server: $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(SERVER_OBJ) -o server

# === CLIENT ===
CLIENT_SRC = \
    TCP_Client/client.c \
    TCP_Client/protocol/protocol.c
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)

client: $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(CLIENT_OBJ) -o client

clean:
	rm -f $(SERVER_OBJ) $(CLIENT_OBJ) server client
