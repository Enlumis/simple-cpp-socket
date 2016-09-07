#include	"CServer.hh"

CServer::CServer(const int port)
  : _port(port)
{
	int					sock;
	struct sockaddr_in	sin;
	struct protoent		*protocol;
	int					boole = 1;
	int  				maxconnectionsocket = 6;

	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	protocol = getprotobyname("TCP");
	if (protocol == NULL)
		throw CServerException("get protocol check file /etc/protocols");
	if ((sock = socket(PF_INET, SOCK_STREAM, protocol->p_proto)) == -1)
		throw CServerException("socket method error");
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &boole, sizeof(boole)) == -1)
		throw CServerException("setsockopt method error");
	if (bind(sock, (struct sockaddr*)&sin, sizeof(sin)) == -1)
		throw CServerException("bind method error");
	if (listen(sock, maxconnectionsocket) == -1)
		throw CServerException("listen method error");

	_socket = sock;

	// init client list

	FD_ZERO(&_write_set);
	FD_ZERO(&_read_set);
	FD_SET(sock, &_read_set);

	_exit = false;
}

CServer::~CServer()
{
	close(_socket);
}

void CServer::stop() {
	_exit = true;
}

void CServer::acceptClient() {
	int			csock;
	struct sockaddr_in	caddr;
	socklen_t		clen;

	clen = sizeof(caddr);
	if ((csock = accept(server->socket, (struct sockaddr*)&caddr, &clen)) != -1)
	{
		if (!add_client(server, csock, caddr))
		{
			FD_SET(csock, &server->read_set);
			if (csock > server->sockmax)
				server->sockmax = csock;
			else
				server->sockmax = eval_socket_max(server);
		}
	}
	else
		my_error("accept", 1);
}
void CServer::disconnectClient(Client &c) {
	this->clientList.remove(c);
	printf("[Server][%s][Disconnect]\n", c.getIpAdress());
	c.CloseSocket();
}
void CServer::doWriteClient(Client &c) {

}
void CServer::doReadClient(Client &c) {

}

void CServer::run()
{
	std::cout << "Server run on " << this->_port << " port" << std::endl;
	fd_set	cp_read;
	fd_set	cp_write;

	while (!_exit)
	{
		cp_read = this->_read_set;
		cp_write = prepare_write_set(server);
		if (select(server->sockmax + 1, &cp_read, &cp_write, NULL, NULL) == -1)
		{
			if (errno == EINTR)
				return ;
			throw CServerException("select method error");
		}
		if (FD_ISSET(server->socket, &cp_read))
			this->acceptClient();

		t_client	*client;
		t_client	*next;

		client = server->clientlist;
		while (client != NULL)
		{
			next = client->next;
			if (FD_ISSET(client->socket, &cp_write)
			&& this->doWriteClient(client))
			{
				FD_CLR(client->socket, &this->_write_set);
				FD_CLR(client->socket, &this->_read_set);
				this->disconnectClient(client);
			}
			else if (FD_ISSET(client->socket, &cp_read)
			&& this->doReadClient(client))
			{
				if (client->is_in_queue)
					FD_CLR(client->socket, &this->_write_set);
				FD_CLR(client->socket, &this->_read_set);
				this->disconnectClient(client);
			}
			client = next;
		}
	}
}

static int	do_write_client(t_client *client)
{
  int		need_write;

  need_write = 0;
  if (client->write_buf->start == client->write_buf->bufferend)
    client->write_buf->start = client->write_buf->buffer;
  if (client->write_buf->end > client->write_buf->start)
    need_write = client->write_buf->end - client->write_buf->start;
  else
    need_write = client->write_buf->bufferend - client->write_buf->start;
  send(client->socket, client->write_buf->start, need_write, 0);
  client->write_buf->data_size -= need_write;
  client->write_buf->start += need_write;
  return (0);
}

static int	do_read_client(t_client *c, t_server *server)
{
  int		rd;
  int		need_read;

  if (c->read_buf->data_size >= SIZE_RING_BUF)
    return (my_error("read ring buffer overflow\n", 0));
  if (c->read_buf->start == c->read_buf->bufferend)
    c->read_buf->start = c->read_buf->buffer;
  if (c->read_buf->end == c->read_buf->bufferend)
    c->read_buf->end = c->read_buf->buffer;
  if (c->read_buf->end < c->read_buf->start)
    need_read = c->read_buf->start - c->read_buf->end;
  else
    need_read = c->read_buf->buffer + SIZE_RING_BUF - c->read_buf->end;
  rd = recv(c->socket, c->read_buf->end, need_read, 0);
  if (rd == -1)
    return (my_error("recv", 1));
  c->read_buf->end += rd;
  c->read_buf->data_size += rd;
  if (rd == 0)
    return (my_error("client unreachable\n", 0));
  if (c->read_buf->data_size >= sizeof(t_packet))
    return (handle_client_packets(c, server));
  return (0);
}

static fd_set	prepare_write_set(t_server *server)
{
  t_client	*client;

  client = server->clientlist;
  while (client != NULL)
    {
      if (client->write_buf->data_size >= sizeof(t_packet))
	{
	  if (client->is_in_queue == 0)
	    FD_SET(client->socket, &server->write_set);
	  client->is_in_queue = 1;
	}
      else if (client->is_in_queue)
	{
	  client->is_in_queue = 0;
	  FD_CLR(client->socket, &server->write_set);
	}
      client = client->next;
    }
  return (server->write_set);
}

static int		eval_socket_max(t_server *server)
{
  t_client		*client;
  int			sockmax;

  client = server->clientlist;
  sockmax = server->socket;
  while (client != NULL)
    {
      if (sockmax < client->socket)
	sockmax = client->socket;
      client = client->next;
    }
  return (sockmax);
}

CServerException::CServerException(const std::string &error) : m_error(error){}
CServerException::~CServerException() throw(){}
const char* CServerException::what() const throw()
{
  return m_error.c_str();
}
