#include "ServerOrNot.h"
#include <map>
#include <string>
#include "Network/Client.h"
#include <boost/asio.hpp>

const std::string remoteIp = "118.25.134.24";//远程服务器

using namespace std;
using namespace boost::asio;

static bool firsttime = true;
static map<string, string> servers;
Client* instance = nullptr;

Client* ServerOrNot::getClient()
{
	//static Client* instance = Client::create("118.25.134.24");
	if (firsttime)
	{
		firsttime = false;
		instance = Client::create(remoteIp);
		instance->retain();//>>>>>>>>>>>>>>>>>>>>>
	}
	assert(instance != nullptr);

	return instance;
}

const map<string, string>& ServerOrNot::getServers()
{
	auto client = getClient();
	
	auto msg = client->getMessage();
	//if(msg[0]=='I')msg = client->getMessage();
	while (msg[0] != 'n')
	{
		if (msg[0] == 's')
		{
			int ipend = msg.find_first_of("*");

			auto ip = msg.substr(1, ipend - 1);
			auto name = msg.substr(ipend + 1);

			
			//servers.insert(make_pair(ip, name));
			servers[ip] = name;//有即刷新房名，无则插入
		}
		//else if (msg[0] == 'e')
		{
			//auto name = msg.substr(msg.find_first_of("*") + 1);
			//servers.erase(name);
		}
		msg = client->getMessage();
	}
	for (auto it = servers.begin();it!=servers.end();)
	{
		//-----------
		//auto item = *it;
		io_context ioc;
		tcp::resolver res(ioc);
		auto endp = res.resolve(it->first, "1024");
		chat_client c(ioc, endp);
		thread t([&ioc]() {ioc.run();});
		Sleep(300);
		c.close();
		//-----------
		if (c.getOrderListSize() == 0)
		{
			auto msg = std::string("e").append(it->first).append("*").append(it->second);
			getClient()->sendMessage(msg);
			servers.erase((it++)->first);
		}
		else {
			++it;
		}

		t.join();
	}

	return servers;
}