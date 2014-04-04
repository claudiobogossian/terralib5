/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/qt/widgets/canvas/TextFrame.h

  \brief The frame data....
*/

// TerraLib
#include "TextFrame.h"
#include "LayoutEditor.h"

#include <terralib/geometry.h>
//#include <terralib/postgis.h>
//#include <terralib/dataaccess.h>
#include <terralib/srs.h>
#include <terralib/qt/widgets/canvas/Canvas.h>

// Qt
#include <QApplication>
#include <QAction>
#include <QMenu>

#include <QPainter>
#include <QColor>
#include <QRect>
#include <QWidget>
#include <QColorDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QMenu>
#include <QAction>
#include <QToolTip>
#include <QResizeEvent>
#include <QBoxLayout>
#include <QCoreApplication>
#include <QtCore/qmath.h>

te::qt::widgets::TextFrame::TextFrame(const QPointF& p, const QString& txt, const int& pointSize, te::qt::widgets::LayoutEditor* me, Qt::WindowFlags f) :
  te::qt::widgets::Frame(me, f),
  m_cor(Qt::magenta),
  m_point(p),
  m_angle(0),
  m_family("Linux Libertine G"),
  m_stretch(100),
  m_align(Qt::AlignHCenter),
  m_style(QFont::StyleNormal),
  m_overline(false),
  m_strikeOut(false),
  m_underline(false),
  m_letterSpacing(100),
  m_wordSpacing(0),
  m_capitalization(QFont::MixedCase),
  m_weight(QFont::Normal),
  m_text(txt)
{
  setWindowTitle("TextFrame");
  setMouseTracking(true);
  //m_layoutEditor = (te::qt::widgets::LayoutEditor*)(parent->parent());
  //QMatrix matrix = m_layoutEditor->getMatrixPaperViewToVp();
  //QRectF pRect = matrix.mapRect(m_layoutEditor->getPaperViewRect());
  m_font = new QFont(m_family);
  m_font->setStretch(m_stretch);
  m_font->setStyle(m_style);
  m_font->setOverline(m_overline);
  m_font->setStrikeOut(m_strikeOut);
  m_font->setUnderline(m_underline);
  m_font->setLetterSpacing(QFont::PercentageSpacing, m_letterSpacing);
  m_font->setWordSpacing(m_wordSpacing);
  m_font->setCapitalization(m_capitalization);
  m_font->setWeight(m_weight);

  setPointSize(pointSize);
  adjust();

  show();
  createMenu();
}

te::qt::widgets::TextFrame::TextFrame(const TextFrame& rhs) :
  te::qt::widgets::Frame(rhs.m_layoutEditor, rhs.windowFlags()),
  m_cor(rhs.m_cor),
  m_point(rhs.m_point + QPointF(20, 20)),
  m_angle(rhs.m_angle),
  m_family(rhs.m_family),
  m_stretch(rhs.m_stretch),
  m_align(rhs.m_align),
  m_style(rhs.m_style),
  m_overline(rhs.m_overline),
  m_strikeOut(rhs.m_strikeOut),
  m_underline(rhs.m_underline),
  m_letterSpacing(rhs.m_letterSpacing),
  m_wordSpacing(rhs.m_wordSpacing),
  m_capitalization(rhs.m_capitalization),
  m_weight(rhs.m_weight),
  m_text(rhs.m_text),
  m_pointSize(rhs.m_pointSize)
{
  te::qt::widgets::Frame::operator=(rhs);
  setWindowTitle("TextFrame");
  setMouseTracking(true);

  //m_layoutEditor = rhs.m_layoutEditor,

  m_font = new QFont(*rhs.m_font);
  m_font->setStretch(m_stretch);
  m_font->setStyle(m_style);
  m_font->setOverline(m_overline);
  m_font->setStrikeOut(m_strikeOut);
  m_font->setUnderline(m_underline);
  m_font->setLetterSpacing(QFont::PercentageSpacing, m_letterSpacing);
  m_font->setWordSpacing(m_wordSpacing);
  m_font->setCapitalization(m_capitalization);
  m_font->setWeight(m_weight);
  m_font->setFamily(m_family);

  adjust();

  show();
  createMenu();
}

te::qt::widgets::TextFrame::~TextFrame()
{
  hide();
  delete m_font;
  delete m_setTextAction;
  delete m_setFamilyAction;
  delete m_setColorAction;
  delete m_showTooTipAction;
  delete m_hideTooTipAction;
  delete m_menu;
}

