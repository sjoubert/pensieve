#include "psvServer.hpp"

/**
 * Entry point of the server executable
 */
int main(void)
{
  psv::Server server;
  server.Run(7142);

  return 0;
}
