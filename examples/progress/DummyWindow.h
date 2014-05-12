#ifndef __DUMMY_WINDOW_H
#define __DUMMY_WINDOW_H

//Terralib
#include "terralib/qt/widgets/progress/ProgressViewerDialog.h"
#include "terralib/qt/widgets/progress/ProgressViewerWidget.h"
#include "terralib/qt/widgets/progress/ProgressViewerBar.h"

// QT
#include <QWidget>
#include <QAction>
#include <QGridLayout>
#include <QPushButton>
#include <QStatusBar>
#include <QFrame>


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
    te::qt::widgets::ProgressViewerDialog* m_dlgViewer;
    te::qt::widgets::ProgressViewerWidget* m_widgetViewer;
    te::qt::widgets::ProgressViewerBar* m_barViewer;
};
#endif //__DUMMY_WINDOW_H
