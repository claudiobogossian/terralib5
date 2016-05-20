#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
  class MainWindow;
}

/*!
 * \brief
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:

    /*!
     * \brief Constructor.
     */
    explicit MainWindow(QWidget *parent = 0);

  signals:

  public slots:

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

  private:

    Ui::MainWindow* m_ui;   //!<
};

#endif // MAINWINDOW_H
