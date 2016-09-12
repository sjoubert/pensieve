#ifndef PSV_SERVER_HPP_20150318261417
#define PSV_SERVER_HPP_20150318261417

#include "psvPensieve.hpp"

#include <cstdlib>
#include <map>
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
   * \param p_storageFile Storage file to use for data persistence
   */
  Server(std::string const& p_storageFile = "");

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
   * @param p_requestInternalData Pointer to persistent data across one request
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
   * @param p_uploadData Uploaded data if any
   * @param p_requestInternalData Pointer to persistent data across one request
   */
  int ConnectionHandler(MHD_Connection* p_connection,
    std::string const& p_url, std::string const& p_method,
    std::string const& p_uploadData, void** p_requestInternalData);

  /**
   * Handler for standard interrupt (SIGINT) signal, stopping the deamon loop
   */
  static void InterruptSignal(int);

  /// Pensieve instance
  Pensieve m_pensieve;
  /// Storage file
  std::string m_storageFile;
  /// Buffers
  std::map<void*, std::string> m_buffers;
  /// Empty response
  MHD_Response* m_emptyResponse;
  /// Daemon loop sentinel
  static bool s_running;
};

}

#endif
