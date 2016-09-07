#include	"CClient.hh"

CClient::CClient(const int socket, struct sockaddr_in addr)
  : _socket(socket)
{
  this->_is_in_queue = false;
  this->_addr = addr;
  printf("[Server][Connect][%s]\n", inet_ntoa(addr.sin_addr));
}

CClient::~CClient()
{
}





void		my_strncpy(char *dest, char *str, int n)
{
  int		i;

  i = 0;
  while (i < n)
    {
      dest[i] = str[i];
      i++;
    }
}


CRingBuffer::CRingBuffer()
{
	this->_start = this->buffer;
	this->_end = this->buffer;
	this->_data_size = 0;
	this->_bufferend = (char*)this->_buffer + bufferSize;
}

CRingBuffer::~CRingBuffer()
{
}