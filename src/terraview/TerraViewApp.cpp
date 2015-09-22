#include "TerraViewApp.h"
#include <exception>
#include <iostream>

TerraViewApp::TerraViewApp(int& argc, char** argv) :
  QApplication(argc, argv)
{
}

bool TerraViewApp::notify(QObject* receiver, QEvent* event)
{
  bool done = true;

  try
  {
    done = QApplication::notify(receiver, event);
  }
  catch (const std::exception& ex)
  {
    std::cout << "Exception catched\n";
  }
  catch (...)
  {
    std::cout << "Another exception catched\n";
  }
  
  return done;
}
