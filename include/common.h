/**
 * Copyright(c) 2004-2017, farsight Ltd. All rights reserved.
 * File Name : stuff.h
 * Author    : jfe <jingfeih@foxmail.com>
 * Create    : 2017-01-13
 * Purpose   : 员工信息类型
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define NAME_SIZE   32
#define TEXT_SIZE   1024
#define BACKLOG     100

typedef enum{
    CMD_LOGIN  = 0x10,  /* 登录 */
    CMD_LOGOUT = 0x11,  /* 注销 */
	CMD_QUARY  = 0x12,	/* 查询 */
	

}MTYPE; /* 消息类型 */

typedef struct {
    MTYPE mtype;    /* 消息类型 */
    char name[NAME_SIZE]; 	/* 用户名 */
    char mtext[TEXT_SIZE]; 	/* 消息内容 */
}MSG; /* 消息结构体 */

typedef struct sockaddr SA;

#define MSG_LEN sizeof(MSG)

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)
    
#endif
