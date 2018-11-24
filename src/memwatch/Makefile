CC= /opt/hisi-linux/x86-arm/arm-hisiv500-linux/bin/arm-hisiv500-linux-uclibcgnueabi-gcc
TARGET = libmemwatch.so
$(TARGET):memwatch.c
	$(CC) -DMEMWATCH -DMW_STDIO -shared -o $@ $^