te::qt::widgets::TextFrame& te::qt::widgets::TextFrame::operator=(const TextFrame& rhs)
{
  if(this != &rhs)
  {
    te::qt::widgets::Frame::operator=(rhs);
    setWindowTitle("TextFrame");
    m_cor = rhs.m_cor;
    m_point = rhs.m_point;
    m_angle = rhs.m_angle;
    m_family = rhs.m_family;
    m_stretch = rhs.m_stretch;
    m_align = rhs.m_align;
    m_style = rhs.m_style;
    m_overline = rhs.m_overline;
    m_strikeOut = rhs.m_strikeOut;
    m_underline = rhs.m_underline;
    m_letterSpacing = rhs.m_letterSpacing;
    m_wordSpacing = rhs.m_wordSpacing;
    m_capitalization = rhs.m_capitalization;
    m_weight = rhs.m_weight;
    m_text = rhs.m_text;
    m_pointSize = rhs.m_pointSize;
    m_font = new QFont(*rhs.m_font);
    adjust();

    show();
    createMenu();
  }

  return *this;
}

void te::qt::widgets::TextFrame::createMenu()
{
    m_menu = new QMenu(this);
    m_setFamilyAction = m_menu->addAction("Font Family...");
    m_setColorAction = m_menu->addAction("Text Color...");
    m_setTextAction = m_menu->addAction("Change Text...");
    m_showTooTipAction = m_menu->addAction("Show Tool Tip");
    m_hideTooTipAction = m_menu->addAction("Hide Tool Tip");
}

void te::qt::widgets::TextFrame::setText(const QString& t)
{
  m_text = t;
}

QString te::qt::widgets::TextFrame::getText()
{
  return m_text;
}

QRectF te::qt::widgets::TextFrame::getTextRect()
{
  return m_frameRect;
}

QPixmap* te::qt::widgets::TextFrame::getPixmap()
{
  return &m_pixmap;
}

void te::qt::widgets::TextFrame::calculateTextRect(QRectF& rt)
{
  setPointSize(m_pointSize);

  if(m_text.isEmpty())
    m_text = " ";

  QPixmap pix(10, 10);
  QPainter painter(&pix);
  painter.setFont(*m_font);

  // nao mude nada na linha abaixo
  rt = painter.boundingRect(QRect(0, 0, 100000, 100000), Qt::AlignLeft, m_text); // nao mude nada aqui!!!
  // nao mude nada na linha acima

  painter.end();
}

void te::qt::widgets::TextFrame::adjust()
{
  adjustSize(m_point);
}

void te::qt::widgets::TextFrame::adjustSize(QPointF& p)
{
  m_point = p;
  QMatrix matrix = m_layoutEditor->getMatrixPaperViewToVp();

  QRectF br;
  calculateTextRect(br);
  QRectF r = matrix.inverted().mapRect(br);
  m_frameRect = QRectF(m_point.x(), m_point.y(), r.width(), r.height());

  // calcular o tamanho do widget
  r = matrix.mapRect(m_frameRect);
  QMatrix m;
  m.rotate(m_angle);
  m.translate(-r.center().x(), -r.center().y());
  QPolygon poly = m.mapToPolygon(r.toRect());
  m.reset();
  m.translate(r.center().x(), r.center().y());
  poly = m.map(poly);
  QRect widgetRec = poly.boundingRect();

  // aumente o tamanho do pixmap para se casar com o widget
  m_pixmap = QPixmap(widgetRec.width() + 1, widgetRec.height() + 2);
  m_pixmap.fill(Qt::transparent);

  QWidget::resize(widgetRec.width(), widgetRec.height());
  QWidget::move(widgetRec.left(), widgetRec.top());

  // calculate selection points
  QRectF rec = matrix.mapRect(m_frameRect);

  double pw = rec.width();
  double ph = rec.height();
  int ipw = qRound(pw);
  int iph = qRound(ph);
  int ipw2 = qRound(pw/2.);
  int iph2 = qRound(ph/2.);
  double w = 5;
  double h = 5;

  if(pw < w*5)
    w = pw / 5.;
  if(ph < h*5)
    h = ph / 5.;

  int iw = qRound(w);
  int ih = qRound(h);
  int iw2 = qRound(w/2.);
  int ih2 = qRound(h/2.);

  m_recSel1 = QRect(0, 0, iw, ih);
  m_recSel2 = QRect(ipw2 - iw2, 0, iw, ih);
  m_recSel3 = QRect(ipw - iw, 0, iw, ih);
  m_recSel4 = QRect(ipw - iw, iph2 - ih2, iw, ih);
  m_recSel5 = QRect(ipw - iw, iph - ih, iw, ih);
  m_recSel6 = QRect(ipw2 - iw2, iph - ih, iw, ih);
  m_recSel7 = QRect(0, iph - ih, iw, ih);
  m_recSel8 = QRect(0, iph2 - ih2, iw, ih);
  m_recSel10 = QRect(0, 0, ipw, iph);

  draw();
}

