#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
  class MainWindow;
}

class OutPutStream;

/*!
 * \brief
 */
class MainWindow: public QMainWindow
{
  Q_OBJECT

public:

  /*!
   * \brief Constructor.
   */
  explicit MainWindow(QWidget *parent = 0);

  ~MainWindow();

  signals:

  public slots :

    /*!
     * \brief .
     */
     void open();

  /*!
   * \brief .
   */
  void newScript();

  /*!
   * \brief .
   */
  void save();

  /*!
   * \brief .
   */
  void run();

  void codeChanged();

protected:

  QString m_fileName;

  OutPutStream* m_out;

//  QTextStream* m_stream;

private:

  Ui::MainWindow* m_ui;   //!<
};

#endif // MAINWINDOW_H
