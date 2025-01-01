CC := $(COMPILER_PREFIX)gcc
TARGET := app

.PHONY: all install clean

all: $(TARGET) install
install: $(TARGET)

$(TARGET): app.o
	$(CC) $< -o $@

%.o: %.c 
	$(CC) -c $< -o $@
	
install:
	cp -f $(TARGET) $(ROOT_DIR)/
	
clean:
	rm -f *.o $(TARGET)