// NOTA: apos setar qualquer propriedade chame o metodo adjustSize() para desenhar o texto

QString te::qt::widgets::TextFrame::getFamily()
{
  return m_family;
}

void te::qt::widgets::TextFrame::setFamily(QString family)
{
  m_family = family;
  m_font->setFamily(m_family);
}

int te::qt::widgets::TextFrame::getPointSize()
{
  return m_pointSize;
}

void te::qt::widgets::TextFrame::setPointSize(int pointSize)
{
  m_pointSize = pointSize;

  QMatrix matrix = m_layoutEditor->getMatrixPaperViewToVp();
  double h = m_pointSize * 25.4 / 72.;
  double vtam = h * matrix.m11();
  int ps = qRound(vtam);
  if(ps < 1)
    ps = 1;

  m_font->setPixelSize(ps);
}

int te::qt::widgets::TextFrame::getStretch()
{
  return m_stretch;
}

void te::qt::widgets::TextFrame::setStretch(int s)
{
  m_stretch = s;
  m_font->setStretch(m_stretch);
}

int te::qt::widgets::TextFrame::getAlign()
{
  return m_align;
}

void te::qt::widgets::TextFrame::setAlign(int align)
{
  m_align = align;
}

QFont::Style te::qt::widgets::TextFrame::getStyle()
{
  return m_style;
}

void te::qt::widgets::TextFrame::setStyle(QFont::Style style)
{
  m_style = style;
  m_font->setStyle(m_style);
}

QFont::Weight te::qt::widgets::TextFrame::getWeight()
{
  return m_weight;
}

void te::qt::widgets::TextFrame::setWeight(QFont::Weight weight)
{
  m_weight = weight;
  m_font->setWeight(m_weight);
}

QFont::Capitalization te::qt::widgets::TextFrame::getCapitalization()
{
  return m_capitalization;
}

void te::qt::widgets::TextFrame::setCapitalization(QFont::Capitalization cap)
{
  m_capitalization = cap;
  m_font->setCapitalization(m_capitalization);
}

bool te::qt::widgets::TextFrame::getOverline()
{
  return m_overline;
}

void te::qt::widgets::TextFrame::setOverline(bool b)
{
  m_overline = b;
  m_font->setOverline(m_overline);
}

bool te::qt::widgets::TextFrame::getStrikeOut()
{
  return m_strikeOut;
}

void te::qt::widgets::TextFrame::setStrikeOut(bool b)
{
  m_strikeOut = b;
  m_font->setStrikeOut(m_strikeOut);
}

bool te::qt::widgets::TextFrame::getUnderline()
{
  return m_underline;
}

void te::qt::widgets::TextFrame::setUnderline(bool b)
{
  m_underline = b;
  m_font->setUnderline(m_underline);
}

double te::qt::widgets::TextFrame::getLetterSpacing()
{
  return m_letterSpacing;
}

void te::qt::widgets::TextFrame::setLetterSpacing(double spacing)
{
  m_letterSpacing = spacing;
  m_font->setLetterSpacing(QFont::PercentageSpacing, m_letterSpacing);
}

double te::qt::widgets::TextFrame::getWordSpacing()
{
  return m_wordSpacing;
}

void te::qt::widgets::TextFrame::setWordSpacing(double spacing)
{
  m_wordSpacing = spacing;
  m_font->setWordSpacing(m_wordSpacing);
}

