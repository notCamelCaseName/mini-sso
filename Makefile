CFLAGS = 
LDFLAGS = -pthread -lpthread

BIN_NAME = prog

main: src/*
	gcc $(CFLAGS) $(LDFLAGS) src/*.c -o $(BIN_NAME)

run: main
	./$(BIN_NAME)

debug: src/*
	gcc $(CFLAGS) $(LDFLAGS) -g src/*.c -o $(BIN_NAME)
	gdb ./$(BIN_NAME)
