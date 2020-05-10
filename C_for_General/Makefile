#!/usr/bin/make

TARGET   := build/librs.a
UNITTEST := build/ectest
DEMO     := build/demo

DEFINES  :=
INCLUDE  := -I. -I./include

CC       := gcc
CFLAGS   := -O3 -m64 -ftree-vectorize -funroll-loops -DNDEBUG 

CXX      := g++
CXXFLAGS := -O3 -m64 -ftree-vectorize -funroll-loops -std=c++11 -DNDEBUG 

CPP      := $(CC) -E
CPPFLAGS :=

AR       := ar
ARFLAGS  := rs

LD       := ld
LDFLAGS  :=

RANLIB   := ranlib
SHARE    := -fPIC -shared -o
LIBS     := -pthread

SRC_DIR    := src
C_SOURCES  := $(wildcard $(SRC_DIR)/*.c)
C_OBJS     := $(patsubst %.c,%.o,$(C_SOURCES))

AVX2_SRC_DIR    := src/avx2
AVX2_C_SOURCES  := $(wildcard $(AVX2_SRC_DIR)/*.c)
AVX2_C_OBJS     := $(patsubst %.c,%.o,$(AVX2_C_SOURCES))


AVX_SRC_DIR    := src/avx
AVX_C_SOURCES  := $(wildcard $(AVX_SRC_DIR)/*.c)
AVX_C_OBJS     := $(patsubst %.c,%.o,$(AVX_C_SOURCES))

SSE42_SRC_DIR    := src/sse42
SSE42_C_SOURCES  := $(wildcard $(SSE42_SRC_DIR)/*.c)
SSE42_C_OBJS     := $(patsubst %.c,%.o,$(SSE42_C_SOURCES))

UNITTEST_DIR     := test
UNITTEST_SOURCES := $(wildcard $(UNITTEST_DIR)/*.cpp)
UNITTEST_OBJS    := $(patsubst %.cpp,%.o,$(UNITTEST_SOURCES))

DEMO_DIR         := demo
DEMO_SOURCES      := $(wildcard  $(DEMO_DIR)/*.c)
DEMO_OBJS        := $(patsubst %.c,%.o,$(DEMO_SOURCES))

$(shell if [ ! -d "build" ];then mkdir build; fi;)

all: $(TARGET) $(DEMO) $(UNITTEST)

$(TARGET) : $(C_OBJS) $(AVX2_C_OBJS) $(AVX_C_OBJS) $(SSE42_C_OBJS)
	$(AR) $(ARFLAGS) $@ $^
	@-cp include/rs.h build/rs.h

$(UNITTEST): $(C_OBJS) $(AVX2_C_OBJS) $(AVX_C_OBJS) $(SSE42_C_OBJS) $(UNITTEST_OBJS)
	$(CXX) -o $@ $(LIBS) $^

$(DEMO): $(DEMO_OBJS) $(TARGET)
	$(CC) -o $@  $(DEMO_OBJS) -Lbuild -lrs

distclean : clean
	-rm -fr $(SRC_DIR)/*.d $(AVX2_SRC_DIR)/*.d $(AVX_SRC_DIR)/*.d $(SSE42_SRC_DIR)/*.d $(UNITTEST_DIR)/*.d $(DEMO_DIR)/*.d
	-rm -fr build
clean:
	-rm -fr $(SRC_DIR)/*.o $(AVX2_SRC_DIR)/*.o $(AVX_SRC_DIR)/*.o $(SSE42_SRC_DIR)/*.o $(UNITTEST_DIR)/*.o $(DEMO_DIR)/*.o

.PHONY: clean distclean

$(AVX2_SRC_DIR)/%.o: $(AVX2_SRC_DIR)/%.c
	$(CC) -mavx2 $(CFLAGS) $(INCLUDE) -c $< -o $@

$(AVX_SRC_DIR)/%.o: $(AVX_SRC_DIR)/%.c
	$(CC) -mavx $(CFLAGS) $(INCLUDE) -c $< -o $@

$(SSE42_SRC_DIR)/%.o: $(SSE42_SRC_DIR)/%.c
	$(CC) -msse4.2 $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.cpp  
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

include $(C_SOURCES:.c=.d)
$(SRC_DIR)/%.d: $(SRC_DIR)/%.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(INCLUDE) $< > $@.$$$$; \
	sed "s,\(.*\)\.o[ :]*,$(SRC_DIR)\/\1.o $@ : ,g" $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(DEMO_SOURCES:.c=.d)
$(DEMO_DIR)/%.d: $(DEMO_DIR)/%.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(INCLUDE) $< > $@.$$$$; \
	sed "s,\(.*\)\.o[ :]*,$(DEMO_DIR)\/\1.o $@ : ,g" $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(AVX2_C_SOURCES:.c=.d)
$(AVX2_SRC_DIR)/%.d: $(AVX2_SRC_DIR)/%.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(INCLUDE) $< > $@.$$$$; \
	sed "s,\(.*\)\.o[ :]*,$(AVX2_SRC_DIR)\/\1.o $@ : ,g" $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(AVX_C_SOURCES:.c=.d)
$(AVX_SRC_DIR)/%.d: $(AVX_SRC_DIR)/%.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(INCLUDE) $< > $@.$$$$; \
	sed "s,\(.*\)\.o[ :]*,$(AVX_SRC_DIR)\/\1.o $@ : ,g" $@.$$$$ > $@; \
	rm -f $@.$$$$


include $(SSE42_C_SOURCES:.c=.d)
$(SSE42_SRC_DIR)/%.d: $(SSE42_SRC_DIR)/%.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(INCLUDE) $< > $@.$$$$; \
	sed "s,\(.*\)\.o[ :]*,$(SSE42_SRC_DIR)\/\1.o $@ : ,g" $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(UNITTEST_SOURCES:.cpp=.d)
$(UNITTEST_DIR)/%.d: $(UNITTEST_DIR)/%.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $(CXXFLAGS) $(INCLUDE) $< > $@.$$$$; \
	sed "s,\(.*\)\.o[ :]*,$(UNITTEST_DIR)\/\1.o $@ : ,g" $@.$$$$ > $@; \
	rm -f $@.$$$$