void te::qt::widgets::TextFrame::draw()
{
  QRectF rec = m_layoutEditor->getMatrix().mapRect(m_frameRect);

  QPainter painter(&m_pixmap);
  painter.setPen(m_cor);
//  painter.setBrush(Qt::cyan);
  painter.setFont(*m_font);

  double rad = fabs(m_angle * 3.141593 / 180);
  QRect br(0, 0, qRound(rec.width()), qRound(rec.height()));

  QMatrix matrix = getAuxMatrix();
  painter.setMatrix(matrix);
  painter.drawText(br, m_align, m_text);  
  painter.end();

  update();
}

void te::qt::widgets::TextFrame::rubberBand()
{
  // faz tudo em coordenadas do dispositivo

  QPixmap* pixmap = m_layoutEditor->getDraftPixmap();
  QPainter painter(pixmap);
  painter.setBrush(Qt::NoBrush);
  painter.setPen(Qt::red);

  // erase old 
  if(m_copyAuxFrameRect.isValid())
  {
    QRectF rec = m_copyAuxFrameRect;
    rec.moveCenter(QPointF(0, 0));

    painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    QMatrix matrix;
    matrix.translate(m_copyAuxFrameRect.center().x(), m_copyAuxFrameRect.center().y());
    matrix.rotate(m_auxAngle);
    painter.setMatrix(matrix);
    painter.drawRect(rec);
  }

  // show new
  QRectF rec = m_auxFrameRect;
  rec.moveCenter(QPointF(0, 0));

  painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
  QMatrix matrix;
  matrix.translate(m_auxFrameRect.center().x(), m_auxFrameRect.center().y());
  matrix.rotate(m_angle);
  painter.setMatrix(matrix);
  painter.drawRect(rec);
  painter.end();

  m_auxAngle = m_angle;
  m_copyAuxFrameRect = m_auxFrameRect;
          
  m_layoutEditor->update();
}

void te::qt::widgets::TextFrame::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.drawPixmap(0, 0, m_pixmap);
}

void te::qt::widgets::TextFrame::showSelectionPoints()
{
  m_pixmap.fill(Qt::transparent);
  draw();

  QPainter painter(&m_pixmap);
  QMatrix matrix = getAuxMatrix();
  painter.setMatrix(matrix);

  painter.setPen(Qt::blue);
  painter.setBrush(QColor(0, 0, 255, 24));
  painter.drawRect(QRect(m_recSel10.left(), m_recSel10.top(), m_recSel10.width(), m_recSel10.height()-1));  

  // draw selection points
  painter.setBrush(QColor(0, 0, 255, 150));
  painter.drawRect(m_recSel1);
  painter.drawRect(m_recSel3);
  painter.drawRect(m_recSel5);
  painter.drawRect(m_recSel7);

  painter.drawRect(m_recSel2);
  painter.drawRect(m_recSel6);

  painter.drawRect(m_recSel4);
  painter.drawRect(m_recSel8);

  if(!(fabs(m_angle) == 0 || fabs(m_angle) == 90 || fabs(m_angle) == 180) || (fabs(m_angle) == 270 || fabs(m_angle) == 360))
  {
    painter.resetMatrix();
    painter.setBrush(QColor(0, 0, 255, 24));
    painter.drawRect(QRect(0, 0, width()-1, height()-1));  
  }

  painter.end();
  update();
}

void te::qt::widgets::TextFrame::hideSelectionPoints()
{
  m_pixmap.fill(Qt::transparent);
  draw();
  update();
}

void te::qt::widgets::TextFrame::getSelectionPoint(const QPoint& p)
{
  m_selected = 0;

  QMatrix matrix = getAuxMatrix();

  if(matrix.mapToPolygon(m_recSel1).containsPoint(p, Qt::OddEvenFill))
    m_selected = 1;
  else if(matrix.mapToPolygon(m_recSel2).containsPoint(p, Qt::OddEvenFill))
    m_selected = 2;
  else if(matrix.mapToPolygon(m_recSel3).containsPoint(p, Qt::OddEvenFill))
    m_selected = 3;
  else if(matrix.mapToPolygon(m_recSel4).containsPoint(p, Qt::OddEvenFill))
    m_selected = 4;
  else if(matrix.mapToPolygon(m_recSel5).containsPoint(p, Qt::OddEvenFill))
    m_selected = 5;
  else if(matrix.mapToPolygon(m_recSel6).containsPoint(p, Qt::OddEvenFill))
    m_selected = 6;
  else if(matrix.mapToPolygon(m_recSel7).containsPoint(p, Qt::OddEvenFill))
    m_selected = 7;
  else if(matrix.mapToPolygon(m_recSel8).containsPoint(p, Qt::OddEvenFill))
    m_selected = 8;
  else if(matrix.mapToPolygon(m_recSel10).containsPoint(p, Qt::OddEvenFill))
    m_selected = 10;
}

