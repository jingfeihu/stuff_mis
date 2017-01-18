##
 #   Copyright(c) 2004-2016, farsight Ltd. All rights reserved.
 #   
 #   File   ： Makefile
 #   Author ： jfe <jingfeihu@foxmail.com>
 #   Created： 2017-01-13
 #   Purpose： Compile the stuff mis project.
 #
 ##

ROOT_DIR = $(shell pwd)
EXE = server client

all:$(EXE)
	cp $(ROOT_DIR)/src/client/client .
	cp $(ROOT_DIR)/src/server/server .
client:
	make -C $(ROOT_DIR)/src/client
server:
	make -C $(ROOT_DIR)/src/server

.PHONY:clean
clean:
	make clean -C $(ROOT_DIR)/src/client
	make clean -C $(ROOT_DIR)/src/server
	rm -rf $(EXE)

