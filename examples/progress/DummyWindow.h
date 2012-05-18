#ifndef __DUMMY_WINDOW_H
#define __DUMMY_WINDOW_H

//Terralib
#include "terralib/qt/widgets/progress/DialogProgressViewer.h"
#include "terralib/qt/widgets/progress/WidgetProgressViewer.h"
#include "terralib/qt/widgets/progress/BarProgressViewer.h"

// QT
#include <QtGui/QWidget>
#include <QtGui/QAction>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QFrame>


class DummyWindow : public QWidget
{
  Q_OBJECT

  public:
    DummyWindow(QWidget* parent = 0);
    ~DummyWindow();

  public slots:
    void showProgressBar();
    void showThreadProgressBar();
    void showWidgetViewer();

  protected:
    QGridLayout* m_mainLayout;
    QPushButton* m_progressPushButton;
    QPushButton* m_progressThreadPushButton;
    QStatusBar* m_statusBar;

    int m_dlgViewerId;
    int m_WidgetViewerId;
    int m_barViewerId;
    te::qt::widgets::DialogProgressViewer* m_dlgViewer;
    te::qt::widgets::WidgetProgressViewer* m_widgetViewer;
    te::qt::widgets::BarProgressViewer* m_barViewer;
};
#endif //__DUMMY_WINDOW_H
