# simple-cpp-socket

<h1>What is simple-cpp-socket</h1>
simple-cpp-socket is a very light TCP Server.</br>
The server can handle structured packets and can be compiled on any unix platform very easily

<h1>How to compile my server</h1>
To compile the program you can use Makefile
```
cd simple-cpp-socket
make all
```
or you can also use your prefered C++ compilator
```
g++ src/*.cpp -Iinclude
```
<b>simple-cpp-socket is very light and does not need any dependance</b>

<h1>How to create a simple TCP server</h1>
simple-cpp-socket is really simple to use.</br>
You just have to:
- Define packets ids
- Define a packet structure
- Define a packet handler
- Run the server

<h2>1. Define packets ids</h2>
<h5>PacketID.hh</h5>
```
#pragma once

typedef enum                  PacketID
{
	PACKET_DEFAULT      = 0
}                             PacketID;
```

<h2>2. Define a packet structure</h2>
<h5>PacketDefault.hh</h5>
```
#pragma once

#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "Packet.hh"

typedef struct			s_packet_data_default
{
  unsigned char		uchar_test;
  unsigned short	ushort_test;
  unsigned int		uint_test;
  short				short_test;
  int				int_test;
}__attribute__((__packed__))	t_packet_data_default;

class PacketDefault: public Packet
{
public:
	PacketDefault();
	~PacketDefault();

public:
	size_t getPacketLength();
	PacketID getPacketID();
	void serialize(char *dest);
	void unserialize(char *src);
	
public:
	t_packet_data_default _data;
};
```
<h5>PacketDefault.cpp</h5>
```
#include "PacketDefault.hh"

PacketDefault::PacketDefault() {
	this->_data.uchar_test = 56;
	this->_data.ushort_test = 1324;
	this->_data.uint_test = 2543624;
	this->_data.short_test = 456;
	this->_data.int_test = 423452;
}
PacketDefault::~PacketDefault() {
	
}
PacketID PacketDefault::getPacketID() {
	return PACKET_DEFAULT;
}
size_t PacketDefault::getPacketLength() {
	return sizeof(t_packet_data_default);
}
void PacketDefault::serialize(char *dest) {
	memcpy(dest,(char*) &this->_data, sizeof(t_packet_data_default));
}
void PacketDefault::unserialize(char *src) {
	t_packet_data_default *data_rebuild = (t_packet_data_default*)src;
	this->_data = *data_rebuild;
}
```

<h2>3. Define a packet handler</h2>
Put this line in your main file before the CServer initialization
```
CClient::_packetsMap[PACKET_DEFAULT] = &CClient::handleDefautPacket;
```
<h5>MyClient.cpp</h5>
```
bool CClient::handleDefautPacket(t_packet_data *packet_data) {
  PacketDefault testRead;
  testRead.unserialize(packet_data->data);
  std::cout << coutprefix << "testRead { uchar:" << testRead._data.uchar_test 
            << "ushort:" << testRead._data.ushort_test 
            << "uint:" << testRead._data.uint_test 
            << "short:" << testRead._data.short_test 
            << "int:" << testRead._data.int_test 
            << std::endl;

  return true;
}
```

<h2>4. Run the server</h2>
<h5>main.cpp</h5>
```
#include	"CClient.hh"
#include	"CServer.hh"

int main()
{
	CClient::_packetsMap[PACKET_DEFAULT] = &CClient::handleDefautPacket;
 	CServer server(4000);
	server.run();
	return 0;
}
```

<h2>How to send packet</h2>
You can send a packet from the server to the client using the *sendPacket* method
```
PacketDefault test;
this->sendPacket(test);
```
