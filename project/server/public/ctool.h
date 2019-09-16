#ifndef _C_TOOL_H
#define _C_TOOL_H
#define Debug(fmt, ...)   printf("%s:%d %s>" fmt "\n",__FILE__,__LINE__,__FUNCTION__ ,##__VA_ARGS__)//##当为空时候把 ,去掉


#endif