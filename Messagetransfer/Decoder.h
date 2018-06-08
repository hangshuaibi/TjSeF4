#pragma once

#include "cocos2d.h"
#include "iostream"
#include "GridMap.h"
#include <string>
#include <vector>

using namespace std;
typedef vector<Grid> GridVector;


class Decoder {
private:
	GridVector _path;

	//�ӷ�������ȡ������Ϣ
	string _message;

	//��Ϣ��ʾ�Ĳ�������
	string _type;//

	//��Ϣ��ʿ���ĵ�id
	int _id;
public:

	//��ȡ������Ϣ����Decoder
	Decoder(const string& message);

	//����Ϣ�н����·��
	GridVector decodePath();

	//������Ϣ��ʿ����id
	int getId();


	//���ز�������
	char getType();

};