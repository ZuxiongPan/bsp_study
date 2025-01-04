# GLOBAL VARIABLES
CURRENT_DIR := $(shell pwd)
TARGET_ARCH := arm
KERNEL_DIR := $(CURRENT_DIR)/linux-6.6
BUSYBOX_DIR := $(CURRENT_DIR)/busybox-1.36.0
ROOT_DIR := $(CURRENT_DIR)/root
APP_DIR := $(CURRENT_DIR)/applications
COMPILER_DIR := $(CURRENT_DIR)/arm-linux-gnueabihf-14.0.0
COMPILER_PREFIX := $(COMPILER_DIR)/bin/arm-linux-gnueabihf-
jN := j4

export KERNEL_DIR BUSYBOX_DIR ROOT_DIR COMPILER_DIR COMPILER_PREFIX

# qemu-system-arm -M bpim2u -nographic -kernel ./zImage -dtb ./sun8i-r40-bananapi-m2-ultra.dtb -drive if=sd,file=./sdcard,format=raw
# readelf -l app | grep interpreter
# readelf -d app | grep NEEDED
# git restore --source=origin/master myfile.txt

.PHONY: all clean kernel kernel_clean busybox busybox_clean app app_clean\
	root root_clean update_app

all: kernel root busybox

kernel:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) sunxi_defconfig
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) zImage -$(jN)
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) modules -$(jN)
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) dtbs -$(jN)
	cp $(KERNEL_DIR)/arch/arm/boot/zImage $(CURRENT_DIR) -f
	cp $(KERNEL_DIR)/arch/arm/boot/dts/allwinner/sun8i-r40-bananapi-m2-ultra.dtb $(CURRENT_DIR) -f

kernel_clean:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) distclean

busybox:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(BUSYBOX_DIR) defconfig
	sed -i 's/# CONFIG_STATIC is not set/CONFIG_STATIC=y/' $(BUSYBOX_DIR)/.config
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(BUSYBOX_DIR) -$(jN)
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) CONFIG_PREFIX=$(ROOT_DIR) \
		-C $(BUSYBOX_DIR) install

busybox_clean:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(BUSYBOX_DIR) clean

app:
	make -C $(APP_DIR)
	make -C $(APP_DIR) install
	
app_clean:
	make -C $(APP_DIR) clean

root:
	./mkroot

root_clean:
	sudo rm -rf $(ROOT_DIR)/*
	rm -f $(CURRENT_DIR)/*Image $(CURRENT_DIR)/*.dtb

update_app:
	sudo mount -t ext4 $(CURRENT_DIR)/sdcard /mnt/ -o loop
	sudo cp -f $(APP_DIR)/app /mnt/
	sudo umount /mnt/

clean:
	make kernel_clean
	make busybox_clean
	make root_clean