QMatrix te::qt::widgets::TextFrame::getAuxMatrix()
{
  QRectF rec = m_layoutEditor->getMatrix().mapRect(m_frameRect);
  double widHeight = height();
  double widWidth = width();
  double rad = m_angle * 3.141593 / 180;
  QMatrix matrix;

  if(m_angle >= 0 && m_angle <= 90)
  {
    double w = rec.height() * sin(rad);
    matrix.translate(w, 0);
  }
  else if(m_angle >= 90 && m_angle <= 180)
  {
    double h = rec.height() * cos(rad);
    matrix.translate(widWidth, -h);
  }
  else if(m_angle >= 180 && m_angle <= 270)
  {
    double w = rec.height() * sin(rad);
    matrix.translate(widWidth + w, widHeight);
  }
  else
  {
    double h = rec.height() * cos(rad);
    matrix.translate(0, widHeight - h);
  }
  matrix.rotate(m_angle);
  return matrix;
}

QPoint te::qt::widgets::TextFrame::getCenterSelected()
{
  QMatrix matrix = getAuxMatrix();
  QPolygon pol;
  QPoint c;

  if(m_selected == 1)
    c = matrix.map(m_recSel1.center());
  else if(m_selected == 2)
    c = matrix.map(m_recSel2.center());
  else if(m_selected == 3)
    c = matrix.map(m_recSel3.center());
  else if(m_selected == 4)
    c = matrix.map(m_recSel4.center());
  else if(m_selected == 5)
    c = matrix.map(m_recSel5.center());
  else if(m_selected == 6)
    c = matrix.map(m_recSel6.center());
  else if(m_selected == 7)
    c = matrix.map(m_recSel7.center());
  else if(m_selected == 8)
    c = matrix.map(m_recSel8.center());
  else if(m_selected == 10)
    c = matrix.map(m_recSel10.center());

  return c;
}

void te::qt::widgets::TextFrame::verifyConstraints()
{
  //QSize size = m_layoutEditor->getPaperSize();
  //QRectF page(0, 0, size.width(), size.height());
  //QMatrix matrix = m_layoutEditor->getMatrixPaperViewToVp();
  //QRectF r = matrix.inverted().mapRect(m_auxFrameRect);
  //                 
  //if(page.contains(r) == false)
  //{
  //  if(m_selected == 10)
  //  {
  //    if(r.top() < page.top())
  //      r = QRectF(r.left(), page.top(), r.width(), r.height());
  //    if(r.bottom() > page.bottom())
  //      r = QRectF(r.left(), page.bottom() - r.height(), r.width(), r.height());
  //    if(r.left() < page.left())
  //      r = QRectF(page.left(), r.top(), r.width(), r.height());
  //    if(r.right() > page.right())
  //      r = QRectF(page.right() - r.width(), r.top(), r.width(), r.height());
  //  }
  //  else
  //  {
  //    if(r.top() < page.top())
  //      r.setTop(page.top());
  //    if(r.bottom() > page.bottom())
  //      r.setBottom(page.bottom());
  //    if(r.left() < page.left())
  //      r.setLeft(page.left());
  //    if(r.right() > page.right())
  //      r.setRight(page.right());
  //  }
  //  m_auxFrameRect = matrix.mapRect(r);
  //}
}

QPoint te::qt::widgets::TextFrame::getGlobalPositionCenter()
{
  QMatrix matrix = m_layoutEditor->getMatrixPaperViewToVp();
  QRect rec = matrix.mapRect(m_frameRect).toRect();
  QPoint center = rec.center();
  return mapToGlobal(center);
  //QWidget* parent = static_cast<QWidget*>(m_layoutEditor);
  //while(parent)
  //{
  //  center += QPoint(parent->x(), parent->y());
  //  parent = static_cast<QWidget*>(parent->parent());
  //}
  //return center;
}

