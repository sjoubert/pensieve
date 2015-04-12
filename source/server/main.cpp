#include "psvServer.hpp"

#include "psvConfigure.hpp"
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

#if PSV_POSIX
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
#endif

namespace psv
{

/**
 * Return status of the daemonization process
 */
enum class DaemonStatus
{
  /// Daemonization failed
  eFailed,
  /// Daemonization succeeded and current process is the daemon
  eDaemon,
  /// Daemonization succeeded and current process is the parent process
  eParent
};

/**
 * Make the application a daemon if enabled
 * @return Daemon status
 */
DaemonStatus Daemonize()
{
#ifdef PSV_POSIX
  pid_t pid = fork();

  // Already a daemon, parent pid is 1(init)
  if(getppid() == 1)
  {
    return DaemonStatus::eDaemon;
  }

  if(pid < 0)
  {
    return DaemonStatus::eFailed;
  }
  else if(pid == 0)
  {
    // Correctly detach daemon in its own session group
    if(setsid() < 0)
    {
      return DaemonStatus::eFailed;
    }

    // Reset working directory
    if(chdir("/") < 0)
    {
      return DaemonStatus::eFailed;
    }

    // Close inherited file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Reopen file descriptors to a safe place
    int devNull = open("/dev/null", O_RDWR, 0);
    if(devNull != -1)
    {
      dup2(devNull, STDIN_FILENO);
      dup2(devNull, STDOUT_FILENO);
      dup2(devNull, STDERR_FILENO);

      if(devNull > 2)
      {
        close(devNull);
      }
    }

    // Reset file creation mask to 750
    umask(027);

    return DaemonStatus::eDaemon;
  }
  else
  {
    return DaemonStatus::eParent;
  }
#else
  return DaemonStatus::eDaemon;
#endif
}

/**
 * Run the application
 * @param p_port Port to listen to
 * @param p_storageFile Storage file for data persistence
 * @param p_daemonize Should the application be daemonized
 * @return Application exit code
 */
int Run(unsigned int p_port, std::string const& p_storageFile, bool p_daemonize)
{
  // Daemonization
  auto status = DaemonStatus::eDaemon;
  if(p_daemonize)
  {
    status = Daemonize();
  }

  switch(status)
  {
    case DaemonStatus::eFailed:
    {
      std::cerr << "Daemonization failed" << std::endl;
      return 1;
    }
    case DaemonStatus::eDaemon:
    {
      // Start application
      psv::Server server(p_storageFile);
      server.Run(p_port);
      break;
    }
    case DaemonStatus::eParent:
    {
      break;
    }
  }

  return 0;
}

}

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

  bpo::options_description options("Usage: pensieved [Options]\nOptions");
  options.add_options()
    ("help,h", "Display help message")
    ("version,v", "Display version")
    ("storage-file,s",
      bpo::value<std::string>()->default_value("")->value_name("file"),
      "Data storage file")
#ifdef PSV_POSIX
    ("daemon,d", "Start the application as a daemon")
#endif
    ("port,p",
      bpo::value<unsigned int>()->default_value(7142)->value_name("number"),
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

  return psv::Run(values["port"].as<unsigned int>(),
    values["storage-file"].as<std::string>(),
    values.count("daemon"));
}
