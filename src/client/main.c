/**
 * Copyright(c) 2004-2017, farsight Ltd. All rights reserved.
 * File Name : client.c
 * Author    : jfe <jingfeih@foxmail.com>
 * Create    : 2017-01-13
 * Purpose   : Stuff MIS Client
 */

#include <common.h>
#include <client.h>
#include <layout_client.h>


int main(int argc, const char *argv[])
{
	int sockfd, ret;
	struct sockaddr_in serveraddr;
    MSG mbuf;
	char cmd[TEXT_SIZE] = {0};
/* 主页 */	
layout_main:
    while (1) {
        layout_main();
        bzero(cmd, sizeof(cmd));
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {	/* 获取命令 */
			return -1;
		}
        switch (atoi(cmd)) {
        case 1:	/* 登录 */
			if (init_client(&sockfd, &serveraddr, argc, argv) < 0) { /* 初始化客户端 */
				puts("网络异常");
                sleep(1);
				break;
			}
			ret = do_login(sockfd, &mbuf);	/* 密码验证 */
			if (ret == 0) {
				goto layout_user;
			} else if (ret == 1) {
				goto layout_admin;
			} else {
				puts("用户名或密码错误");
                sleep(1);
                break;
			}
        case 2:	/* 退出 */
            exit(0);
        default:
            puts("无效的命令");
            sleep(1);
        }
    }
/* 普通用户主页 */
layout_user:
    while (1) { 
        layout_user();
        bzero(cmd, sizeof(cmd));
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {	/* 获取命令 */
			return -1;
		}
        switch (atoi(cmd)) {
        case 1:	/* 查询信息 */
			if (do_quary(sockfd, &mbuf) < 0) {
				puts("网络异常");
                sleep(1);
			}
			break;
		case 2:	/* 修改信息 */
            puts("修改信息...");
            sleep(1);
            goto layout_user_update_detail;
            break;
        case 3:
            puts("注销...");
            do_logout(sockfd, &mbuf);
            goto layout_main;
        default:
            puts("cmd is valid.");
            sleep(1);
        }
    }


layout_user_update_detail:
    while (1) {
        layout_user_update_detail();
        bzero(cmd, sizeof(cmd));
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {	/* 获取命令 */
			return -1;
		}
        switch (atoi(cmd))
        {
        case 1:
            puts("用户名..."); 
            sleep(1);
            break;
        case 2:
            puts("密码...");
            sleep(1);
            break;
        case 3:
            puts("姓名...");
            sleep(1);
            break;
        case 4:
            puts("地址...");
            sleep(1);
            break;
        case 5:
            puts("年龄...");
            sleep(1);
            break;
        case 6:
            puts("电话...");
            sleep(1);
            break;
        case 7:
            puts("返回...");
            sleep(1);
            goto layout_user;
        default:
            puts("cmd is valid.");
            sleep(1);
        }
    }

layout_admin:
    while (1) {
        layout_admin();
        bzero(cmd, sizeof(cmd));
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {	/* 获取命令 */
			return -1;
		}
        switch (atoi(cmd))
        {
        case 1:
            puts("查询信息..."); 
            sleep(1);
            goto layout_admin_quary;
        case 2:
            puts("修改信息...");
            sleep(1);
            goto layout_admin_update;
        case 3:
            puts("添加用户...");
            sleep(2);
            goto layout_admin_insert;
        case 4:
            puts("删除用户...");
            sleep(1);
            goto layout_admin_delete;
        case 5:
            puts("注销...");
            do_logout(sockfd, &mbuf);
            goto layout_main;
        default:
            puts("cmd is valid.");
            sleep(1);
        }
    }


layout_admin_quary:
    while (1) {
        layout_admin_quary();
        bzero(cmd, sizeof(cmd));
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {	/* 获取命令 */
			return -1;
		}
        switch (atoi(cmd))
        {
        case 1:
            puts("输入用户名..."); 
            sleep(1);
            break;
        case 2:
            puts("返回...");
            sleep(1);
            goto layout_admin;
        default:
            puts("cmd is valid.");
            sleep(1);
        }
    }
    
    
layout_admin_update:
    while (1) {
        layout_admin_update();
        bzero(cmd, sizeof(cmd));
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {	/* 获取命令 */
			return -1;
		}
        switch (atoi(cmd))
        {
        case 1:
            puts("输入用户名..."); 
            sleep(1);
            goto layout_admin_update_detail;
        case 2:
            puts("返回...");
            sleep(1);
            goto layout_admin;
        default:
            puts("cmd is valid.");
            sleep(1);
        }
    }


layout_admin_update_detail:
    while (1) {
        layout_admin_update_detail();
        bzero(cmd, sizeof(cmd));
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {	/* 获取命令 */
			return -1;
		}
        switch (atoi(cmd))
        {
        case 1:
            puts("权限...");
            sleep(1);
            break;
        case 2:
            puts("用户名..."); 
            sleep(1);
            break;
        case 3:
            puts("密码...");
            sleep(1);
            break;
        case 4:
            puts("姓名...");
            sleep(1);
            break;
        case 5:
            puts("地址...");
            sleep(1);
            break;
        case 6:
            puts("年龄...");
            sleep(1);
            break;
        case 7:
            puts("等级...");
            sleep(1);
            break;
        case 8:
            puts("工号...");
            sleep(1);
            break;
        case 9:
            puts("电话...");
            sleep(1);
            break;
        case 10:
            puts("工资...");
            sleep(1);
            break;
        case 11:
            puts("返回...");
            sleep(1);
            goto layout_admin_update;
        default:
            puts("cmd is valid.");
            sleep(1);
        }
    }


layout_admin_insert:
    while (1) {
        layout_admin_insert();
        bzero(cmd, sizeof(cmd));
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {	/* 获取命令 */
			return -1;
		}
        switch (atoi(cmd))
        {
        case 1:
            puts("输入用户名..."); 
            sleep(1);
            break;
        case 2:
            puts("返回...");
            sleep(1);
            goto layout_admin;
        default:
            puts("cmd is valid.");
            sleep(1);
        }
    }
    
    
layout_admin_delete:
    while (1) {
        layout_admin_delete();
        bzero(cmd, sizeof(cmd));
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {	/* 获取命令 */
			return -1;
		}
        switch (atoi(cmd))
        {
        case 1:
            puts("输入用户名..."); 
            sleep(1);
            break;
        case 2:
            puts("返回...");
            sleep(1);
            goto layout_admin;
        default:
            puts("cmd is valid.");
            sleep(1);
        }
    }
    

	return 0;
}

