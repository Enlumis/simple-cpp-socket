#include	"CRingBuffer.hh"



CRingBuffer::CRingBuffer()
{
//	this->_start = this->_buffer;
//	this->_end = this->_buffer;
//	this->_data_size = 0;
//	this->_bufferend = (char*)this->_buffer + CRING_BUFFER_SIZE;

  this->_realbufferSize = 0;
}

CRingBuffer::~CRingBuffer()
{
}

size_t CRingBuffer::getBufferLength() {
  return this->_realbufferSize;
}

bool CRingBuffer::pushPacket(Packet &p) {
  t_packet_header h;
  h.packet_id = p.getPacketID();
  h.packet_len = p.getPacketLength();
  if (this->_realbufferSize + h.packet_len + sizeof(t_packet_header) >= CRING_BUFFER_SIZE) {
    std::cout << coutprefix << " [Server][FATAL ERROR] send buffer overflow" << std::endl;  
    return (false);
  }
  my_strncpy(this->_realbuffer + this->_realbufferSize, (char*)&h, sizeof(t_packet_header));
  this->_realbufferSize += sizeof(t_packet_header);
  p.serialize(this->_realbuffer + this->_realbufferSize);
  this->_realbufferSize += h.packet_len;
  return true;
}

int CRingBuffer::sendSocket(int socket) {
  int need_send = this->_realbufferSize;
  int wd = send(socket, this->_realbuffer, need_send, 0);
  if (wd == -1) {
    return (-1);
  }
  this->_realbufferSize -= wd;
  std::cout << coutprefix << "Bytes sended: "<< wd << ", bytes rest: " << this->_realbufferSize << std::endl;
  return wd;
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
/*

char* CRingBuffer::getSafeBytePointer()
{
  int n = this->_bufferend - this->_start;
  my_strncpy(this->_tmpbuffer, this->_start, n);
  my_strncpy(this->_tmpbuffer + n, this->_buffer, CRING_BUFFER_SIZE - n);
  return this->_tmpbuffer;
}*/