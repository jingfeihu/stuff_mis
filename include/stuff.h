/**
 * Copyright(c) 2004-2017, farsight Ltd. All rights reserved.
 * File Name : stuff.h
 * Author    : jfe <jingfeih@foxmail.com>
 * Create    : 2017-01-13
 * Purpose   : 员工信息类型
 */
 
#ifndef __STUFF_H__
#define __STUFF_H__

typedef struct{
    int type;   //权限    
    char name[32];  //用户名
    char passwd[32];    //密码
}USER;  //用户信息

typedef struct{
    char name[32];  //姓名
    char addr[64];  //地址
    char age;   //年龄
    int level;  //评级
    int no; //工号
    char phone[32]; //电话
    double salary;  //薪资
}INFO;  //员工信息

#endif