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
	this->_start = this->buffer;
	this->_end = this->buffer;
	this->_data_size = 0;
	this->_bufferend = (char*)this->_buffer + bufferSize;
}

CRingBuffer::~CRingBuffer()
{
}