
TARGET=AVLTree$(EXT)
CC=clang++

$(TARGET) : AVLTree.cpp main.cpp AVLTree.h
	$(CC) -o $(TARGET) $(CFLAGS) AVLTree.cpp main.cpp