TARGET := app

.PHONY: all install clean

all: $(TARGET)

$(TARGET): test.o
	$(CC) $< $(LDFLAGS) -o $@
	
test.o: test.c
	$(CC) -c $< $(CFLAGS) -o $@

install: $(TARGET)
	cp $(TARGET) $(APP_INSTALL_DIR) -f
	
clean:
	rm -f *.o $(TARGET)
