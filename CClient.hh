#ifndef CCLIENT_H_
# define CCLIENT_H_

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

#include "CRingBuffer.hh"

class CClient
{
private:
  int                 _socket;
  struct sockaddr_in  _addr;

  bool                _is_in_queue;
  CRingBuffer        _read_buf;
  CRingBuffer        _write_buf;

public:
  CClient(const int socket, struct sockaddr_in addr);
  ~CClient();

public:
  void closeSocket();
  std::string getIpAdress();
};

#endif
