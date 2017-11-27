// LcTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vld.h>
#include "LongConnection.h"
#include "VSBridge.h"
#include <string>
#include "wrap/timer.h"
#include <memory>
#include "wrap/seq_map.h"
#include "wrap/thread_wrapper.h"
#include "wrap/sleep.h"

std::string HOST = "123.206.229.213";
short PORT = 27710;

void testXN(){
	//比较DataBlock和DataBlockLocal的性能差异
	std::shared_ptr<char> pBuf(new char[65535]);
	memset(pBuf.get(), '9', 65535);
	PreciseTimer timer;
	printf("测试开始！！！\n");

	timer.start();
	NetworkUtil::DataBlock data1(65535);
	timer.stop();
	printf("DataBlock 构造函数执行 %lf 微秒\n", timer.getElapsedTimeInMicroSec());

	timer.start();
	NetworkUtil::DataBlockLocal<> data2;//需要有参数()
	NetworkUtil::DataBlockLocal65535 data3;
	timer.stop();
	printf("DataBlockLocal 构造函数执行 %lf 微秒\n", timer.getElapsedTimeInMicroSec());

	timer.start();
	data1.append(pBuf.get(), 65535);
	data1.append(pBuf.get(), 65535);
	data1.append(pBuf.get(), 65535);
	timer.stop();
	printf("DataBlock append函数执行 %lf 微秒\n", timer.getLastElapsedTimeInMicroSec());

	timer.start();
	data2.append(pBuf.get(), 65535);
	data2.append(pBuf.get(), 65535);
	data2.append(pBuf.get(), 65535);
	timer.stop();
	printf("DataBlockLocal append函数执行 %lf 微秒\n", timer.getLastElapsedTimeInMicroSec());

	printf("测试结束！！！\n");
}

int g_Int = 11;

bool Fun1(ThreadObj obj)
{
	NetworkUtil::SeqMap_ThreadSafe<int>& map1 = *((NetworkUtil::SeqMap_ThreadSafe<int>*)obj);

	while (true)
	{
		if (!map1.empty())
			map1.del(map1.begin());

		printf("************************************************************************\nFun1");
		map1.lock();
		NetworkUtil::SeqMap<int>::iterator it = map1.begin();
		for (it; it != map1.end(); it++)
			printf("cur [%d] = %d\n", it->first, it->second);
		printf("\n");
		map1.unlock();

		SleepMs(1000);
	}
}

bool Fun2(ThreadObj obj)
{
	NetworkUtil::SeqMap_ThreadSafe<int>& map1 = *((NetworkUtil::SeqMap_ThreadSafe<int>*)obj);

	while (true)
	{
		map1.put(g_Int, g_Int * 2);

		printf("************************************************************************\nFun2");
		map1.lock();
		map1.lock();
		NetworkUtil::SeqMap<int>::iterator it = map1.begin();
		for (it; it != map1.end(); it++)
			printf("cur [%d] = %d;", it->first, it->second);
		printf("\n");
		map1.unlock();
		map1.unlock();

		SleepMs(500);
	}
	
}

void testMap(){

	static NetworkUtil::SeqMap_ThreadSafe<int> map1;//NetworkUtil::SeqMap<int> map1;
	for (int i = 1; i < 10; i++){
		if (i != 1)
			map1.put(i, i * 2);
		else
			map1.put(i, i);
	}
	map1.cover(1, 1 * 2);
	map1.cover(10, 20);

	int* d1 = map1.get(1);
	map1.del(1);
	NetworkUtil::SeqMap<int>::iterator it = map1.del(map1.begin());
	int size = map1.size();
	//map1.clear();

	for (it; it != map1.end();)
	{
		//code
		printf("cur [%d] = %d\n", it->first, it->second);

		if (it->first == 5)
			it = map1.erase(it);//删除的时候
		else
			it++;//不删除的时候
	}

	
	ThreadWrapper* t1 = ThreadWrapper::CreateThread(Fun1, &map1);
	ThreadWrapper* t2 = ThreadWrapper::CreateThread(Fun2, &map1);

	unsigned int thread_id1;
	unsigned int thread_id2;
	t1->Start(thread_id1);
	t2->Start(thread_id2);
}

int _tmain(int argc, _TCHAR* argv[])
{
	LCSetResponse(VSBridge::getInstance());
	LCStartClient();
	LCConnect(HOST.c_str(), PORT, 3);

	getchar();
	LCStopClient(true);

	//testXN();
	//testMap();

	getchar();

	return 0;
}

