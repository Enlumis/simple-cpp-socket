#include	"CClient.hh"
#include <iomanip>
#include "PacketDefault.hh"

namespace SCPPS
{
std::map<PacketID, CClient::PacketHandler> CClient::_packetsMap;

CClient::CClient(CServer* server, const int socket, struct sockaddr_in addr)
  : _server(server), _socket(socket)
{
  this->_is_in_queue = false;
  this->_addr = addr;
  this->_straddr = std::string(inet_ntoa(addr.sin_addr));
  std::cout << coutprefix << "Connect " << inet_ntoa(addr.sin_addr) << std::endl;
}

CClient::~CClient()
{
}
bool CClient::handleDefautPacket(t_packet_data *packet_data) {
  PacketDefault testRead;
  testRead.unserialize(packet_data->data);
  std::cout << coutprefix << "testRead { uchar:" << testRead._data.uchar_test 
            << "ushort:" << testRead._data.ushort_test 
            << "uint:" << testRead._data.uint_test 
            << "short:" << testRead._data.short_test 
            << "int:" << testRead._data.int_test 
            << std::endl;

  PacketDefault test;
  this->sendPacket(test);
  return true;
}

bool CClient::handlePacket(t_packet_data *packet)
{
  std::cout << coutprefix << this->getIpAdress() << " Packet received completly (ID: "<< packet->packet_id 
    << ", PacketLength: " << (packet->packet_len + sizeof(t_packet_header)) 
    << ", DataLength: " << packet->packet_len 
    << ")"<< std::endl;
  unsigned int c = 0;
  
  std::ios::fmtflags f( std::cout.flags() );
  std::cout << coutprefix << this->getIpAdress() << " Packet hexa data [ " << std::setfill('0');
  while (c < packet->packet_len) {
    std::cout << std::hex << std::setw(2) << static_cast<int>(packet->data[c]) << " ";
    c++;
  }
  std::cout << "]";
  std::cout << std::endl;
  std::cout.flags( f );

  std::map<PacketID, PacketHandler>::iterator it = this->_packetsMap.find(static_cast<PacketID>(packet->packet_id));

  if (it != this->_packetsMap.end()){
    return (this->*((*it).second))(packet);
  }else{
    return false;
  }
  return true;
}
bool CClient::doRead() {
  int rd = this->_read_buf.readSocket(this->_socket);
  if (rd == -2) {
    std::cout << coutprefix << this->getIpAdress() << " [Server][FATAL ERROR] Buffer overflow" << std::endl;
    return (false);
  }
  if (rd == -1) {
    std::cout << coutprefix << this->getIpAdress() << " [Server][FATAL ERROR] "<<strerror(errno) << std::endl;
    return (false);
  }
  if (rd == 0) {
    std::cout << coutprefix << this->getIpAdress() << " Client unreachable (recv return 0) "<< std::endl;
    return (false);
  }
  t_packet_data *packet;
  while ((packet = this->_read_buf.extractPacket()) != NULL) {
    if (!this->handlePacket(packet))
      return (false);
  }
  return true;
}



bool CClient::sendPacket(Packet &p) {
  if (!this->_write_buf.pushPacket(p)) {
    std::cout << coutprefix << this->getIpAdress() << "Fail to push the packet" << std::endl;
    return false;
  }
  if (this->_is_in_queue == false) {
    this->_server->clientAddWriteListening(this);
  }
  std::cout << coutprefix << this->getIpAdress() << " Packet ready to send (ID: "<< p.getPacketID() 
    << ", PacketLength: " << (p.getPacketLength()  + sizeof(t_packet_header)) 
    << ", DataLength: " << p.getPacketLength()
    << ")"<< std::endl;
  return true;
}
bool CClient::doWrite() {
  int wd = this->_write_buf.sendSocket(this->_socket);
  if (wd == -1) {
    std::cout << coutprefix << this->getIpAdress() << " [Server][FATAL ERROR] "<<strerror(errno) << std::endl;
    return (false);
  }
  if (this->_write_buf.getBufferLength() == 0) {
    this->_server->clientRemoveWriteListening(this);
    this->_is_in_queue = false;
  }
  return true;
}
bool CClient::isInQueue()
{
  return (this->_is_in_queue);
}





std::string CClient::getIpAdress()
{
  return this->_straddr;
}

int CClient::getSocket()
{
  return this->_socket;
}

void CClient::closeSocket()
{
  close(this->_socket);
}
}
