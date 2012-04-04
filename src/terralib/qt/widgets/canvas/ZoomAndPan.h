
#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_ZOOM_AND_PAN_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_ZOOM_AND_PAN_H

// TerraLib
#include "DisplayEventHandler.h"
#include "../../../qt/widgets/canvas/Canvas.h"
#include "../../../geometry.h"

//Qt
#include <QtGui/QBitmap>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class ZoomAndPan : public DisplayEventHandler
      {
        Q_OBJECT

      public:

        ZoomAndPan(QWidget* parent = 0);
        ~ZoomAndPan();

        void drawCursor();

      signals:
        /*!
          \brief This signal is emitted to execute a zoom area.

          \param rec The new rect. 
        */
        void execZoomArea(QRect rec);
        void execZoomIn(QPoint p);
        void execZoomOut(QPoint p);
        void execPan(QPoint p, QPoint);
        void execSelection(QRect);
        void execAddSelection(QRect);

      protected:
        void mouseMoveEvent(QMouseEvent*);
        void mousePressEvent(QMouseEvent*);
        void mouseReleaseEvent(QMouseEvent*);
        void keyPressEvent(QKeyEvent*);
        void keyReleaseEvent(QKeyEvent*);
        void paintEvent(QPaintEvent*);

      protected slots:
        void sizeChanged(QSize);


      private:
        bool m_leftButton;
        bool m_midButton;
        QRect m_rect;
        QPoint m_point;
        QPixmap* m_pixmap;
        //te::qt::widgets::Canvas* m_canvas;
        QColor m_corCursor;
        QColor m_corCursorContour;
        Qt::CursorShape m_cursorShape;
        QBitmap* m_inBitmap;
        QBitmap* m_inMaskBitmap;
        QBitmap* m_outBitmap;
        QBitmap* m_outMaskBitmap;
      };
    }
  }
}

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_ZOOM_AND_PAN_H
