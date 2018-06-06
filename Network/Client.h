#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "chat_message.h"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
	chat_client(boost::asio::io_service& io_service,
		tcp::resolver::iterator endpoint_iterator)
		: io_service_(io_service),
		socket_(io_service)//初始化
	{
		do_connect(endpoint_iterator);
	}

	void write(const chat_message& msg)
	{
		io_service_.post(//发起异步事件
			[this, msg]()
		{
			bool write_in_progress = !write_msgs_.empty();
			write_msgs_.push_back(msg);
			if (!write_in_progress)
			{
				do_write();
			}
		});
	}

	void close()
	{
		io_service_.post([this]() { socket_.close(); });
	}

private:
	void do_connect(tcp::resolver::iterator endpoint_iterator)
	{
		//发起连接
		boost::asio::async_connect(socket_, endpoint_iterator,
			[this](boost::system::error_code ec, tcp::resolver::iterator)
		{
			if (!ec)
			{
				do_read_header();//读取消息
			}
		});
	}

	void do_read_header()
	{
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.data(), chat_message::header_length),
			[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec && read_msg_.decode_header())
			{
				do_read_body();
			}
			else
			{
				socket_.close();
			}
		});
	}

	void do_read_body()
	{
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
			[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				std::cout.write(read_msg_.body(), read_msg_.body_length());
				std::cout << "\n";//打印消息到屏幕
				do_read_header();
			}
			else
			{
				socket_.close();
			}
		});
	}

	void do_write()
	{
		boost::asio::async_write(socket_,
			boost::asio::buffer(write_msgs_.front().data(),
				write_msgs_.front().length()),
			[this](boost::system::error_code ec, std::size_t /*length*/)//handle write
		{
			if (!ec)
			{
				write_msgs_.pop_front();
				if (!write_msgs_.empty())
				{
					do_write();
				}
			}
			else
			{
				socket_.close();
			}
		});
	}

private:
	boost::asio::io_service& io_service_;
	tcp::socket socket_;
	chat_message read_msg_;
	chat_message_queue write_msgs_;
};

class Client :public Node {
	chat_client* _client;

public:
	void runClient()
	{
		std::thread t(&Client::client, this);
		t.detach();
	}

	int client()
	{
		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		auto endpoint_iterator = resolver.resolve({ "192.168.1.103", "1024" });
		chat_client c(io_service, endpoint_iterator);//客户端

		_client = &c;

		std::thread t([&io_service]() { io_service.run(); });//启动线程执行io_service.run()

		while (1)
		{
			;
		}

		c.close();
		t.join();
	}

	void sendMessage(std::string s)
	{
		chat_message msg;
		std::string sendMsg = "hello server!";
		msg.body_length(sendMsg.size());
		memcpy(msg.body(), sendMsg.c_str(), msg.body_length());
		msg.encode_header();

		_client->write(msg);
	}

	static Client* create()
	{
		Client* sprite = new Client();
		if (sprite)
		{
			sprite->autorelease();
			sprite->runClient();

			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
};