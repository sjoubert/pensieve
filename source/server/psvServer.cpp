#include "psvServer.hpp"

#include <microhttpd.h>

#include <chrono>
#include <fstream>
#include <string>
#include <thread>

namespace psv
{

Server::Server(std::string const& p_storageFile):
  m_storageFile(p_storageFile),
  m_emptyResponse(MHD_create_response_from_buffer(0, nullptr, MHD_RESPMEM_PERSISTENT))
{
  // Initialize data from storage file
  std::ifstream storageStream(m_storageFile);
  std::string data((std::istreambuf_iterator<char>(storageStream)),
    std::istreambuf_iterator<char>());

  Pensieve pensieve;
  if(Pensieve::FromJSON(data, pensieve))
  {
    m_pensieve = pensieve;
  }
}

Server::~Server()
{
  MHD_destroy_response(m_emptyResponse);
}

void Server::Run(unsigned int p_port)
{
  auto daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, p_port,
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
  char const* p_url, char const* p_method, char const* /*p_version*/,
  char const* p_uploadData, size_t* p_uploadDataSize,
  void** p_requestInternalData)
{
  auto thisServer = static_cast<Server*>(p_callbackParameters);

  std::string uploadData(p_uploadData, *p_uploadDataSize);
  *p_uploadDataSize = 0;

  return thisServer->ConnectionHandler(p_connection, p_url, p_method,
    uploadData, p_requestInternalData);
}

int Server::ConnectionHandler(MHD_Connection* p_connection,
  std::string const& p_url, std::string const& p_method,
  std::string const& p_uploadData, void** p_requestInternalData)
{
  // 404 not found
  if(p_url != "/")
  {
    return MHD_queue_response(
      p_connection, MHD_HTTP_NOT_FOUND, m_emptyResponse);
  }

  // 405 method not allowed
  if(p_method != MHD_HTTP_METHOD_GET && p_method != MHD_HTTP_METHOD_PUT)
  {
    return MHD_queue_response(
      p_connection, MHD_HTTP_METHOD_NOT_ALLOWED, m_emptyResponse);
  }

  // GET
  if(p_method == MHD_HTTP_METHOD_GET)
  {
    std::string jsonPensieve = m_pensieve.ToJSON();
    auto response = MHD_create_response_from_buffer(
      jsonPensieve.size(), const_cast<char*>(jsonPensieve.c_str()),
      MHD_RESPMEM_MUST_COPY);
    auto returnValue = MHD_queue_response(p_connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return returnValue;
  }
  else // PUT
  {
    // New request
    if(*p_requestInternalData == nullptr)
    {
      // Buffer id, use a pointer value as UID
      *p_requestInternalData = new int;
      return MHD_YES;
    }

    if(p_uploadData.empty() == false)
    {
      // Append data to buffer
      m_buffers[*p_requestInternalData] += p_uploadData;
      return MHD_YES;
    }
    else
    {
      // End of data, handle PUT request
      int httpCode = MHD_HTTP_CREATED;
      Pensieve pensieve;
      if(Pensieve::FromJSON(m_buffers[*p_requestInternalData], pensieve))
      {
        // Update memory data
        m_pensieve = pensieve;

        // Persist data if possible (otherwise write will just silently fail)
        std::ofstream storageStream(m_storageFile);
        storageStream << m_buffers[*p_requestInternalData];
      }
      else
      {
        httpCode = MHD_HTTP_BAD_REQUEST;
      }

      // Clean
      m_buffers.erase(*p_requestInternalData);
      delete static_cast<int*>(*p_requestInternalData);

      return MHD_queue_response(p_connection, httpCode, m_emptyResponse);
    }
  }
}

}
