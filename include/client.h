/**
 * Copyright(c) 2004-2017, farsight Ltd. All rights reserved.
 * File Name : client.h
 * Author    : jfe <jingfeih@foxmail.com>
 * Create    : 2017-01-13
 * Purpose   : Stuff MIS Client
 */

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <common.h>
#include <stuff.h>
 
int init_client(int *sockfd_ptr, struct sockaddr_in *serveraddr_ptr, int argc, const char **argv);
int do_login(int sockfd, MSG *mbuf);
int do_logout(int sockfd, MSG *mbuf);
int do_quary(int sockfd, MSG *mbuf);

#endif