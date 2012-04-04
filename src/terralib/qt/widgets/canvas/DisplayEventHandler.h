
#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DISPLAYEVENTHANDLER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DISPLAYEVENTHANDLER_H

#include <QtGui/QWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DisplayEventHandler : public QWidget
      {
      public:

        DisplayEventHandler(QWidget* parent = 0) : QWidget(parent){m_key = -1;}
        ~DisplayEventHandler() {}

        QPixmap* getDisplayPixmap();
        void setRepaint(bool s);

      protected:
        int m_key;
        QPoint m_pressPoint;
      };
    }
  }
}

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_DISPLAYEVENTHANDLER_H
