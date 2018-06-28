#include "Client.h"
#include "Scenes/ServerOrNot.h"
#include <boost/asio.hpp>

#include <string>
#include <vector>
#include <map>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")

using namespace std;
using namespace boost::asio;

/*全局map,每次lanSearch清空一次*/
static map<string, string> roomlist;
static bool firsttime = true;
static vector<string> ips;
static vector<string> ipHeads;

const map<string, string>& Client::getRoomlist()
{
	return roomlist;
	//return ServerOrNot::getServers();
}

void Client::lanSearch()
{
	roomlist.clear();

	getLocalhost();

	if (ipHeads.empty())
	{
		return;
	}
	auto ipHead = ipHeads.front();

	
	for (int i = 0;i <= 255; i += 4)
	{
		auto ips(vector<string>(4, ipHead));
		for (int j = 0;j < 4;++j)ips[j].append(to_string(i + j));
		io_context iocs[4];

		tcp::resolver res0(iocs[0]);
		tcp::resolver res1(iocs[1]);
		tcp::resolver res2(iocs[2]);
		tcp::resolver res3(iocs[3]);

		auto edp_it0 = res0.resolve(ips[0], "1024");
		auto edp_it1 = res1.resolve(ips[1], "1024");
		auto edp_it2 = res2.resolve(ips[2], "1024");
		auto edp_it3 = res3.resolve(ips[3], "1024");

		typedef shared_ptr<chat_client> client_ptr;
		vector<client_ptr> clients;
		clients.push_back(client_ptr(new chat_client(iocs[0], edp_it0)));
		clients.push_back(client_ptr(new chat_client(iocs[1], edp_it1)));
		clients.push_back(client_ptr(new chat_client(iocs[2], edp_it2)));
		clients.push_back(client_ptr(new chat_client(iocs[3], edp_it3)));

		std::thread t0([&iocs]() {iocs[0].run();});
		std::thread t1([&iocs]() {iocs[1].run();});
		std::thread t2([&iocs]() {iocs[2].run();});
		std::thread t3([&iocs]() {iocs[3].run();});

		Sleep(100);
		for (int j = 0;j < 4;++j)
		{
			if (clients[j]->_orderList.size() == 1)
			{
				log(ips[j].c_str());
				auto order = clients[j]->getOrder();
				auto roomname = order.substr(order.find_first_of("0123456789") + 1);
				log(roomname.c_str());
				roomlist.insert(make_pair(ips[j], roomname));

				//clients[j]->sendMessage("Bye~xixi");
			}
			clients[j]->close();
		}
		t0.join();
		t1.join();
		t2.join();
		t3.join();

	}
}

std::string Client::getLocalhost()
{
	if (firsttime)
	{
		firsttime = false;

		WSAData data;
		assert(WSAStartup(MAKEWORD(1, 1), &data) == 0);
		char host[255];
		assert(gethostname(host, sizeof(host)) != SOCKET_ERROR);

		//vector<string> ipHeads;

		struct hostent *p = gethostbyname(host);
		for (int i = 0; p->h_addr_list[i] != 0; i++)
		{
			struct in_addr in;
			memcpy(&in, p->h_addr_list[i], sizeof(struct in_addr));
			std::string temp(inet_ntoa(in));

			ips.push_back(temp);

			int pos = temp.find_last_of('.');
			ipHeads.push_back(temp.substr(0, pos + 1));
		}

		WSACleanup();
	}

	if (ips.empty())
	{
		assert(0);
		return "";//记得做判断
	}
	return ips.front();
}
