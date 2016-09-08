#ifndef CSERVER_H_
# define CSERVER_H_

# include	<iostream>
# include	<string>
# include	<exception>
# include <list>

# include <sys/stat.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <sys/types.h>

# include <netinet/in.h>
# include <arpa/inet.h>

#include <stdlib.h>
#include <netdb.h>

#include "global.h"

#include "CClient.hh"

class CServerException : public std::exception
{
private:
  std::string		m_error;

public:
  CServerException(const std::string &error);
  virtual ~CServerException() throw();

public:
  virtual const char* what() const throw();
};

class CServer
{
private:
  int	                  _port;
  int	                  _socket;

  std::list<CClient*>    _clientsList;

  fd_set                _read_set;
  fd_set                _write_set;
  int                   _sockmax;

public:
  CServer(const int port);
  ~CServer();

private:
  void prepareWriteSet();

public:
  void run();
  void stop();
  void acceptClient();
  void disconnectClient(CClient *c);
};

#endif
