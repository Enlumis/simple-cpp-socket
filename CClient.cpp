#include	"CClient.hh"

CClient::CClient(CServer* server, const int socket, struct sockaddr_in addr)
  : _server(server), _socket(socket)
{
  this->_is_in_queue = false;
  this->_addr = addr;
  std::cout << coutprefix << "Connect " << inet_ntoa(addr.sin_addr) << std::endl;
}

CClient::~CClient()
{
}

std::string CClient::getIpAdress()
{
  return "0.0.0.0";
}
bool CClient::handlePacket()
{
  return (true);
}

bool CClient::haveSomethingToSend() {
//  return (this->_write_buf._data_size >= sizeof(t_packet));
  return (this->_write_buf._data_size > 0);
}

bool CClient::doRead() {

  int   rd;
  int   need_read;

  if (this->_read_buf._data_size >= CRING_BUFFER_SIZE){
    std::cout << coutprefix << "[Server][FATAL ERROR] read ring buffer overflow" << std::endl;
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
    std::cout << coutprefix << "[Server][FATAL ERROR] "<<strerror(errno) << std::endl;
    return (false);
  } 
  this->_read_buf._end += rd;
  this->_read_buf._data_size += rd;
  if (rd == 0)
  {
    std::cout << coutprefix << "Client unreachable (recv return 0) "<< std::endl;
    return (false);
  }
  if (this->_read_buf._data_size >= sizeof(t_packet)) {
    return (this->handlePacket());
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
    std::cout << coutprefix << "[Server][FATAL ERROR] "<<strerror(errno) << std::endl;
    return (false);
  }
  this->_write_buf._data_size -= need_write;
  this->_write_buf._start += need_write;
  return (true);
}

int CClient::getSocket()
{
  return this->_socket;
}

void CClient::closeSocket()
{
  close(this->_socket);
}

bool CClient::isInQueue()
{
  return (this->_is_in_queue);
}

void CClient::setInQueue(bool b)
{
  this->_is_in_queue = b;
}

