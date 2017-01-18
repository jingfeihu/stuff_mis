/**
 * Copyright(c) 2004-2017, farsight Ltd. All rights reserved.
 * File Name : main.c
 * Author    : jfe <jingfeih@foxmail.com>
 * Create    : 2017-01-13
 * Purpose   : Stuff MIS Server
 */

#include <common.h> 
#include <server.h>
#include <stuff.h>
#include <signal.h>
#include <sqlite3.h>
#include <time.h>


int main(int argc, const char *argv[])
{
    int listenfd;
    struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
    if (init_server(&listenfd, &serveraddr, argc, argv) < 0) {	/* 设置服务器监听 */
		exit(-1);
	}
    sqlite3 *db;
    if (open_database(&db) < 0) {  	/* 打开数据库 */  
        exit(-1);
	}
    signal(SIGCHLD, handler_zombie); 	/* 处理僵尸进程 */
    while (1) {
        int connfd;
		socklen_t len = sizeof(SA);
		struct sockaddr_in clientaddr;
		bzero(&clientaddr, sizeof(clientaddr));
		/* 接受客户端连接请求 */
        if ((connfd = accept(listenfd, (SA *)&clientaddr, &len)) < 0) {
            perror("failed to accept");
            continue;
        }
        pid_t pid = fork();	
        if (pid == 0) {		/* 子进程处理连接的客户端 */
            while (1) {
                close(listenfd);
                MSG mbuf;
                bzero(&mbuf.mtext, TEXT_SIZE);
                if (recv(connfd, &mbuf, MSG_LEN, 0) <= 0) {
                    printf("%s:%d:%s:exit\n", inet_ntoa(clientaddr.sin_addr), \
						ntohs(clientaddr.sin_port), mbuf.name );
                    exit(0);
                }
                switch (mbuf.mtype) {
                case CMD_LOGIN:
					process_login(connfd, &clientaddr, &mbuf, db);
                    break;
                case CMD_LOGOUT:
                    printf("%s:%d:%s:logout\n", inet_ntoa(clientaddr.sin_addr),\
                           ntohs(clientaddr.sin_port), mbuf.name);
                    close(connfd);
                    exit(0);
                default:
                    puts("recv...valid.");
                }
            }
        } else if (pid > 0) {	/* 父进程等待客户端连接 */
            close(connfd);
        } else {
            perror("failed  to fork");
            exit(-1);
        }
    }
	
    return 0;
}
