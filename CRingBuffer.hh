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

#define CRING_BUFFER_SIZE 40

typedef struct			s_packet_header
{
  unsigned short	packet_id;
  unsigned int      packet_len;
}__attribute__((__packed__))	t_packet_header;

typedef struct			s_packet_data
{
  unsigned short	packet_id;
  unsigned int      packet_len;
  unsigned char		data[CRING_BUFFER_SIZE - sizeof(t_packet_header)];
}__attribute__((__packed__))	t_packet_data;



class CRingBuffer 
{
public:
  char        _buffer[CRING_BUFFER_SIZE];
  char        _tmpbuffer[CRING_BUFFER_SIZE];
  char        *_start;
  char        *_end;
  char        *_bufferend;
  size_t      _data_size;

public:
  CRingBuffer();
  ~CRingBuffer();

  char* getSafeBytePointer();
};

#endif
