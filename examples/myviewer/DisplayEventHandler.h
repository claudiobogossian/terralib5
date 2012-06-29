
#ifndef __DISPLAY_EVENT_HANDLER_H
#define __DISPLAY_EVENT_HANDLER_H

#include <QtGui/QWidget>

class DisplayEventHandler : public QWidget
{
public:

  DisplayEventHandler(QWidget* parent = 0) : QWidget(parent) {}
  ~DisplayEventHandler() {}

  QPixmap* getDisplayPixmap();
  void setRepaint(bool s);

protected:
  QPoint m_pressPoint;
};
#endif  // __DISPLAY_EVENT_HANDLER_H
