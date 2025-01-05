TARGET := appctl
RELY := appctl.o

.PHONY: all install update clean

all: $(RELY)
	$(CC) $(RELY) -o $(TARGET)
	
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)
	
install:
	cp -f $(TARGET) $(ROOT_DIR)/bin/
	
update:
	sudo cp -f $(TARGET) /mnt/bin/
	
clean:
	rm -f $(RELY) $(TARGET)