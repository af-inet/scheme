TARGET  = skeme
CC      = gcc
CFLAGS  = -Wall -I. -Wno-initializer-overrides
OBJ     = main.o token.o
HEADERS =        token.h

# where plexer will be installed
INSTALL_DEST = /usr/local/bin

.PHONY: default all clean install uninstall

default: all

all: $(TARGET) $(OBJ)

install: $(TARGET)
	chmod 755 ./$(TARGET)
	cp ./$(TARGET) $(INSTALL_DEST)/$(TARGET)

uninstall:
	rm $(INSTALL_DEST)/$(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $@

debug: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -g -o $(TARGET)-debug

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o
	$(RM) $(TARGET)-debug
	$(RM) -r $(TARGET)-debug.dSYM
	$(RM) $(TARGET)