double te::qt::widgets::TextFrame::getAngle(QPoint& p)
{ 
  QPoint ref = rect().center();
  
  QPoint da = p - ref;
  double angle = 90;
  if(da.x() != 0)
    angle = atan((double)da.y() / (double)da.x()) * 180 / 3.141593;
  else if(da.y() < 0)
    angle = 270;

  if(da.x() < 0)
    angle = 180 + angle;
  else if(da.x() > 0 && da.y() < 0)
    angle = 360 + angle;

  return angle;
}

void te::qt::widgets::TextFrame::adjustAngleRange(double& ang)
{
  if(ang < 0)
    ang += 360;
  else if(ang > 360)
    ang -= 360;

  if(fabs(ang) == 360)
    ang = 0;
}

void te::qt::widgets::TextFrame::toolTip(const QPoint& p, const QString& type)
{
  if(m_showToolTip == false)
  {
    QToolTip::hideText();
    return;
  }
 
  QString rotation, stretch, pointSize, letterSpacing, wordSpacing, align, weight, style, capitalization, overline, underline, strikeOut;

  int angle = m_angle;
  if(angle == 360)
    angle = 0;
  else if(angle > 180)
    angle -= 360;
  rotation.setNum(angle);
  rotation = "Rotation = " + rotation + " degrees";

  stretch.setNum(m_stretch); 
  stretch = "Stretch = " + stretch + "%";

  pointSize.setNum(m_pointSize);
  pointSize = "Point Size = " + pointSize;

  letterSpacing.setNum(qRound(m_letterSpacing));
  letterSpacing = "Letter Spacing = " + letterSpacing + "%";

  wordSpacing.setNum(qRound(m_wordSpacing));
  wordSpacing = "Word Spacing = " + wordSpacing;

  if(m_align == Qt::AlignLeft)
    align = "Align = Left";
  else if(m_align ==  Qt::AlignHCenter)
    align = "Align = Center";
  else
    align = "Align = Right";

  if(m_weight == QFont::Light)
    weight = "Weight = Weight Light";
  else if(m_weight == QFont::Normal)
    weight = "Weight = Normal Light";
  else if(m_weight == QFont::DemiBold)
    weight = "Weight = DemiBold Light";
  else if(m_weight == QFont::Bold)
    weight = "Weight = Bold Light";
  else if(m_weight == QFont::Black)
    weight = "Weight = Black Light";

  if(m_style == QFont::StyleNormal)
    style = "Style = Normal";
  else if(m_style == QFont::StyleItalic)
    style = "Style = Italic";
  else if(m_style == QFont::StyleOblique)
    style = "Style = Oblique";

  if(m_capitalization == QFont::MixedCase)
    capitalization = "Case = Mixed Case";
  else if(m_capitalization == QFont::AllUppercase)
    capitalization = "Case = All Uppercase";
  else if(m_capitalization == QFont::AllLowercase)
    capitalization = "Case = All Lowercase";
  else if(m_capitalization == QFont::SmallCaps)
    capitalization = "Case = Small Caps";
  else if(m_capitalization == QFont::Capitalize)
    capitalization = "Case = Capitalize";

  if(m_overline)
    overline = "Overline = ON";
  else
    overline = "Overline = OFF";

  if(m_underline)
    underline = "Underline = ON";
  else
    underline = "Underline = OFF";

  if(m_strikeOut)
    strikeOut = "Strike Out = ON";
  else
    strikeOut = "Strike Out = OFF";

  if(type == "Selection")
  {
    if(m_selected == 1 || m_selected == 3 || m_selected == 5 || m_selected == 7)
      QToolTip::showText(p, rotation, this);
    else if(m_selected == 2 || m_selected == 6)
      QToolTip::showText(p, pointSize, this);
    else if(m_selected == 4 || m_selected == 8)
      QToolTip::showText(p, stretch, this);
    else if(m_selected == 10)
      QToolTip::showText(p, "Drag to Move", this);
    else
      QToolTip::hideText();
  }
  else if(type == "Rotate")
    QToolTip::showText(p, rotation, this);
  else if(type == "Stretch")
    QToolTip::showText(p, stretch, this);
  else if(type == "PointSize")
    QToolTip::showText(p, pointSize, this);
  else if(type == "LetterSpacing")
    QToolTip::showText(p, letterSpacing, this);
  else if(type == "WordSpacing")
    QToolTip::showText(p, wordSpacing, this);
  else if(type == "Align")
    QToolTip::showText(p, align, this);
  else if(type == "Weight")
    QToolTip::showText(p, weight, this);
  else if(type == "Style")
    QToolTip::showText(p, style, this);
  else if(type == "Capitalization")
    QToolTip::showText(p, capitalization, this);
  else if(type == "Overline")
    QToolTip::showText(p, overline, this);
  else if(type == "Underline")
    QToolTip::showText(p, underline, this);
  else if(type == "StrikeOut")
    QToolTip::showText(p, strikeOut, this);
}

