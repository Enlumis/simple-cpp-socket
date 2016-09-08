#include	<signal.h>
#include	"CServer.hh"

static void	exit_server(int sig)
{
  (void)sig;
  gg_exit = true;
}

int main()
{
  signal(SIGINT, exit_server);
  CServer server(4000);
  server.run();
  std::cout << std::endl << coutprefix << "Server Terminated" << std::endl;
  return 0;
}
