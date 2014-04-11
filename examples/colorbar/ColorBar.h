
#include "../../src/terralib/color/ColorBar.h"
#include <QDialog>
#include <QPolygon>

class ColorBar : public QDialog
{
public:

  ColorBar(QWidget* parent = 0);
  ~ColorBar();

protected:
  void mouseMoveEvent(QMouseEvent*);
  void mousePressEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent*);
  void keyPressEvent(QKeyEvent*);
  void keyReleaseEvent(QKeyEvent*);
  void wheelEvent(QWheelEvent*);
  void resizeEvent(QResizeEvent*);
  void paintEvent(QPaintEvent*);
  void setColorMsg(const te::color::RGBAColor& color, const double& pos);

private:
  te::color::ColorBar* m_colorBar;
  te::color::RGBAColor m_color;
  Qt::CursorShape m_cursorShape;
  int m_slices;
  QPoint m_point;
  int m_width;
  int m_height;
  int m_hoffset;
  int m_voffset;
  int m_key;
  bool m_capsLock;
  double m_pos;
  double m_editPos;
  double m_copypos;
  QPolygon* m_pol;
  QString m_colorMsg;
  QString m_colorMsg2;
  QString m_colorMsg3;
  QString m_colorMsg4;
};
