#include "pack.h"
void PackDel::setPack(int Ptype,int Plenth,void *Pack,void *pdata)
{
	//打包

	PACK_HEAD head;
	memset(&head,0,PACK_HEAD_SIZE);
	head.head=PACK_HEAD_VALUE;
	head.type=Ptype;
	head.lenth=Plenth;

	PACK_TAIL tail;
	memset(&tail,0,PACK_TAIL_SIZE);
	tail.tail=PACK_TAIL_VALUE;

	memcpy((char*)pdata,&head,PACK_HEAD_SIZE);//包头赋值
	memcpy((char*)pdata+PACK_HEAD_SIZE+Plenth,&tail,PACK_TAIL_SIZE);//包尾赋值

    memcpy((char*)pdata+PACK_HEAD_SIZE,Pack,Plenth);//包体赋值

}
PACK_RETURN PackDel::getPack(int & Plenth,char * &pdata)
{	
	PACK_HEAD head;
	memset(&head,0,PACK_HEAD_SIZE);
	memcpy(&head,pdata,PACK_HEAD_SIZE);
	PACK_TAIL tail;
	memset(&tail,0,PACK_TAIL_SIZE);
	memcpy(&tail,pdata+PACK_HEAD_SIZE+head.lenth,PACK_TAIL_SIZE);
	PACK_RETURN res;
	memset(&res,0,sizeof(PACK_RETURN));
	//判断包是否完整
	if (head.head==PACK_HEAD_VALUE && tail.tail==PACK_TAIL_VALUE)
	{
        res.type=head.type;
        res.lenth=head.lenth;
        res.data=pdata+PACK_HEAD_SIZE;
		res.isOver=false;
		pdata=pdata+PACK_HEAD_SIZE+head.lenth+PACK_TAIL_SIZE;
        return res;
	}
	res.isOver=true;
	return res;
}