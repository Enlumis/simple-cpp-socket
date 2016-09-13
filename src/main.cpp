#include	"CClient.hh"
#include	"CServer.hh"

int main()
{
  	CClient::_packetsMap[PACKET_DEFAULT] = &CClient::handleDefautPacket;
	CServer server(4000);
	server.run();
	return 0;
}
