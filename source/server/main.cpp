#include "psvServer.hpp"

#include "psvConfigure.hpp"
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

/**
 * Entry point of the server executable
 * \param p_argc Parameters count
 * \param p_argv Parameters values
 * \return Exit code
 */
int main(int p_argc, char** p_argv)
{
  // Command line arguments
  namespace bpo = boost::program_options;
  unsigned int port;
  std::string storageFile;

  bpo::options_description options("Usage: pensieved [Options]\nOptions");
  options.add_options()
    ("help,h", "Display help message")
    ("version,v", "Display version")
    ("storage-file,s", bpo::value<std::string>(&storageFile)
      ->default_value("")->value_name("file"),
      "Data storage file")
    ("port,p", bpo::value<unsigned int>(&port)
      ->default_value(7142)->value_name("number"),
      "Port to listen to");

  bpo::variables_map values;
  try
  {
    bpo::store(bpo::parse_command_line(p_argc, p_argv, options), values);
    bpo::notify(values);
  }
  catch(std::exception const& e)
  {
    std::cerr << "Error: " << e.what() << std::endl << options;
    return 1;
  }

  if(values.count("help"))
  {
    std::cout << options;
    return 0;
  }
  if(values.count("version"))
  {
    std::cout << psv::PENSIEVE_VERSION << std::endl;
    return 0;
  }

  // Start application
  psv::Server server(values["storage-file"].as<std::string>());
  server.Run(port);

  return 0;
}
