# GLOBAL VARIABLES
CURRENT_DIR := $(shell pwd)
TARGET_ARCH := arm
KERNEL_DIR := $(CURRENT_DIR)/linux-6.6
UBOOT_DIR := $(CURRENT_DIR)/u-boot-2024.10
BUSYBOX_DIR := $(CURRENT_DIR)/busybox-1.36.0
ROOT_DIR := $(CURRENT_DIR)/root
APP_DIR := $(CURRENT_DIR)/applications
COMPILER_DIR := $(CURRENT_DIR)/arm-linux-gnueabihf-14.0.0
COMPILER_PREFIX := $(COMPILER_DIR)/bin/arm-linux-gnueabihf-
jN := j4

export KERNEL_DIR BUSYBOX_DIR ROOT_DIR UBOOT_DIR COMPILER_DIR COMPILER_PREFIX

# sudo qemu-system-arm -M vexpress-a9 -m 512M -kernel ./u-boot -drive if=sd,file=../sdcard,format=raw -nographic -no-reboot
# readelf -l app | grep interpreter
# readelf -d app | grep NEEDED
# git restore --source=origin/master myfile.txt

.PHONY: all clean uboot uboot_clean kernel kernel_clean busybox busybox_clean app app_clean\
	root root_clean update_kernel update_app

all: kernel uboot root busybox
root: busybox app

uboot:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(UBOOT_DIR) vexpress_ca9x4_defconfig
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(UBOOT_DIR) -$(jN)

uboot_clean:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(UBOOT_DIR) distclean

kernel:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) vexpress_defconfig
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) zImage -$(jN)
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) modules -$(jN)
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) dtbs -$(jN)

kernel_clean:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) distclean

busybox:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(BUSYBOX_DIR) defconfig
	sed -i 's/# CONFIG_STATIC is not set/CONFIG_STATIC=y/' $(BUSYBOX_DIR)/.config
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(BUSYBOX_DIR) -$(jN)

busybox_clean:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(BUSYBOX_DIR) clean

app:
	make -C $(APP_DIR)
	
app_clean:
	make -C $(APP_DIR) clean

root:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) CONFIG_PREFIX=$(ROOT_DIR) \
		-C $(BUSYBOX_DIR) install
	./mkroot

root_clean:
	sudo rm -rf $(ROOT_DIR)/*

update_kernel:
	rm -f $(KERNEL_DIR)/arch/arm/boot/uImage
	$(UBOOT_DIR)/tools/mkimage -A arm -O linux -T kernel -C none -a 0x60100000 -n pzxkernel \
    	-d $(KERNEL_DIR)/arch/arm/boot/zImage $(KERNEL_DIR)/arch/arm/boot/uImage
	sudo mount -t ext4 $(CURRENT_DIR)/sdcard /mnt/ -o loop
	sudo rm -f /mnt/boot/uImage /mnt/boot/vexpress-v2p-ca9.dtb
	sudo cp $(KERNEL_DIR)/arch/arm/boot/uImage /mnt/boot/ -f
	sudo cp $(KERNEL_DIR)/arch/arm/boot/dts/arm/vexpress-v2p-ca9.dtb /mnt/boot/ -f
	sudo find $(KERNEL_DIR)/drivers -name "*.ko" -exec cp -f {} /mnt/kmodule \;
	sudo umount /mnt/

update_app:
	sudo mount -t ext4 $(CURRENT_DIR)/sdcard /mnt/ -o loop
	sudo cp -f $(APP_DIR)/app /mnt/
	sudo umount /mnt/

clean:
	make uboot_clean
	make kernel_clean
	make busybox_clean
	make root_clean
