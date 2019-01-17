CC=/opt/hisi-linux/x86-arm/arm-hisiv500-linux/bin/arm-hisiv500-linux-uclibcgnueabi-gcc
CFLAGS=-shared -o
CUR=$(shell pwd)
SRC=$(CUR)/*.c
INCLUDES=$(CUR)
LIB=libcmd5.so
OBJS=md5.o
all:$(LIB)

md5.o : $(SRC)
	$(CC) -fpic -c $< -o $(OBJS) -I$(INCLUDES) 
		
$(LIB) : $(OBJS)
	$(CC) $(CFLAGS) $@ $(OBJS)
	mv $(CUR)/$(LIB) $(CUR)/../../lib/

clean:
	rm -rf *.o
	rm -rf $(CUR)/../../lib/*.so
