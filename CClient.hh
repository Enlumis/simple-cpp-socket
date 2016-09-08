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
#include <unistd.h>

#include "global.h"

#include "CRingBuffer.hh"

class CServer;

class CClient
{
private:
  CServer*            _server;
  int                 _socket;
  struct sockaddr_in  _addr;

  bool                _is_in_queue;
public:
  CRingBuffer        _read_buf;
  CRingBuffer        _write_buf;

public:
  CClient(CServer* server, const int socket, struct sockaddr_in addr);
  ~CClient();

public:
  int getSocket();
  void closeSocket();
  std::string getIpAdress();
  bool isInQueue();
  void setInQueue(bool b);
  bool haveSomethingToSend();

  bool handlePacket();

  bool doWrite();
  bool doRead();
};

#endif
