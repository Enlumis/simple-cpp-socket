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
}

CRingBuffer::~CRingBuffer()
{
}