#ifndef PSV_SERVER_HPP_20150318261417
#define PSV_SERVER_HPP_20150318261417

#include "psvPensieve.hpp"

#include <cstdlib>
#include <string>

struct MHD_Connection;
struct MHD_Response;

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

private:
  /**
   * Connection handler
   * @param p_callbackParameters Parameters provided at registration
   * @param p_connection Connection to answer to
   * @param p_url Requested URL
   * @param p_method Requested method
   * @param p_version HTTP version string
   * @param p_uploadeData Pointer to uploaded content
   * @param p_uploadeDataSize Size of the uploaded content
   * @param p_requestState Pointer to persistent data across one request
   */
  static int ConnectionHandler(
    void* p_callbackParameters, MHD_Connection* p_connection,
    char const* p_url, char const* p_method, char const* p_version,
    char const* p_uploadData, size_t* p_uploadDataSize,
    void** p_requestState);

  /**
   * Connection handler
   *
   * Simplified and C++-ified version
   * @param p_connection Connection to answer to
   * @param p_url Requested URL
   * @param p_method Requested method
   * @param p_upload If there is uploaded data
   * @param p_uploadData Uploaded data if any
   */
  int ConnectionHandler(MHD_Connection* p_connection,
    std::string const& p_url, std::string const& p_method,
    bool p_upload, std::string const& p_uploadData);

  /// Pensieve instance
  Pensieve m_pensieve;
  /// Empty response
  MHD_Response* m_emptyResponse;
};

}

#endif
