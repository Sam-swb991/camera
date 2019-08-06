CC= /opt/hisi-linux/x86-arm/arm-hisiv500-linux/bin/arm-hisiv500-linux-uclibcgnueabi-gcc
CXX = /opt/hisi-linux/x86-arm/arm-hisiv500-linux/bin/arm-hisiv500-linux-uclibcgnueabi-g++
CFLAGS = -g -O2 
CXXFLAG =  -O2 -Wall 
VPATH = . 
DIRS=$(VPATH)


INC := $(INC) 


LDFLAGS := $(CFLAGS) -D_LINUX_OS_ 

CPP_SRCS = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
C_SRCS = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
OBJS = $(patsubst %.cpp,%.o,$(CPP_SRCS)) $(patsubst %.c,%.o,$(C_SRCS)) $(patsubst %.cc,%.o,$(CC_SRCS))


TARGET = libsqlite.so

all: $(TARGET)

$(TARGET):$(OBJS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)
	mv $(TARGET) ../../lib

%.o:%.cpp
	$(CXX) $(INC) $(CXXFLAG) -c -o $@ $< $(LDFLAGS)
%.o:%.cc
	$(CXX) $(INC) $(CXXFLAG) -c -o $@ $< $(LDFLAGS)
%.o:%.c
	$(CC) $(INC) $(CXXFLAG) -c -o $@ $< $(LDFLAGS)
clean:
	rm -f $(OBJS)
	rm -f ../../$(TARGET)
        
        
