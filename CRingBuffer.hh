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
# define PACKET_SIZE 2048

typedef struct			s_packet
{
  char				packet_id;
  unsigned char			data[PACKET_SIZE + 1];
}__attribute__((__packed__))	t_packet;

class CRingBuffer 
{
public:
  char        _buffer[CRING_BUFFER_SIZE];
  char        *_start;
  char        *_end;
  char        *_bufferend;
  size_t      _data_size;

public:
  CRingBuffer();
  ~CRingBuffer();
};

#endif
