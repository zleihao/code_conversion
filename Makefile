# 可执行文件名
TARGET = main

# gcc类型
CC = gcc

# 存放中间文件的路径
BUILD_DIR = build

#存放.c 源文件的文件夹
SRC_DIR = \
	./src

# 存放头文件的文件夹
INC_DIR = \
	./inc

# 在头文件路径前面加入-I
INCLUDE	= $(patsubst %, -I %, $(INC_DIR))

# 得到带路径的 .c 文件
CFILES := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))

# 得到不带路径的 .c 文件
CFILENDIR := $(notdir  $(CFILES))

# 将工程中的.c 文件替换成 ./build 目录下对应的目标文件 .o
COBJS = $(patsubst %, ./$(BUILD_DIR)/%, $(patsubst %.c, %.o, $(CFILENDIR)))

# make 自动在源文件目录下搜索 .c 文件
VPATH = $(SRC_DIR)

$(BUILD_DIR)/$(TARGET).exe : $(COBJS)
	$(CC) -o $@ $^

$(COBJS) : $(BUILD_DIR)/%.o : %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCLUDE) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

