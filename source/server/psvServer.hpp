#ifndef PSV_SERVER_HPP_20150318261417
#define PSV_SERVER_HPP_20150318261417

namespace psv
{

/**
 * HTTP server that respond to requests on the pensieve data
 */
class Server
{
public:
  /**
   * Construct a server
   */
  Server();

  Server(Server const&) = delete;

  /**
   * Destruct the server
   */
  ~Server();

  Server& operator=(Server const&) = delete;

  /**
   * Start the server and answer to requests
   * @param p_port Port to listen to
   */
  void Run(unsigned int p_port);
};

}

#endif
