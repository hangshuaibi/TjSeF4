#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "chat_message.h"
#include <fstream>




using boost::asio::ip::tcp;


typedef std::deque<chat_message> chat_message_queue;

class Client;

class chat_client
{
	friend class Client;
	//public:
	chat_client(boost::asio::io_service& io_service,
		tcp::resolver::iterator endpoint_iterator);

	void write(const chat_message& msg);
	void close();
private:
	

	void do_connect(tcp::resolver::iterator endpoint_iterator);

	void do_read_header();

	void do_read_body();

	void do_write();
	/*-----------------------------------------------------------*/
private:
	//从服务端收到的消息列表
	std::deque<std::string> _orderList;

	//读取消息的接口，一次读取一条，从头部开始读取
	std::string getOrder();
	//发送消息的接口
	void sendMessage(std::string sendMsg);
	/*-----------------------------------------------------------*/

private:
	boost::asio::io_service& io_service_;
	tcp::socket socket_;
	chat_message read_msg_;
	chat_message_queue write_msgs_;
};

class Client :public Node {
	chat_client* _client;

public:
	std::mutex t_lock;

	void runClient();

	int client();
	/*-----------------------------------------------------------*/
	void sendMessage(std::string sendMsg);
	std::string getMessage();
	/*-----------------------------------------------------------*/

	static Client* create(std::string ip);
	
};


