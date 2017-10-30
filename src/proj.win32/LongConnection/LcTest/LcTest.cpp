// LcTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "LongConnection.h"
#include "VSBridge.h"
#include <string>
#include "wrap/timer.h"
#include <memory>

std::string HOST = "123.206.229.213";
short PORT = 27710;


int _tmain(int argc, _TCHAR* argv[])
{
	LCSetResponse(VSBridge::getInstance());
	LCStartClient();
	LCConnect(HOST.c_str(), PORT, 3);

	getchar();
	LCStopClient(true);

	//�Ƚ�DataBlock��DataBlockLocal�����ܲ���
	std::shared_ptr<char> pBuf(new char[65535]);
	memset(pBuf.get(), '9', 65535);
	PreciseTimer timer;
	printf("���Կ�ʼ������\n");

	timer.start();
	NetworkUtil::DataBlock data1(65535);
	timer.stop();
	printf("DataBlock ���캯��ִ�� %lf ΢��\n", timer.getElapsedTimeInMicroSec());

	timer.start();
	NetworkUtil::DataBlockLocal<> data2;//��Ҫ�в���()
	NetworkUtil::DataBlockLocal65535 data3;
	timer.stop();
	printf("DataBlockLocal ���캯��ִ�� %lf ΢��\n", timer.getElapsedTimeInMicroSec());
	
	timer.start();
	data1.append(pBuf.get(), 65535);
	data1.append(pBuf.get(), 65535);
	data1.append(pBuf.get(), 65535);
	timer.stop();
	printf("DataBlock append����ִ�� %lf ΢��\n", timer.getLastElapsedTimeInMicroSec());

	timer.start();
	data2.append(pBuf.get(), 65535);
	data2.append(pBuf.get(), 65535);
	data2.append(pBuf.get(), 65535);
	timer.stop();
	printf("DataBlockLocal append����ִ�� %lf ΢��\n", timer.getLastElapsedTimeInMicroSec());

	printf("���Խ���������\n");

	getchar();

	return 0;
}

