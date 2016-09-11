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

char* CRingBuffer::getSafeBytePointer()
{
  int n = this->_bufferend - this->_start;
  my_strncpy(this->_tmpbuffer, this->_start, n);
  my_strncpy(this->_tmpbuffer + n, this->_buffer, CRING_BUFFER_SIZE - n);
  return this->_tmpbuffer;
}