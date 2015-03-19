#include "psvServer.hpp"

#include <microhttpd.h>

#include <chrono>
#include <string>
#include <thread>

namespace psv
{

std::string const Server::GET = "GET";

Server::Server() = default;

Server::~Server() = default;

void Server::Run(unsigned int p_port)
{
  auto daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, p_port,
      nullptr, nullptr, ConnectionHandler, this, MHD_OPTION_END);

  while(true)
  {
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(100ms);
  }

  MHD_stop_daemon(daemon);
}

int Server::ConnectionHandler(
  void* p_callbackParameters, MHD_Connection* p_connection,
  char const* p_url, char const* p_method, char const* p_version,
  char const* p_uploadData, size_t* p_uploadDataSize,
  void** p_requestStat)
{
  auto thisServer = static_cast<Server*>(p_callbackParameters);

  std::string uploadData;
  bool upload = *p_uploadDataSize > 0;
  if(upload)
  {
    uploadData = std::string(p_uploadData, *p_uploadDataSize);
  }

  return thisServer->ConnectionHandler(p_connection, p_url, p_method,
    upload, uploadData);
}

int Server::ConnectionHandler(MHD_Connection* p_connection,
  std::string const& p_url, std::string const& p_method,
  bool p_upload, std::string const& p_uploadData)
{
  if(p_method != GET || p_upload || p_url != "/")
  {
    return MHD_NO;
  }

  std::string jsonPensieve = m_pensieve.ToJSON();

  auto response = MHD_create_response_from_data(
    jsonPensieve.size(), const_cast<char*>(jsonPensieve.c_str()),
    MHD_NO/*free*/, MHD_YES/*copy*/);
  auto returnValue = MHD_queue_response(p_connection, MHD_HTTP_OK, response);
  MHD_destroy_response(response);

  return returnValue;
}

}
