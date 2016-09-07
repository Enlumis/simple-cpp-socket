#ifndef CRINGBUFFER_H_
# define CRINGBUFFER_H_

# include	<iostream>
# include	<string>
# include	<exception>

# include <sys/stat.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <sys/types.h>

# include <netinet/in.h>
# include <arpa/inet.h>

#include <stdlib.h>
#include <netdb.h>

#define CRING_BUFFER_SIZE 8000

class CRingBuffer 
{
private:
  char        _buffer[CRING_BUFFER_SIZE];
  char        *_start;
  char        *_end;
  char        *_bufferend;
  size_t      _data_size;

public:
  CCRingBuffer();
  ~CCRingBuffer();
};

#endif
