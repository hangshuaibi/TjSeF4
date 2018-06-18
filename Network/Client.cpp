#include "Client.h"
#include "chat_message.h"
#include <iostream>
#include <string>
#include <vector>

int serverIndex ;
int ipindex;                                //能正确连接的ip地址在ip列表里的索引
std::string                 currentIp;       //当前连接的ip地址字符串
std::vector<std::string>    ipList;          //所有连接到该网段的设备的ip地址表
std::vector<std::string> serverIpList;
std::string ipOut;
std::string _ip; //MainScene传入的手动ip


static Client* thisClient = nullptr;


chat_client::chat_client(boost::asio::io_service& io_service,
	tcp::resolver::iterator endpoint_iterator)
	: io_service_(io_service),
	socket_(io_service)//初始化
{
	do_connect(endpoint_iterator);
}


void chat_client::write(const chat_message& msg)
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

void chat_client::close()
{
	io_service_.post([this]() { socket_.close(); });
}


void chat_client:: do_connect(tcp::resolver::iterator endpoint_iterator)
	{
		//发起连接
		boost::asio::async_connect(socket_, endpoint_iterator,
			[this](boost::system::error_code ec, tcp::resolver::iterator)
		{
			if (!ec)
			{

				//测试部分
				serverIndex = ipindex;
				serverIpList.push_back(ipOut);
				do_read_header();//读取消息
			}
		});
	}

	void chat_client::do_read_header()
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

	void chat_client::do_read_body()
	{
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
			[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				thisClient->t_lock.lock();
				/*-----------------------------------------------------------*/
				//将消息压入队列，供UnitManager读取
				_orderList.push_back(std::string(read_msg_.body(), read_msg_.body_length()));
				/*-----------------------------------------------------------*/
				thisClient->t_lock.unlock();
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

	void chat_client::do_write()
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
	/*-----------------------------------------------------------*/
	std::string chat_client::getOrder()
	{
		if (_orderList.empty())
			return std::string("null");

		std::string msg = _orderList.front();
		_orderList.pop_front();

		return msg;
	}
	//发送消息的接口
	void chat_client::sendMessage(std::string sendMsg)
	{
		chat_message msg;
		//std::string sendMsg = "hello server!";
		msg.body_length(sendMsg.size());
		memcpy(msg.body(), sendMsg.c_str(), msg.body_length());
		msg.encode_header();

		this->write(msg);
	}
	/*-----------------------------------------------------------*/
	Client* Client::create(std::string ip)
	{
		_ip = ip;
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

	void Client:: runClient()
	{
		std::thread t(&Client::client, this);
		t.detach();
	}

	int Client::client()
	{
		
		system("ping 255.255.255.255");

		system("arp -a > arp.txt");

		std::ifstream in("arp.txt");

		bool start_read = false;
		while (!in.eof())
		{
			std::string temp;
			getline(in, temp);
			if (in.eof()) {
				break;
			}
			if (temp.size() >= 3)
			{
				//if (temp[2] == 'I')
				//{
				start_read = true;
				if (temp[2] == 'I')
					continue;
				//}
			}

			if (!start_read) {
				continue;
			}
			if (temp.size())
			{
				int i = temp.find(':', 3);
				if (i == -1)
				{
					int j = temp.find(' ', 3);
					temp = temp.substr(2, j - 2);

					currentIp = temp;
					ipList.push_back(temp);
				}
				else
				{
					int j = temp.find('-', 7);

					temp = temp.substr(6, j - 7);

					currentIp = temp;
					ipList.push_back(temp);

				}
			}
		}

		std::vector<boost::thread> t;

		for (int i = 0; i < ipList.size() - 1; i++)
		{


			if (ipList.size() == 0) {
				return 0;
			}

			bool if_wrong = false;
			int point_count = 0;

			for (int j = 0; j < ipList.at(i).size(); j++)
			{
				if (ipList.at(i).at(j) > '9' || ipList.at(i).at(j) < '0')
				{
					if (ipList.at(i).at(j) != '.')
					{
						if_wrong = true;
						break;
					}

					if (ipList.at(i).at(j) == '.')
					{
						point_count++;
					}

				}
			}

			if (point_count != 3 || if_wrong)
			{
				continue;
			}


			boost::asio::io_service io_service;

			tcp::resolver resolver(io_service);//10.22.5.232
			auto endpoint_iterator = resolver.resolve({ ipList.at(i), "1024" });
			ipOut = ipList.at(i);
			chat_client c(io_service, endpoint_iterator);//客户端
			_client = &c;
			thisClient = this;

			//std::thread t([&io_service]() { io_service.run(); });//启动线程执行io_service.run()
			boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
			ipindex = i;
			Sleep(100);
			c.close();
			t.join();



		}

		try {
			boost::asio::io_service io_service;

			tcp::resolver resolver(io_service);//10.22.5.232
			auto endpoint_iterator = resolver.resolve({_ip, "1024" });
			//auto endpoint_iterator = resolver.resolve({ipList.at(serverIndex), "1024" });
			chat_client c(io_service, endpoint_iterator);//客户端

			_client = &c;
			thisClient = this;
			//std::thread t([&io_service]() { io_service.run(); });//启动线程执行io_service.run()
			boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
			while (1)
			{
				;
			}

			c.close();
			t.join();
		}
		catch (std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << "\n";
		}
		return 0;
	}
		
	
	/*-----------------------------------------------------------*/
	void Client::sendMessage(std::string sendMsg)
	{
		_client->sendMessage(sendMsg);
	}

	std::string Client::getMessage()
	{
		return _client->getOrder();
	}
	/*-----------------------------------------------------------*/

