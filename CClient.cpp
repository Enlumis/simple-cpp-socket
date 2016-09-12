#include	"CClient.hh"
#include <iomanip>
#include "PacketDefault.hh"

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



bool CClient::handlePacket(t_packet_data *packet)
{
  std::cout << coutprefix << this->getIpAdress() << " Packet received completly (ID: "<< packet->packet_id 
    << ", PacketLength: " << (packet->packet_len + sizeof(t_packet_header)) 
    << ", DataLength: " << packet->packet_len 
    << ")"<< std::endl;
  int c = 0;
  
  std::ios::fmtflags f( std::cout.flags() );
  std::cout << coutprefix << this->getIpAdress() << " Packet hexa data [ " << std::setfill('0');
  while (c < packet->packet_len) {
    std::cout << std::hex << std::setw(2) << static_cast<int>(packet->data[c]) << " ";
    c++;
  }
  std::cout << "]";
  std::cout << std::endl;
  std::cout.flags( f );

  PacketDefault test;
  this->sendPacket(test);
  // packet handling
  // packet->data

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
/*
bool CClient::handlePackets()
{
  t_packet_data  *packet_data;
  t_packet_header  *packet_header;
  
  std::cout << coutprefix << this->getIpAdress() << " handlePackets" << std::endl;
  while (this->_read_buf._data_size >= sizeof(t_packet_header))
  {
    packet_header = (t_packet_header*)this->_read_buf._start;

    std::cout << coutprefix << this->getIpAdress() << " Retriving packet ... (ID: "<< packet_header->packet_id 
    << ", Lenght: " << packet_header->packet_len << ")"<< std::endl;
    std::cout << coutprefix << this->getIpAdress() << " Ring Buffer data length: "<< this->_read_buf._data_size << std::endl;

    if (packet_header->packet_id > PACKET_ID_MAX) {
      std::cout << coutprefix << this->getIpAdress() << " Client try to send fake packet" << std::endl;
      return (false); 
    }
    if (packet_header->packet_len >= CRING_BUFFER_SIZE - sizeof(t_packet_header)) {
      std::cout << coutprefix << this->getIpAdress() << " Client try to send too big packet" << std::endl;
      return (false);
    }

    packet_data = (t_packet_data*)this->_read_buf.getSafeBytePointer();

    this->_read_buf._start += sizeof(t_packet_header) + packet_data->packet_len;
    if (this->_read_buf._start == this->_read_buf._bufferend)
      this->_read_buf._start = this->_read_buf._buffer;
    this->_read_buf._data_size -= sizeof(t_packet_header) + packet_data->packet_len;
    if (!this->handlePacket(packet_data))
      return (false);
  }
  return (true);
}
*/
/*bool CClient::doRead() {

  int   rd;
  int   need_read;

  std::cout << coutprefix << this->getIpAdress() << " Start reading"<< std::endl;
  if (this->_read_buf._data_size >= CRING_BUFFER_SIZE){
    std::cout << coutprefix << this->getIpAdress() << " [Server][FATAL ERROR] read ring buffer overflow" << std::endl;
    return (false);
  }
  if (this->_read_buf._start == this->_read_buf._bufferend)
    this->_read_buf._start = this->_read_buf._buffer;
  if (this->_read_buf._end == this->_read_buf._bufferend)
    this->_read_buf._end = this->_read_buf._buffer;
  if (this->_read_buf._end < this->_read_buf._start)
    need_read = this->_read_buf._start - this->_read_buf._end;
  else
    need_read = this->_read_buf._buffer + CRING_BUFFER_SIZE - this->_read_buf._end;
  rd = recv(this->_socket, this->_read_buf._end, need_read, 0);
  if (rd == -1)
  {
    std::cout << coutprefix << this->getIpAdress() << " [Server][FATAL ERROR] "<<strerror(errno) << std::endl;
    return (false);
  } 
  this->_read_buf._end += rd;
  this->_read_buf._data_size += rd;
  
  std::cout << coutprefix << this->getIpAdress() << " Bytes read :"<< rd << std::endl;
  if (rd == 0)
  {
    std::cout << coutprefix << this->getIpAdress() << " Client unreachable (recv return 0) "<< std::endl;
    return (false);
  }
  if (this->_read_buf._data_size >= sizeof(t_packet_header)) {
    return (this->handlePackets());
  }
  return (true);
}
bool CClient::doWrite() {
  
  int   need_write;

  need_write = 0;
  if (this->_write_buf._start == this->_write_buf._bufferend)
    this->_write_buf._start = this->_write_buf._buffer;
  if (this->_write_buf._end > this->_write_buf._start)
    need_write = this->_write_buf._end - this->_write_buf._start;
  else
    need_write = this->_write_buf._bufferend - this->_write_buf._start;
  if (send(this->_socket, this->_write_buf._start, need_write, 0) == -1) {
    std::cout << coutprefix << this->getIpAdress() << " [Server][FATAL ERROR] "<<strerror(errno) << std::endl;
    return (false);
  }
  this->_write_buf._data_size -= need_write;
  this->_write_buf._start += need_write;
  return (true);
}
*/
/*
bool CClient::isInQueue()
{
  return (this->_is_in_queue);
}

void CClient::setInQueue(bool b)
{
  this->_is_in_queue = b;
}

*/