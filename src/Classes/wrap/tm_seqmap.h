#ifndef TMSEQMAP__H__
#define TMSEQMAP__H__

/*
	注释添加以及修改于 2014-4-3
	mender: glp

	封装一个能对超时进行处理并保存数据的map
	*/

#include "event_handler.h"
#include "seq_map.h"
#include "thread_informer.h"

namespace NetworkUtil{

	class TMSeqMap : public TMEventHandler, public SeqMap_ThreadSafe<ReserveData*>
	{
	public:
		TMSeqMap(Reactor *pReactor)
			: TMEventHandler(pReactor)
		{}

		virtual void onTimeOut(){
			std::map<int, ReserveData*> RDMap = *getMap();
			std::map<int, ReserveData*>::iterator it;

			ReserveData *pRD;
			for (it = RDMap.begin(); it != RDMap.end(); it++)
			{
				pRD = it->second;
				if ((time(NULL) - pRD->t) > pRD->timeout)//超时，没响应
					pRD->OnTimeOut();
			}
		}
	};

}

#endif//TMSEQMAP__H__
