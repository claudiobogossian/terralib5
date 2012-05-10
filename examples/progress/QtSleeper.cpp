#include "ProgressExamples.h"

#include "DummyWindow.h"

// QT
#include <QtGui/QApplication>



void QtSleeper(int argc, char *argv[])
{
  QApplication app(argc, argv);

  //create dummy window and show
  DummyWindow* dW = new DummyWindow(NULL);
  dW->resize(300, 300);
  dW->show();

  app.exec();

  delete dW;
}