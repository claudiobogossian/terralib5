#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
  class MainWindow;
}


class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);

  signals:

  public slots:

    void open();

    void newScript();

    void save();

    void run();

  private:

    Ui::MainWindow* m_ui;
};

#endif // MAINWINDOW_H
