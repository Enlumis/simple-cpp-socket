#include	"CClient.hh"
#include	"CServer.hh"

int main()
{
  	CClient::_packetsMap[Packet::DEFAULT] = &CClient::handleDefautPacket;
	CServer server(4000);
	server.run();
	return 0;
}
