#ifndef PROTOCOL_H__20170930
#define PROTOCOL_H__20170930

enum eCMD{
	/*
	C->S:
	string uid    �û�ID
	string token  Token
	string appkey Ӧ��Key
	S->C: ��
	*/
	CMD_LOGIN_C2S2C = 1,//��¼

	/*
	C->S: ��
	S->C: �� �ͻ��˲����û��
	*/
	CMD_LOGOUT_C2S2C = 2,//�ǳ�

	/*
	C->S: һ��
	S->C: һ��
	char    type       type==0ʱ,to_uid��ʾ���û�id��type==1ʱ,to_uid��ʾ��Ⱥid
	string from_uid   ������ 0 ��ʾϵͳ,������ʾ�û�ID
	string to_uid     �����߶���
	string content    json�ַ���
	string ext        json�ַ���
	*/
	CMD_SAYTO_C2S2C = 3,//����

	/*
	S->C:
	string from_uid   0 ��ʾϵͳ,������ʾ�û�ID
	string content    json�ַ���
	*/
	CMD_NOTIFY_S2C = 4,//֪ͨ��Ϣ

	/*
	C->S:
	string room_id  ����ID

	S->C:
	string room_id  ����ID
	string uid
	*/
	CMD_ENTERROOM_C2S2C = 5,//���뷿��

	/*
	C->S: ��
	S->C:
	string uid
	*/
	CMD_EXITROOM_C2S2C = 6,//�뿪����

	/*
	C->S: ��
	*/
	CMD_KEEPALIVE_C2S = 7,//������

	/*
	S->C: ��
	*/
	CMD_DRIVEAWAY_S2C = 8,//��������
};

#endif // PROTOCOL_H__20170930
