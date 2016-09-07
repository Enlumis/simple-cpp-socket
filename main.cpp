#include	"CServer.hh"

int main()
{
  CServer server(4000);
  server.run();
  return 0;
}
