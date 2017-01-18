/**
 * Copyright(c) 2004-2017, farsight Ltd. All rights reserved.
 * File Name : server.c
 * Author    : jfe <jingfeih@foxmail.com>
 * Create    : 2017-01-13
 * Purpose   : Stuff MIS Server
 */

#include <server.h>


/**
 * init_server--初始化服务器监听套接字
 * return
 *    |__ 0--成功
 *    |__-1--失败
 * Describe:
 *		1.SA--地址信息结构体 	在common.h文件中定义
 *		2.BACKLOG--最大监听数	在common.h文件中定义
 */
int init_server(int *listenfd_ptr, \
    struct sockaddr_in *serveraddr_ptr, int argc, const char **argv)
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
    /* 设置监听套接字 */
    if ((*listenfd_ptr = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("failed to socket");
        return -1;
    }
    bzero(serveraddr_ptr, sizeof(SA));
    serveraddr_ptr->sin_family = AF_INET;
    serveraddr_ptr->sin_port = htons(atoi(port));
    serveraddr_ptr->sin_addr.s_addr = inet_addr(addr);
    if (bind(*listenfd_ptr, (SA *)serveraddr_ptr, sizeof(SA)) < 0) {
        perror("failed to bind");
        return -1;
    }
    if (listen(*listenfd_ptr, BACKLOG) < 0) {
        perror("failed to listen");
        return -1;
    }
	return 0;
}


/**
 * open_database--打开数据库，并创建表
 * return
 *    |__ 0--成功
 *    |__-1--失败
 * Describe:
 *		1.DB_PATH--数据库路径名		在server.h文件中定义
 *		2.TABLE_NAME--表名 			在server.h文件中定义 		
 */
int open_database(sqlite3 **db)
{
    char sql[TEXT_SIZE] = {0};
    char *errmsg;
    if (sqlite3_open(DB_PATH, db) != SQLITE_OK) {
        puts(sqlite3_errmsg(*db));
        return -1;
    }
    sprintf(sql, "create table if not exists %s(\
		id integer primary key autoincrement,type integer,username string,\
		password string,name string,addr string,age string,level integer,\
		phone string,salary real)", TABLE_NAME);
    if (sqlite3_exec(*db, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
        printf("sqlite3_exec: %s\n", errmsg);
        return -1;
    }
    return 0;
}


/**
 * process_login--处理客户端登录请求
 * return
 *    |__ 0--成功
 *    |__-1--失败
 * Describe:
 *		1.DB_PATH--数据库路径名		在server.h文件中定义
 *		2.TABLE_NAME--表名 			在server.h文件中定义 		
 */
int process_login(int connfd, struct sockaddr_in *clientaddr_ptr, MSG *mbuf, sqlite3 *db)
{
    puts("process_login...");
    char sql[TEXT_SIZE] = {0};
    char **resultp;
    int nrow, ncolumn;
    char *errmsg;
	/* 验证密码 */
    sprintf(sql, "select * from stuff where username='%s' and password='%s';", \
		mbuf->name, mbuf->mtext);
    if (sqlite3_get_table(db, sql, &resultp, &nrow, &ncolumn, &errmsg) != SQLITE_OK) {
        strcpy(mbuf->mtext, "fail");
        send(connfd, mbuf, MSG_LEN, 0);
		printf("%s:%d:%s:sqlite3_get_table > %s\n", \
			inet_ntoa(clientaddr_ptr->sin_addr), ntohs(clientaddr_ptr->sin_port), \
			mbuf->name, errmsg);
        return -1;
    }
    if (nrow > 0) {	/* 密码验证通过 */
		/* 验证权限 */
        sprintf(sql, "select type from stuff where username='%s';", mbuf->name);
        if (sqlite3_get_table(db, sql, &resultp, &nrow, &ncolumn, &errmsg) != SQLITE_OK) {
            strcpy(mbuf->mtext, "fail");
            send(connfd, mbuf, MSG_LEN, 0);
			printf("%s:%d:%s:sqlite3_get_table > %s\n", \
				inet_ntoa(clientaddr_ptr->sin_addr), ntohs(clientaddr_ptr->sin_port), \
				mbuf->name, errmsg);
            return -1;
        }
        if (nrow > 0) {	/* 验证权限成功 */
            //printf("rights type = %s\n", resultp[1]);
            switch (atoi(resultp[1])) {
            case 0:	/* 普通用户权限 */
                strcpy(mbuf->mtext, "user");
                send(connfd, mbuf, MSG_LEN, 0);
                printf("%s:%d:%s:<user>login...ok!\n", \
					inet_ntoa(clientaddr_ptr->sin_addr), \
					ntohs(clientaddr_ptr->sin_port), mbuf->name);
                return 0;
            case 1:	/* 管理员权限 */
                strcpy(mbuf->mtext, "admin");
                send(connfd, mbuf, MSG_LEN, 0);
                printf("%s:%d:%s:<admin>login...ok!\n", \
					inet_ntoa(clientaddr_ptr->sin_addr), \
					ntohs(clientaddr_ptr->sin_port), mbuf->name);
                return 0;
            default: /* 权限异常 */
                strcpy(mbuf->mtext, "fail");
                send(connfd, mbuf, MSG_LEN, 0);
                printf("%s:%d:%s:<admin>login...failed > rights error!\n",\
                       inet_ntoa(clientaddr_ptr->sin_addr), \
					   ntohs(clientaddr_ptr->sin_port), mbuf->name);
                return -1;
            }
        } else { 	/* 验证权限失败 */
            strcpy(mbuf->mtext, "fail");
            send(connfd, mbuf, MSG_LEN, 0);
            printf("%s:%d:%s:<admin>login...failed > rights error!\n",\
                   inet_ntoa(clientaddr_ptr->sin_addr), \
				   ntohs(clientaddr_ptr->sin_port), mbuf->name);
            return -1;
        }
    } else { /* 密码验证失败 */
        strcpy(mbuf->mtext, "fail");
        send(connfd, mbuf, MSG_LEN, 0);
        printf("%s:%d:%s:<admin>login...failed > password error!\n",\
               inet_ntoa(clientaddr_ptr->sin_addr), \
			   ntohs(clientaddr_ptr->sin_port), mbuf->name);
        return -1;
    }
}



#if 0
/*处理注册*/
int process_register(int connfd, MSG *mbuf, sqlite3 *db)
{
    puts("process_register...");
    printf("mtype = %d\n", mbuf->mtype);
    printf("name = %s\n", mbuf->name);
    printf("mtext = %s\n", mbuf->mtext);

    char sql[TEXT_SIZE] = {0};
    char *errmsg;

    sprintf(sql, "insert into stuff values('%s', '%s');", mbuf->name, mbuf->mtext);
    if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        printf("sqlite3_exec: %s", errmsg);
        strcpy(mbuf->mtext, "fail");
        send(connfd, mbuf, MSG_LEN, 0);
        return -1;
    }
    else
    {
        strcpy(mbuf->mtext, "ok");
        send(connfd, mbuf, MSG_LEN, 0);
        return 0;
    }
}







