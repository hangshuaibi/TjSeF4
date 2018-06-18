#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace LocalSpace {
	
	int ipindex;                                ///能正确连接的ip地址在ip列表里的索引
	int serverIndex;                            ///server所在的ip地址在ip列表里的索引

	std::string                 ipOut;          ///能成功建立连接的ip地址字符串
	std::string                 currentIp;      ///当前连接的ip地址字符串
	std::vector<std::string>    ipList;         ///所有连接到该网段的设备的ip地址表
	std::vector<std::string>    serverIpList;   ///能够连接上的, 存在server的ip地址表
}