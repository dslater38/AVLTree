
TARGET=AVLTree$(EXT)
CC=cl

$(TARGET) : AVLTree.cpp main.cpp AVLTree.h
	$(CC) -o $(TARGET) $(CFLAGS) AVLTree.cpp main.cpp