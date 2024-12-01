TARGET := app

.PHONY: all install clean

all: $(TARGET)

$(TARGET): app.o
	$(CC) $< $(LDFLAGS) -o $@
	
app.o: app.c
	$(CC) -c $< $(CFLAGS) -o $@

install: $(TARGET)
	cp $(TARGET) $(APP_INSTALL_DIR) -f
	
clean:
	rm -f *.o $(TARGET)
