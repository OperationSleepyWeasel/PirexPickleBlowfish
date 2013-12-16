#include <QtWidgets>
#include <QObject>
#include <iostream>
#include "logic/Dollar.hpp"
//#include "gui/BoardSample.hpp"
#include "gui/MainWindowSample.hpp"
#include "Logger.hpp"
#include <boost/log/trivial.hpp>

int main(int argc, char* argv[])
{
  initLogging();

  BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
  BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
  BOOST_LOG_TRIVIAL(info) << "An informational severity message";
  BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
  BOOST_LOG_TRIVIAL(error) << "An error severity message";
  BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

  QApplication app(argc, argv);
  //BoardSample boardSample;
  //boardSample.show();
  MainWindowSample mainWindowSample;
  QObject::connect (mainWindowSample.getQuitGameButton(), SIGNAL(clicked()), &app, SLOT(quit()));
  mainWindowSample.show();
  Dollar* dollar = new Dollar(5);
  dollar->times(2);
  std::cout << "Hello World!" << std::endl;
  return app.exec();
}
