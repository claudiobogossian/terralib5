
#ifndef __MOUSE_HANDLER_H
#define __MOUSE_HANDLER_H

// TerraLib
#include "DisplayEventHandler.h"
#include <terralib/qt/widgets/canvas/Canvas.h>
#include <terralib/geometry/Geometry.h>

//Qt
#include <QtGui/QPixmap>
#include <QtGui/QCursor>

class MouseHandler : public DisplayEventHandler
{
  Q_OBJECT

public:
  enum MouseMode{ZoomInMode, ZoomOutMode, PanMode, SelectionMode, AddSelectionMode, ToggleSelectionMode};

  MouseHandler(QWidget* parent = 0);
  ~MouseHandler();

  void drawCursor();
  void setCursor();
  MouseMode getMode();
  void setMode(MouseMode m);

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
  void execToggleSelection(QRect);

protected:
  void mouseMoveEvent(QMouseEvent*);
  void mousePressEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent*);
  void keyPressEvent(QKeyEvent*);
  void paintEvent(QPaintEvent*);

protected slots:
  void sizeChanged(QSize);


private:
  MouseMode m_mouseMode;
  MouseMode m_mouseOldMode;
  bool m_panEnd;
  QRect m_rect;
  QPoint m_point;
  QPixmap* m_pixmap;
  QColor m_corCursor;
  QColor m_corCursorContour;
  QPixmap* m_zoomInPixmap;
  QPixmap* m_zoomOutPixmap;
  QPixmap* m_panPixmap;
  QPixmap* m_selectionPixmap;
  QPixmap* m_addSelectionPixmap;
  QPixmap* m_toggleSelectionPixmap;
};
#endif  // __MOUSE_HANDLER_H
