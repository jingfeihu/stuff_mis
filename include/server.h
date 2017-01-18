/**
 * Copyright(c) 2004-2017, farsight Ltd. All rights reserved.
 * File Name : server.h
 * Author    : jfe <jingfeih@foxmail.com>
 * Create    : 2017-01-13
 * Purpose   : Stuff MIS Server
 */


#ifndef __SERVER_H__
#define __SERVER_H__

#include <common.h> 
#include <sqlite3.h>
#include <sys/wait.h>

#define DB_PATH "./res/stuff_MIS.db"
#define TABLE_NAME "stuff"

void handler_zombie(int arg);
int open_database(sqlite3 **db);
int init_server(int *listenfd_ptr, struct sockaddr_in *serveraddr_ptr, int argc, const char **argv);
int process_login(int connfd, struct sockaddr_in *clientaddr_ptr,MSG *mbuf, sqlite3 *db);

#if 0

int process_register(int connfd, MSG *mbuf, sqlite3 *db);
int process_quary(int connfd, MSG *mbuf, sqlite3 *db);
int process_history(int connfd, MSG *mbuf, sqlite3 *db);
int insert_history(MSG *mbuf, sqlite3 *db);
void handler(int arg);

#endif


#endif
