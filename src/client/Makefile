##
 #   Copyright(c) 2004-2016, farsight Ltd. All rights reserved.
 #   
 #   File   ： Makefile
 #   Author ： jfe <jingfeihu@foxmail.com>
 #   Created： 2017-01-17
 #   Purpose： Compile the stuff mis client project.
 #
 ##

CC 		:= gcc
CFLAGS 	:= -Wall -O2 -g -I../../include
LD 		:= 

EXE 	:= client
OBJS 	:= $(patsubst %.c,%.o,$(wildcard ./*.c))

all:$(EXE)

$(EXE):$(OBJS)
	$(CC) $^ $(CFLAGS) $(LD) -o $@ 

.PHONY:clean
clean:
	rm -rf $(OBJS) $(EXE)