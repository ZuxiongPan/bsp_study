# GLOBAL VARIABLES
CURRENT_DIR := $(shell pwd)
TARGET_ARCH := arm
KERNEL_DIR := $(CURRENT_DIR)/linux-5.4.246
UBOOT_DIR := $(CURRENT_DIR)/u-boot-2020.10
BUSYBOX_DIR := $(CURRENT_DIR)/busybox-1.32.0
ROOT_DIR := $(CURRENT_DIR)/root
APPLICATIONS_DIR := $(CURRENT_DIR)/applications
COMPILER_DIR := $(CURRENT_DIR)/arm-linux-gnueabihf-11.3.1
COMPILER_PREFIX := $(COMPILER_DIR)/bin/arm-linux-gnueabihf-
jN := j2

export APP_INSTALL_DIR ?= $(ROOT_DIR)/usr/bin

export KERNEL_DIR BUSYBOX_DIR ROOT_DIR UBOOT_DIR APPLICATIONS_DIR COMPILER_DIR COMPILER_PREFIX

# sudo qemu-system-arm -M vexpress-a9 -m 512M -kernel ./u-boot -sd ../sdcard -nographic -no-reboot
# readelf -l app | grep interpreter
# readelf -d app | grep NEEDED

.PHONY: all clean uboot uboot_clean kernel kernel_clean busybox busybox_clean \
	applications applications_clean root root_clean update_version

all: kernel uboot root busybox applications
root: busybox applications

uboot:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(UBOOT_DIR) vexpress_ca9x4_defconfig
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(UBOOT_DIR) -$(jN)

uboot_clean:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(UBOOT_DIR) clean

kernel:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) vexpress_defconfig
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) zImage -$(jN)
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) modules -$(jN)
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) dtbs -$(jN)

kernel_clean:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(KERNEL_DIR) clean

busybox:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(BUSYBOX_DIR) defconfig
	sed -i 's/# CONFIG_STATIC is not set/CONFIG_STATIC=y/' $(BUSYBOX_DIR)/.config
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(BUSYBOX_DIR) -$(jN)

busybox_clean:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) -C $(BUSYBOX_DIR) clean

applications:
	make -C $(APPLICATIONS_DIR) all

applications_clean:
	make -C $(APPLICATIONS_DIR) clean

root:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(COMPILER_PREFIX) CONFIG_PREFIX=$(ROOT_DIR) \
		-C $(BUSYBOX_DIR) install
	make -C $(APPLICATIONS_DIR) install
	./mkroot

root_clean:
	sudo rm -rf $(ROOT_DIR)/*

update_version:
	rm -f $(KERNEL_DIR)/arch/arm/boot/uImage
	$(UBOOT_DIR)/tools/mkimage -A arm -O linux -T kernel -C none -a 0x66000000 -n pzxkernel \
    	-d $(KERNEL_DIR)/arch/arm/boot/zImage $(KERNEL_DIR)/arch/arm/boot/uImage
	sudo mount -t ext4 $(CURRENT_DIR)/sdcard /mnt/ -o loop
	sudo rm -f /mnt/boot/uImage /mnt/boot/vexpress-v2p-ca9.dtb
	sudo cp $(KERNEL_DIR)/arch/arm/boot/uImage /mnt/boot/ -f
	sudo cp $(KERNEL_DIR)/arch/arm/boot/dts/vexpress-v2p-ca9.dtb /mnt/boot/ -f
	sudo find $(KERNEL_DIR)/drivers -name "*.ko" -exec cp -f {} /mnt/kmodule \;
	sudo cp $(APPLICATIONS_DIR)/source/app /mnt/usr/bin -f
	sudo umount /mnt/

clean:
	make uboot_clean
	make kernel_clean
	make busybox_clean
	make applications_clean
	make root_clean
