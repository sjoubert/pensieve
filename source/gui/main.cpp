#include "psvPensieveWindow.hpp"

#include <QApplication>

int main(int p_argc, char** p_argv)
{
  QApplication app(p_argc, p_argv);

  psv::PensieveWindow window;
  window.show();

  return app.exec();
}
