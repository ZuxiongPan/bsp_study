CC := $(COMPILER_PREFIX)gcc
CFLAGS := -I$(BSP_INCDIR)
LDFLAGS :=

export CC CFLAGS LDFLAGS

dirs-y += appctl/

.PHONY: all install update clean

all:
	for dir in $(dirs-y); do \
	make -C $$dir; \
	done
	
install:
	for dir in $(dirs-y); do \
	make -C $$dir install; \
	done

update:
	for dir in $(dirs-y); do \
	make -C $$dir update; \
	done

clean:
	for dir in $(dirs-y); do \
	make -C $$dir clean; \
	done
