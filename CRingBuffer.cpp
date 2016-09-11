#include	"CRingBuffer.hh"

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
	this->_start = this->_buffer;
	this->_end = this->_buffer;
	this->_data_size = 0;
	this->_bufferend = (char*)this->_buffer + CRING_BUFFER_SIZE;

  this->_realbufferSize = 0;
}

CRingBuffer::~CRingBuffer()
{
}

char* CRingBuffer::getSafeBytePointer()
{
  int n = this->_bufferend - this->_start;
  my_strncpy(this->_tmpbuffer, this->_start, n);
  my_strncpy(this->_tmpbuffer + n, this->_buffer, CRING_BUFFER_SIZE - n);
  return this->_tmpbuffer;
}

int CRingBuffer::readSocket(int socket) {
  int need_read = CRING_BUFFER_SIZE - this->_realbufferSize;
  if (need_read == 0)
    return -2;
  int rd = recv(socket, this->_realbuffer + this->_realbufferSize, need_read, 0);
  this->_realbufferSize += rd;
  std::cout << coutprefix << "Bytes read :"<< rd << std::endl;
  return rd;
}
t_packet_data* CRingBuffer::extractPacket() {
  t_packet_data* packet = (t_packet_data*)this->_realbuffer;

  if (this->_realbufferSize >= sizeof(t_packet_header) && 
      this->_realbufferSize - sizeof(t_packet_header) >= packet->packet_len) {

    int packetSize = sizeof(t_packet_header) + packet->packet_len;
    my_strncpy(this->_realtmpbuffer, this->_realbuffer, packetSize);
    packet = (t_packet_data*)this->_realtmpbuffer;

    this->_realbufferSize -= packetSize;

    if (this->_realbufferSize > 0) {
      // shift rest packet
      my_strncpy(this->_realbuffer, this->_realbuffer + packetSize, CRING_BUFFER_SIZE - packetSize);      
    }
    return packet;
  }
  return NULL;
}

