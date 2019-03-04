CC=/opt/hisi-linux/x86-arm/arm-hisiv500-linux/bin/arm-hisiv500-linux-uclibcgnueabi-gcc
CPP=/opt/hisi-linux/x86-arm/arm-hisiv500-linux/bin/arm-hisiv500-linux-uclibcgnueabi-g++
CFLAGS= -g -o
PARAM=-Wall -std=c++11 
CUR=$(shell pwd)
SRC=$(CUR)/src
INCLUDES=$(SRC)/include/
SRC_C=$(SRC)/*.c
SRC_P=$(SRC)/*.cpp
LIB_PATH=$(CUR)/lib/
LIBS=-lspi -lCJsonObject -lpthread -lsqlite -ljsoncpp -lcurl 
TARGET=$(CUR)/bin/camera
.PHONY:$(TARGET)
all:$(TARGET)

$(TARGET):$(SRC_P)
	@-mkdir lib;
	@-mkdir bin;
	@-make shared;
	$(CPP) $^ $(CFLAGS) $@ $(PARAM) -L$(LIB_PATH) $(LIBS) -I$(INCLUDES)
	
clean:
	@-rm -rf $(TARGET)
	@-cd $(SRC)/spi;make clean;
	@-cd $(SRC)/CJsonObject;make clean;
	
shared:
	@$(MAKE) -C $(SRC)/spi;
	@$(MAKE) -C $(SRC)/CJsonObject;
	@$(MAKE) -C $(SRC)/sqlite;
	
