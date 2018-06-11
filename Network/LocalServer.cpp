#pragma once

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "chat_message.h"

#define TEST_CODE

/*-----------------------------------------------------------*/
#include <cstdio>
#include <string>
//#include "Classes/Data.h"
/*-----------------------------------------------------------*/
#define MAX_PLAYER_NUM 4
using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

/*-----------------------------------------------------------*/
int clientNum = 0;//客户端数量，借此识别玩家id
int preparedClientNum = 0;
int startFlag = false;//游戏开始后禁止加入

bool	 isIdUsed[MAX_PLAYER_NUM] = { 0,0,0,0 };//
												/*-----------------------------------------------------------*/

class chat_participant
{
public:
	virtual ~chat_participant() {}
	virtual void deliver(const chat_message& msg) = 0; //participant should deliver message
	int _id = -1;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;//shared ptr

class chat_server;//room的友元
class chat_room
{
	friend class chat_server;

	chat_message stringToMsg(const char s[])
	{
		chat_message msg;

		msg.body_length(strlen(s));
		memcpy(msg.body(), s, msg.body_length());
		msg.encode_header();

		return msg;
	}

public:
	void join(chat_participant_ptr participant)
	{
		participants_.insert(participant);//add a client
										  //将之前的消息写一遍给最新的连接者
		for (auto msg : recent_msgs_)
			participant->deliver(msg);

		/*-----------------------------------------------------------*/
		//发送玩家id给client
		++clientNum;
		assert(clientNum <= 4);
		int validId = -1;
		for (int i = 0;i < MAX_PLAYER_NUM;++i)
		{
			if (!isIdUsed[i])
			{
				validId = i;
				break;
			}
		}
		assert(validId >= 0);
		isIdUsed[validId] = 1;
		participant->_id = validId;

		char s[20];
		sprintf(s, "Id(%d", validId + 1);
		chat_message msg = stringToMsg(s);

		participant->deliver(msg);
		/*-----------------------------------------------------------*/
	}

	void leave(chat_participant_ptr participant)
	{
		participants_.erase(participant);//remove a client

		isIdUsed[participant->_id] = 0;//清空flag
		std::cout << "id  " << participant->_id + 1 << "  is valid" << std::endl;
		--clientNum;
		assert(clientNum >= 0);
	}

	void deliver(const chat_message& msg)
	{

#ifdef TEST_CODE
		{
			if (msg.body()[msg.body_length() - 1] == 'T')//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<测试
			{
				std::string msg(msg.body(), msg.body_length());
				std::cout << msg << std::endl;

				return;
			}
		}
#endif
		if (!startFlag && msg.body()[0] == 'C')//Client ready!>>>>>>>>>>>>>>>>>>>>>>>>
		{
			if (++preparedClientNum == clientNum
				&& clientNum >= 2 //游戏人数大于2
				)
			{
				chat_message startMsg = stringToMsg("Start!");
				for (auto participant : participants_)
					participant->deliver(startMsg);

				startFlag = true;
			}
			return;
		}
		//先将发送的消息放入待发送列表。
		recent_msgs_.push_back(msg);
		while (recent_msgs_.size() > max_recent_msgs)
			recent_msgs_.pop_front();//delete expired message

		for (auto participant : participants_)
			participant->deliver(msg);//将消息向所有的对象转发一遍
	}

private:
	std::set<chat_participant_ptr> participants_;//all client
	enum { max_recent_msgs = 0 };//保存的最大聊条记录数量
	chat_message_queue recent_msgs_;
};//chat room

class chat_session :
	public chat_participant,
	public std::enable_shared_from_this<chat_session>
{
public:
	chat_session(tcp::socket socket, chat_room& room)
		: socket_(std::move(socket)),
		room_(room)
	{
	}
	void start()
	{
		//收到连接之后首先将客户加入到room中
		//
		room_.join(shared_from_this());//add a participant

		do_read_header();//读消息
	}
	void deliver(const chat_message& msg)
	{
		//
		bool write_in_progress = !write_msgs_.empty();//空
		write_msgs_.push_back(msg);//会将消息先放到write_msgs_中
		if (!write_in_progress)
		{
			//write message
			do_write();
		}
	}
private:
	void do_read_header()
	{
		auto self(shared_from_this());
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.data(), chat_message::header_length),//data,length
			[this, self](boost::system::error_code ec, std::size_t /*length*/)//lambada
		{
			if (!ec && read_msg_.decode_header())
			{
				do_read_body();// read body
			}
			else
			{
				auto ep_ = socket_.remote_endpoint();
				std::cout << "client : " << ep_.port() << " leave this room" << std::endl;

				room_.leave(shared_from_this());//read a eof
			}
		});
	}

	void do_read_body()
	{
		auto self(shared_from_this());
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				room_.deliver(read_msg_);//deliver将发送消息，刚连接进来的客户将收到历史消息
				do_read_header();//
			}
			else
			{
				room_.leave(shared_from_this());
			}
		});
	}

	void do_write()
	{
		auto self(shared_from_this());
		boost::asio::async_write(socket_,//当前session的socket
			boost::asio::buffer(write_msgs_.front().data(),
				write_msgs_.front().length()),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				//no change 
				std::string msg(write_msgs_.front().body(), write_msgs_.front().body_length());
				std::cout << msg << std::endl;
				write_msgs_.pop_front();
				if (!write_msgs_.empty())
				{
					do_write();
				}
			}
			else
			{
				room_.leave(shared_from_this());
			}
		});
	}

	tcp::socket socket_;
	chat_room& room_;
	chat_message read_msg_;
	chat_message_queue write_msgs_;
};//seesion

class chat_server
{
public:
	//constructor
	chat_server(boost::asio::io_service& io_service,
		const tcp::endpoint& endpoint)
		: acceptor_(io_service, endpoint),//listen fd
		socket_(io_service)//conn fd
	{
		do_accept();
	}

private:
	void do_accept()
	{
		//a new connection
		acceptor_.async_accept(socket_,
			[this](boost::system::error_code ec)
		{
			if (!ec)
			{
				auto ep_ = socket_.remote_endpoint();
				std::cout << "client : " << ep_.port() << " enter this room" << std::endl;

				auto newClient = std::make_shared<chat_session>(std::move(socket_), room_);
				newClient->start();//session

			}

			do_accept();
		});
	}

	tcp::acceptor acceptor_;
	tcp::socket socket_;
	//server should keep all client connected
	chat_room room_;//chat_room class//保存所有的client
};


int main(int argc, char* argv[])
{
	try
	{
#if 0
		if (argc < 2)
		{
			std::cerr << "Usage: chat_server <port> [<port> ...]\n";
			return 1;
		}
#endif

		boost::asio::io_service io_service;

		std::list<chat_server> servers;
		//  for (int i = 1; i < argc; ++i)
		//  {
		tcp::endpoint endpoint(tcp::v4(), 1024);//listen 1024 port
		servers.emplace_back(io_service, endpoint);//constructor
												   //  }

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}