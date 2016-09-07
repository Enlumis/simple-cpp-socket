#ifndef CSERVER_H_
# define CSERVER_H_

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
  int		_port;
  int		_socket;
  //CLIENT LIST
  fd_set	_read_set;
  fd_set	_write_set;
  int		_sockmax;
  bool		_exit;

public:
  CServer(const int port);
  ~CServer();

public:
  void run();
  void stop();
  void acceptClient();
  void disconnectClient();
};

#endif