void te::qt::widgets::TextFrame::mousePressEvent(QMouseEvent* mouseEvent)
{
  QMatrix matrix = m_layoutEditor->getMatrixPaperViewToVp();

  if(mouseEvent->buttons() == Qt::LeftButton)
  {
    // start edition
    QPoint p = mouseEvent->pos();
    m_pressPoint = p;
    m_auxFrameRect = matrix.mapRect(m_frameRect);
    m_copyAuxFrameRect = m_auxFrameRect;
    m_auxStretch = m_stretch;
    m_auxPointSize = m_pointSize;
    m_auxWidth = m_auxFrameRect.width();
    m_auxHeight = m_auxFrameRect.height();
    m_iniAngle = m_angle;
    m_auxAngle = m_angle;
    m_refAngle = getAngle(p);
    m_undo = false;

    m_layoutEditor->raiseDraftLayoutEditor();
  }
  else if(mouseEvent->buttons() == Qt::RightButton)
  {
    QPoint gp = mouseEvent->globalPos();
    if(m_selected == 10)
    {
      QAction* action = m_menu->exec(gp);
      if(action == m_setFamilyAction)
        changeFontFamily();
      else if(action == m_setColorAction)
        changeTextColor();
      else if(action == m_setTextAction)
        changeText();
      else if(action == m_showTooTipAction)
        showToolTip(true);
      else if(action == m_hideTooTipAction)
        showToolTip(false);
    }
  }
}

void te::qt::widgets::TextFrame::mouseMoveEvent(QMouseEvent* mouseEvent)
{
  QMatrix matrix = m_layoutEditor->getMatrixPaperViewToVp();
  QPoint p = mouseEvent->pos();
  QPoint gp = mouseEvent->globalPos();

  if(mouseEvent->buttons() == Qt::NoButton)
  {
    // only moving the mouse (without press any button)
    // search selection point
    m_dragging = false;
    getSelectionPoint(p);
    setCursor();
    toolTip(gp, "Selection");

    if(m_selected == 0)
    {
      m_layoutEditor->setFrameSelected(0);
      if(!(fabs(m_angle) == 0 || fabs(m_angle) == 90 || fabs(m_angle) == 180))
        lower(); // teste
      return;
    }
    else
    {
      if(m_layoutEditor->getFrameSelected() != this)
        m_layoutEditor->setFrameSelected(this);
    }

  }
  else if(mouseEvent->buttons() == Qt::LeftButton)
  {
    // mouse drag with left buttom
    // edit from selection point
    m_dragging = true;
    QPoint p = mouseEvent->pos();
    QPoint gp = mouseEvent->globalPos();
    QPoint d = m_pressPoint - p;
    int pointSize = m_pointSize;

    if(m_selected != 0)
    {
      if(m_undo == false)
      {
        m_layoutEditor->insertCopy2Undo(this);
        m_undo = true;
      }
      if(m_selected == 1 || m_selected == 3 || m_selected == 5 || m_selected == 7) // rotate using corners
      {
        double beta = getAngle(p);
        double ang = beta - m_refAngle;
        adjustAngleRange(ang);

        m_angle = m_iniAngle + ang;
        adjustAngleRange(m_angle);

        // resolucao angular = 1 grau
        m_angle = qRound(m_angle);
        toolTip(gp, "Rotate");
      }
      else if(m_selected == 2 || m_selected == 6) // change pointSize
      {
        QPoint dif = p - QPoint(width()/2, height()/2);
        double height = 2 * qSqrt(dif.x() * dif.x() + dif.y() * dif.y());

        int ps = qRound(m_auxPointSize * height / m_auxHeight);
        setPointSize(ps);
        QPointF center = m_auxFrameRect.center();
        calculateTextRect(m_auxFrameRect);
        m_auxFrameRect.moveCenter(center);
        toolTip(gp, "PointSize");
      }
      else if(m_selected == 4 || m_selected == 8) // stretch
      {
        QPoint dif = p - QPoint(width()/2, height()/2);
        double width = 2 * qSqrt(dif.x() * dif.x() + dif.y() * dif.y());

        int stretch = qRound(m_auxStretch * width / m_auxWidth);
        setStretch(stretch);
        QPointF center = m_auxFrameRect.center();
        calculateTextRect(m_auxFrameRect);
        m_auxFrameRect.moveCenter(center);
        toolTip(gp, "Stretch");
      }
      else // move frame text
      {
        m_auxFrameRect.moveCenter(m_auxFrameRect.center() - d);
        m_pressPoint = p;
      }

      rubberBand();
    }
  }
}

