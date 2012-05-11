#ifndef __DUMMY_WINDOW_H
#define __DUMMY_WINDOW_H

// QT
#include <QtGui/QWidget>
#include <QtGui/QAction>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>


class DummyWindow : public QWidget
{
  Q_OBJECT

  public:
    DummyWindow(QWidget* parent = 0);
    ~DummyWindow();

  public slots:
    void showProgressBar();
    void showThreadProgressBar();
    void showThreadProgressBar2();
    void showStatusProgressBar();

  protected:
    QGridLayout* m_mainLayout;
    QPushButton* m_progressPushButton;
    QPushButton* m_progressThreadPushButton;
    QPushButton* m_progressThreadPushButton2;
    QPushButton* m_progressStatusBar;
    QStatusBar*  m_statusBar;

};
#endif //__DUMMY_WINDOW_H
