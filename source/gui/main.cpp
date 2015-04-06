#include "psvPensieveWindow.hpp"

#include "psvConfigure.hpp"
#include "psvSettingsDialog.hpp"
#include <boost/program_options.hpp>
#include <QApplication>
#include <QSettings>
#include <iostream>

/**
 * Entry point of the graphical application
 * \param p_argc Parameters count
 * \param p_argv Parameters values
 * \return Exit code
 */
int main(int p_argc, char** p_argv)
{
  // Command line arguments
  namespace bpo = boost::program_options;

  bpo::options_description options(
    QObject::tr("Usage: qpensieve [Options] [Qt application options]\nOptions")
    .toStdString());
  options.add_options()
    ("help,h", QObject::tr("Display help message").toStdString().c_str())
    ("version,v", QObject::tr("Display version").toStdString().c_str());

  bpo::variables_map values;
  try
  {
    auto parsed = bpo::command_line_parser(p_argc, p_argv).options(options)
      .allow_unregistered().run();
    bpo::store(parsed, values);
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
  QApplication app(p_argc, p_argv);

  psv::PensieveWindow window;
  window.setWindowState(Qt::WindowMaximized);
  window.setHidden(QSettings().value(
    psv::SettingsDialog::Settings::START_HIDDEN, false).toBool());

  return app.exec();
}
