CC := $(COMPILER_PREFIX)gcc
CFLAGS := -Wall -O1 -I$(APPLICATIONS_DIR)/include
LDFLAGS :=

export CC CFLAGS LDFLAGS

.PHONY: all install clean

all:
	make -C source all

install:
	make -C source install

clean:
	make -C source clean