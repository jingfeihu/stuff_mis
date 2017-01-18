/**
 * Copyright(c) 2004-2017, farsight Ltd. All rights reserved.
 * File Name : client.c
 * Author    : jfe <jingfeih@foxmail.com>
 * Create    : 2017-01-13
 * Purpose   : Stuff MIS Client
 */

#include <client.h>


/**
 * init_client--初始化客户端
 * return
 *    |__ 0--成功
 *    |__-1--失败
 * Describe:
 *		1.SA--地址信息结构体 	在common.h文件中定义
 *		2.NAME_SIZE--地址大小 	在common.h文件中定义
 */
int init_client(int *sockfd_ptr, struct sockaddr_in *serveraddr_ptr, \
	int argc, const char **argv)
{
    /* 获取IP和端口号 */
    char addr[NAME_SIZE] = {0};
    char port[NAME_SIZE] = {0};
    if (argc == 1) {
        strcpy(addr, "127.0.0.1");
        strcpy(port, "8888");
    } else if (argc == 3) {
        strcpy(addr, argv[1]);
        strcpy(port, argv[2]);
    } else {
        fprintf(stderr, "Useage: %s <IP> <PORT>\n", argv[0]);
        return -1;
    }
	/* 连接到服务器 */
    bzero(serveraddr_ptr, sizeof(SA));
    serveraddr_ptr->sin_family = AF_INET;
    serveraddr_ptr->sin_port = htons(atoi(port));
    serveraddr_ptr->sin_addr.s_addr = inet_addr(addr);
	if ((*sockfd_ptr = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("failed to socket");
        return -1;
    }
    if (connect(*sockfd_ptr, (SA*)serveraddr_ptr, sizeof(SA)) < 0) {
        perror("failed to connect");
        return -1;
    }
	return 0;
}


/**
 * do_login--登录验证
 * return
 *    |__ 1--成功：管理员
 *    |__ 0--成功：普通用户
 *    |__-1--失败
 * Describe:
 *		1.CMD_LOGIN--消息类型 	在common.h文件中定义
 *		2.MSG_LEN--消息大小 	在common.h文件中定义
 */
int do_login(int sockfd, MSG *mbuf)
{
    mbuf->mtype = CMD_LOGIN;
    printf("username>> ");
    fflush(stdout);
    if (fgets(mbuf->name, sizeof(mbuf->name), stdin) == NULL) {
        perror("fgets");
        return -1;
    }
    mbuf->name[strlen(mbuf->name) - 1] = '\0';
    printf("password>> ");
    fflush(stdout);
    if (fgets(mbuf->mtext, sizeof(mbuf->mtext), stdin) == NULL) {
        perror("fgets");
        return -1;
    }
    mbuf->mtext[strlen(mbuf->mtext) - 1] = '\0';
    puts("Login..."); 
    send(sockfd, mbuf, MSG_LEN, 0);
    if (recv(sockfd, mbuf, MSG_LEN, 0) <= 0) {
        return -1;
    }
	if (strncmp(mbuf->mtext, "user", 4) == 0) {
		return 0;
	} else if (strncmp(mbuf->mtext, "admin", 5) == 0) {
		return 1;
	} else {
		return -1;
	}
}


/**
 * do_login--注销
 * return
 *    |__ 0--成功
 *    |__-1--失败
 * Describe:
 *		1.CMD_LOGOUT--消息类型 	在common.h文件中定义
 *		2.MSG_LEN--消息大小 	在common.h文件中定义
 */
int do_logout(int sockfd, MSG *mbuf)
{
	mbuf->mtype = CMD_LOGOUT;
    puts("Logout...");
    send(sockfd, mbuf, MSG_LEN, 0);
    close(sockfd);
    return 0;
}

/**
 * do_quary--查询
 * return
 *    |__ 0--成功
 *    |__-1--失败
 * Describe:
 *		1.CMD_LOGOUT--消息类型 	在common.h文件中定义
 *		2.MSG_LEN--消息大小 	在common.h文件中定义
 */
int do_quary(int sockfd, MSG *mbuf)
{
	mbuf->mtype = CMD_QUARY;
	puts("User quary...");
	send(sockfd, mbuf, MSG_LEN, 0);
	if (recv(sockfd, mbuf, MSG_LEN, 0) < 0) {
		return -1;
	}
	INFO *info = (INFO*)mbuf->mtext;
	printf("姓名 = %s\n", info->name);
	printf("地址 = %s\n", info->addr);
	printf("年龄 = %d\n", info->age);
	printf("评级 = %d\n", info->level);
	printf("工号 = %d\n", info->no);
	printf("电话 = %s\n", info->phone);
	printf("薪资 = %f\n", info->salary);
	return 0;
}