/*处理查询 */
int process_quary(int connfd, MSG *mbuf, sqlite3 *db)
{
    puts("process_quary...");
    char sql[TEXT_SIZE] = {0};
    char **resultp;
    int nrow, ncolumn;
    char *errmsg;

    sprintf(sql, "select mean from stuff where word='%s';", mbuf->mtext);
    if (sqlite3_get_table(db, sql, &resultp, &nrow, &ncolumn, &errmsg) != SQLITE_OK)
    {
        printf("sqlite3_get_table: %s", errmsg);
        strcpy(mbuf->mtext, "fail");
        send(connfd, mbuf, MSG_LEN, 0);
        return -1;
    }
    else
    {
        if (nrow > 0)
        {
            insert_history(mbuf, db);

            strcpy(mbuf->mtext, "ok");
            send(connfd, mbuf, MSG_LEN, 0);

            puts(resultp[1]);
            strcpy(mbuf->mtext, resultp[1]);
            send(connfd, mbuf, MSG_LEN, 0);

            puts("quary...ok!");
            return 0;
        }
        else
        {
            strcpy(mbuf->mtext, "fail");
            send(connfd, mbuf, MSG_LEN, 0);
            return -1;
        }
    }
}


/*处理查询历史*/
int process_history(int connfd, MSG *mbuf, sqlite3 *db)
{
    puts("process_history...");
    char sql[TEXT_SIZE] = {0};
    char **resultp;
    int nrow, ncolumn;
    char *errmsg;

//	puts(mbuf->name);
    sprintf(sql, "select * from history where name='%s';", mbuf->name);
    if (sqlite3_get_table(db, sql, &resultp, &nrow, &ncolumn, &errmsg) != SQLITE_OK)
    {
        printf("sqlite3_get_table: %s", errmsg);
        strcpy(mbuf->mtext, "fail");
        send(connfd, mbuf, MSG_LEN, 0);
        //puts("sql...fail.");
        return -1;
    }
    else
    {
        if (nrow > 0)
        {
            strcpy(mbuf->mtext, "ok");
            send(connfd, mbuf, MSG_LEN, 0);

            int i;
            for (i = 0; i< ncolumn*nrow; i+=3)
            {
                sprintf(mbuf->mtext, "%s %s %s", resultp[i], resultp[i+1], resultp[i+2]);
                //		puts(mbuf->mtext);
                send(connfd, mbuf, MSG_LEN, 0);
            }

            strcpy(mbuf->mtext, "end");
            send(connfd, mbuf, MSG_LEN, 0);

            return 0;
        }
        else
        {
            strcpy(mbuf->mtext, "fail");
            send(connfd, mbuf, MSG_LEN, 0);
            //	puts("result....zero.");
            return -1;
        }
    }
}


/*插入历史记录*/
int insert_history(MSG *mbuf, sqlite3 *db)
{
    char time_buf[TEXT_SIZE] = {0};
    time_t t;
    struct tm *tmp;
    char sql[N] = {0};
    char *errmsg;

    time(&t);
    tmp = localtime(&t);
    sprintf(time_buf, "%4d-%2d-%2d %02d:%02d:%02d", tmp->tm_year + 1900, tmp->tm_mon + 1, \
            tmp->tm_mday, tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

    sprintf(sql, "insert into history values('%s', '%s', '%s');", time_buf, mbuf->name, mbuf->mtext);
    if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        printf("sqlite3_exec: %s\n", errmsg);
        return -1;
    }
    return 0;
}

#endif

/*处理僵尸进程*/
void handler_zombie(int arg)
{
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

