CC=/opt/hisi-linux/x86-arm/arm-hisiv500-linux/bin/arm-hisiv500-linux-uclibcgnueabi-gcc
#DEFS=-DLOG
CFLAGS=-shared -o
CUR=$(shell pwd)
SRC=$(CUR)/*.c
INCLUDES=$(CUR)/../include
LIB=libspi.so
OBJS=spi.o
all:$(LIB)

spi.o : $(SRC)
	$(CC) $(DEFS) -fpic -c $< -o $(OBJS) -I$(INCLUDES) 
		
$(LIB) : $(OBJS)
	$(CC) $(CFLAGS) $@ $(OBJS)
	mv $(CUR)/$(LIB) $(CUR)/../../lib/

clean:
	rm -rf *.o
	rm -rf $(CUR)/../../lib/*.so
