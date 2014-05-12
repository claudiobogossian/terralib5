#include "ProgressExamples.h"

#include "DummyWindow.h"

// QT
#include <QApplication>


void QtSleeper(int argc, char *argv[])
{
  QApplication app(argc, argv);

  //create dummy window and show
  DummyWindow* dW = new DummyWindow(NULL);
  dW->show();

  app.exec();

  delete dW;
}
