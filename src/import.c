/*******************************************************************************
 *                    (c) Copyright 2016, farsight Ltd.
 *                           All rights reserved.
 * 
 * File   : import.c
 * By     : Jfe
 * Modify : 2016-11-15
 * Purpose: 词典导入数据库 
 *
 * 
 ******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sqlite3.h>

#define SIZE_WORD 32
#define SIZE_MEAN 1024


int main(int argc, const char *argv[])
{
	FILE *fp;
	sqlite3 *db;
	char *errmsg;
	char sql[SIZE_MEAN] = {0};
	char buf[SIZE_MEAN] = {0};
	
	/*打开文件*/
	if ((fp = fopen("./dict.txt", "r")) == NULL)
	{
		perror("fopen");
		exit(-1);
	}
	
	/*打开数据库*/
	if ((sqlite3_open("./dict.db", &db)) != SQLITE_OK)
	{
		printf("sqlite3_open %s\n", sqlite3_errmsg(db));
		exit(-1);
	}

	/*同名表则删除*/
	sprintf(sql, "drop table dict;");
	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("sqlite3_exec %s\n", sqlite3_errmsg(db));
		exit(-1);
	}

	/*创建表*/
	sprintf(sql, "create table dict(word text primary key, mean text);");
	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("sqlite3_exec %s\n", sqlite3_errmsg(db));
		exit(-1);
	}
	
	int count = 0;
	while (1)
	{
		int i = 0;
		char word[SIZE_WORD] = {0};
		char mean[SIZE_MEAN] = {0};
		bzero(buf, SIZE_MEAN);
		bzero(sql, SIZE_MEAN);
		
		/*读取一行到BUF*/
		if (fgets(buf, SIZE_MEAN, fp) == NULL)
		{
			break;
		}
		buf[strlen(buf) - 1] = '\0'; 	//去掉末尾空格
		
		/*分别获取单词和意思*/
		if (buf[16] == ' ')
		{
			i = 16;
			while (buf[i] == ' ')
			{
				i--;
			}
			strncpy(word, buf, i+1);
			strncpy(mean, buf+17, strlen(buf+17));
		}
		else
		{
			i = 0;
			/*读取单词*/
			while (1)
			{
				if (i >= 16 && buf[i] == ' ')
				{
					break;
				}
				word[i] = buf[i];
				i++;
			}
			
			/*读取意思*/
			while (buf[i] == ' ')
			{
				i++;
			}
			strncpy(mean, buf+i, strlen(buf+i));
		}
		
		/*将单词和意思加入数据库*/
		sprintf(sql, "insert into dict values ('%s','%s');", word, mean);
		if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
		{
			printf("sqlite3_exec %s\n", sqlite3_errmsg(db));
			exit(-1);
		}
		count++;
		printf("count = %-6d  >>  %s  >>  %s\n", count, word, mean);
	}
	
	sqlite3_close(db);
	fclose(fp);

	return 0;
}