void te::qt::widgets::TextFrame::mouseReleaseEvent(QMouseEvent*)
{
  // end edition
  if(m_selected == 0)
    return;

  QMatrix matrix = m_layoutEditor->getMatrixPaperViewToVp();
  QPixmap* pixmap = m_layoutEditor->getDraftPixmap();
  pixmap->fill(Qt::transparent);
  verifyConstraints();
  QPointF pp = matrix.inverted().map(m_auxFrameRect.bottomLeft());
  adjustSize(pp);

  showSelectionPoints();
  m_layoutEditor->lowerDraftLayoutEditor();
  m_layoutEditor->update();

  if(m_selected != 10)
  {
    QPoint c = getCenterSelected();
    cursor().setPos(mapToGlobal(c));
    QMouseEvent e1(QEvent::MouseButtonPress, c, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(this, &e1);
  }
}

void te::qt::widgets::TextFrame::enterEvent(QEvent*)
{
  if(QApplication::overrideCursor() || m_dragging)
    return;

  m_layoutEditor->setFrameSelected(this);
  raise();
  m_selected = 0;
}

void te::qt::widgets::TextFrame::leaveEvent(QEvent*)
{
  if(m_dragging)
  {
    if(m_selected != 0)
      raise();
    return;
  }

  m_selected = 0;
  m_layoutEditor->setFrameSelected(0);
  lower();
  setCursor();
}

void te::qt::widgets::TextFrame::setCursor()
{
  QCursor cursor;

  if(m_selected >= 1 && m_selected <= 8)
    cursor.setShape(Qt::PointingHandCursor);
  else if(m_selected == 10)
    cursor.setShape(Qt::SizeAllCursor);

  if(QApplication::overrideCursor())
  {
    if(QApplication::overrideCursor()->shape() != cursor.shape())
    {
      while(QApplication::overrideCursor())
        QApplication::restoreOverrideCursor();
      if(cursor.shape() != Qt::ArrowCursor)
        QApplication::setOverrideCursor(cursor);
    }
  }
  else if(cursor.shape() != Qt::ArrowCursor)
    QApplication::setOverrideCursor(cursor);
}

void te::qt::widgets::TextFrame::changeFontFamily()
{
  bool ok;
  QFont auxFont(*m_font);
  auxFont.setPointSize(m_pointSize); // use point size original
  // nota: point size = 72 é o maximo da lista na interface do QT,
  // mas, o LineEdit é do tipo write, então voce pode mudar.

  QFont font = QFontDialog::getFont(&ok, auxFont, this);
  if (ok) 
  {
    setFamily(font.family());
    setPointSize(font.pointSize());
    setStyle(font.style());
    setWeight((QFont::Weight)font.weight());
    setStrikeOut(font.strikeOut());
    setUnderline(font.underline());
    adjust();
  }
}

void te::qt::widgets::TextFrame::changeTextColor()
{
  QColor color = QColorDialog::getColor(m_cor, this, "Set Text Color", QColorDialog::ShowAlphaChannel);
  if(color.isValid())
  {
    m_cor = color;
    adjust();
  }
}

void te::qt::widgets::TextFrame::changeText()
{
  bool ok;
  QString text = QInputDialog::getText(this, "Change Text", "New Text", QLineEdit::Normal, m_text, &ok);
  if(text.isEmpty() == false && ok)
  {
    //char c;
    //int i = 0;
    //while(i < text.length())
    //{
    //  c = text[i].toAscii();
    //  i++;
    //}
    m_text = text;
    adjust();
  }
}
