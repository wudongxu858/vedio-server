#ifndef _PACK_H_
#define _PACK_H_
#include "../ctool.h"
#include <stdio.h>
#include <string.h>
#define  PACK_HEAD_VALUE 1028 //包头标识
#define  PACK_TAIL_VALUE 1258 //包尾标识
#define  PACK_LOGASK_VALUE 1  //登入请求标识
#define  PACK_LOGANS_VALUE 2  //登入应答标识
#define  PACK_USEREXIT_VALUE 3 //用户请求退出标识


//包头
struct PackHead
{
	unsigned short int head;//包头的标志
	unsigned short int type;//包类型
	unsigned short int lenth;//包体长度
	
};
//包尾
struct PackTail
{
	unsigned short int tail;

};
//登入请求包
struct PackLoginAsk
{
	char name[20];
};
//登入应答包
struct PackReture
{
    int type;
    int lenth;
    char *data;
	bool isOver;
};  
//用户请求退出包
struct PackUserExit
{
	char username[20];
};



typedef struct PackHead PACK_HEAD;
typedef struct PackTail PACK_TAIL;
typedef struct PackReture PACK_RETURN;

#define PACK_HEAD_SIZE       sizeof(PACK_HEAD)
#define PACK_TAIL_SIZE       sizeof(PACK_TAIL)
#define PACK_RETURN_SIZE     sizeof(PACK_RETURN)

class PackDel
{
public:
/*********************************************
函数:void setPack(int Ptype,int Plenth,void *Pack.void *pdata)
功能:打包函数
参数:int Ptype 包类型
	 int Plenth 包体长度
	 void *Pack 包体指针
	 void *pdta 缓存空间地址
返回值:无
***********************************************/
    static void setPack(int Ptype,int Plenth,void *Pack,void *pdata);
    
/*********************************************
函数:int getPack(int Ptype,int Plenth,char *pdata)
功能:解包函数
int Plenth 包体长度
void *Pack 包
char *pdata 缓存空间地址
返回值:返回包体数据（bool）
***********************************************/

    static PACK_RETURN getPack(int & Plenth,char * &pdata);
};


#endif