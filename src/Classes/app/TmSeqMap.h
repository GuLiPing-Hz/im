#ifndef TMSEQMAP__H__
#define TMSEQMAP__H__

/*
	注释添加以及修改于 2014-4-3
	mender: glp

	封装一个能对超时进行处理并保存数据的map
	*/

#include "../wrap/event_handler.h"
#include "../wrap/seq_map.h"
#include "../wrap/thread_informer.h"


class TMSeqMap : public Wrap::TMEventHandler, public Wrap::SeqMap_ThreadSafe<Wrap::ReserveData*>
{
public:
	TMSeqMap(Wrap::Reactor *pReactor)
		: TMEventHandler(pReactor)
	{}

	virtual void onTimeOut(){
		Wrap::Guard lock(mCS);//安全锁

		SeqMap_ThreadSafe<Wrap::ReserveData*>::iterator it;
		for (it = begin(); it != end();)
		{
			Wrap::ReserveData* pRD = it->second;
			if (pRD && ((time(NULL) - pRD->t) > pRD->timeout)){//超时，没响应
				pRD->OnTimeOut();
				free(pRD);//释放内存
				it = del(it);//移除
			}
			else{
				it++;
			}
		}
	}
};


#endif//TMSEQMAP__H__
