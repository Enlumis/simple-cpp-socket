#include	"CClient.hh"
#include	"CServer.hh"


int main()
{
  	SCPPS::CClient::_packetsMap[PACKET_DEFAULT] = &SCPPS::CClient::handleDefautPacket;
	SCPPS::CServer server(4000);
	server.run();
	return 0;
}
