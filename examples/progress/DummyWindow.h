#ifndef __DUMMY_WINDOW_H
#define __DUMMY_WINDOW_H

// QT
#include <QtGui/QWidget>
#include <QtGui/QAction>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>


class DummyWindow : public QWidget
{
  Q_OBJECT

  public:
    DummyWindow(QWidget* parent = 0);
    ~DummyWindow();

  public slots:
    void showProgressBar();
    void showThreadProgressBar();

  protected:
    QGridLayout* m_mainLayout;
    QPushButton* m_progressPushButton;
    QPushButton* m_progressThreadPushButton;

};
#endif //__DUMMY_WINDOW_H
