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
  \file terralib/maptools/LayoutEditor.cpp

  \brief The map editor....
*/

#include "LayoutEditor.h"
#include "DraftLayoutEditor.h"
#include "DataFrame.h"
#include "TextFrame.h"
#include "EditorInfo.h"
#include "GeographicGridFrame.h"
#include "UTMGridFrame.h"
#include "GraphicScaleFrame.h"
#include "../canvas/MultiThreadMapDisplay.h"

// Qt

#include <QPainter>
#include <QColor>
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>
#include <QMimeData>

te::qt::widgets::LayoutEditor::LayoutEditor(QWidget* parent, Qt::WindowFlags f) :
  QWidget(parent, f),
  m_idCount(1),
  m_paperSize(QSize(210, 297)),
  m_horizontalRulerWidth(30),  // definir tamanho da ruler horizontal em pixels
  m_verticalRulerWidth(38),  // definir tamanho da ruler vertical em pixels
  m_layoutObjectSelected(0),
  m_dataPan(false),
  m_showRulerGrid(true),
  m_rulerGridLines(1),
  m_undoBufferSize(100),
  m_editorState(0),
  m_resize(false),
  m_move(false)
{
  setMouseTracking(true);
  setWindowTitle("Layout Editor");

  m_auxWidget = new QWidget(this, Qt::Widget);
  m_auxWidget->setWindowOpacity(0);

  m_draftLayoutEditor = new DraftLayoutEditor(m_auxWidget, Qt::Widget);
  m_draftLayoutEditor->setWindowOpacity(0);

  m_putUndo = false;
  resize(600, 600); 
  m_putUndo = true;

  // inicializar com papel A4
  m_paperSize = QSize(210, 297);
 
  // insert empty data frame 
  int a = m_paperSize.width();
  if(a > m_paperSize.height())
    a = m_paperSize.height();

  int frameX = qRound((double)a/8);
  int frameY = 100;
  int frameWidth = qRound((double)a * .75);
  int frameHeight = frameWidth;
  QRectF frameRect(frameX, frameY, frameWidth, frameHeight);

  te::qt::widgets::DataFrame* df = new te::qt::widgets::DataFrame(frameRect, this, Qt::Widget);
  df->setData(0);
  insert(df);

  // insert text frames
  int pointSize = 24;
  te::qt::widgets::TextFrame* tf = new te::qt::widgets::TextFrame(QPointF(10, 10), "INPE\nTerraView\nVersao 5.0", pointSize, this, Qt::Widget);
  insert(tf);

  tf = new te::qt::widgets::TextFrame(QPointF(10, 270), "divisao de processamento de imagens", pointSize, this, Qt::Widget);
  insert(tf);

  m_zoomInPixmap = 0;
  m_zoomOutPixmap = 0;
  m_panPixmap = 0;
  m_zmouseMode = 0;
}

te::qt::widgets::LayoutEditor::LayoutEditor(const QSize& paperSize, QWidget* parent, Qt::WindowFlags f) :
  QWidget(parent, f),
  m_idCount(1),
  m_paperSize(paperSize),
  m_horizontalRulerWidth(30),  // definir tamanho da ruler horizontal em pixels
  m_verticalRulerWidth(38),  // definir tamanho da ruler vertical em pixels
  m_layoutObjectSelected(0),
  m_dataPan(false),
  m_showRulerGrid(true),
  m_rulerGridLines(1),
  m_undoBufferSize(100),
  m_editorState(0),
  m_resize(false),
  m_move(false)
{
  setMouseTracking(true);
  setWindowTitle("Layout Editor");

  m_auxWidget = new QWidget(this, Qt::Widget);
  m_auxWidget->setWindowOpacity(0);

  m_draftLayoutEditor = new DraftLayoutEditor(m_auxWidget, Qt::Widget);
  m_draftLayoutEditor->setWindowOpacity(0);

  m_putUndo = false;
  resize(600, 600); 
  m_putUndo = true;

  m_zoomInPixmap = 0;
  m_zoomOutPixmap = 0;
  m_panPixmap = 0;
  m_zmouseMode = 0;
}

te::qt::widgets::LayoutEditor::~LayoutEditor()
{
  hide();
  delete m_draftLayoutEditor;
  delete m_editorState;

  std::vector<te::qt::widgets::LayoutObject*>::iterator it;
  for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
    delete *it;
  m_layoutObjects.clear();

  for(it = m_undoLayoutObjects.begin(); it != m_undoLayoutObjects.end(); ++it)
    delete *it;
  m_undoLayoutObjects.clear();

  delete m_zoomInPixmap;
  delete m_zoomOutPixmap;
  delete m_panPixmap;
}

void te::qt::widgets::LayoutEditor::resetPaperView()
{
  int delta = qRound((double)m_paperSize.width() / 100);
  m_paperViewRect.setWidth(m_paperSize.width() + delta + delta);
  m_paperViewRect.setHeight(m_paperSize.height() + delta + delta);
  m_paperViewRect.moveCenter(QPointF(((double)m_paperSize.width()/2.), (double)m_paperSize.height()/2.));

  m_paperViewReseted = true;
}

void te::qt::widgets::LayoutEditor::copyState()
{
  if(m_editorState)
  {
    m_editorState->m_paperViewRect = m_paperViewRect;
    m_editorState->m_paperSize = m_paperSize;
    m_editorState->m_rulerGridLines = m_rulerGridLines;
    m_editorState->m_showRulerGrid = m_showRulerGrid;
    m_editorState->m_rect = m_rect;
    m_editorState->m_resize = m_resize;
    m_editorState->m_move = m_move;
  }
}

void te::qt::widgets::LayoutEditor::createWorkingArea(bool putUndo)
{
  if(m_paperViewRect.isValid() == false)
    resetPaperView();

  // criar o totalPixmap que representa a area do paperView + as rulers
  int tw = size().width();
  int w = tw - 2 * m_verticalRulerWidth;
  int th = size().height();
  int h = th - 2 * m_horizontalRulerWidth;

  if(m_paperViewReseted)
  {
    double relw = (double)w / (double)m_paperViewRect.width();
    double relh = (double)h / (double)m_paperViewRect.height();

    int nw, nh, ntw, nth;
    // ajustar m_paperViewRect sobre o display
    if(relw < relh)
    {
      nh = qRound((double)(w) * m_paperViewRect.height() / m_paperViewRect.width());
      nth = h + 2 * m_horizontalRulerWidth;

      QPointF c = m_paperViewRect.center();
      double v = (double)h / (double)nh;
      double d = (double)m_paperViewRect.height() * v;
      m_paperViewRect = QRect(0., 0., m_paperViewRect.width(), d);
      m_paperViewRect.moveCenter(c);
    }
    else
    {
      nw = qRound((double)(h) * m_paperViewRect.width() / m_paperViewRect.height());
      ntw = w + 2 * m_verticalRulerWidth;

      QPointF c = m_paperViewRect.center();
      double v = (double)w / (double)nw;
      double d = (double)m_paperViewRect.width() * v;
      m_paperViewRect = QRect(0., 0., d, m_paperViewRect.height());
      m_paperViewRect.moveCenter(c);
    }
  }

  m_totalPixmap = QPixmap(tw, th);
  m_totalPixmap.fill(Qt::gray);

  // criar o pixmap que representa o paperview (regiao de visualizacao)
  m_viewPixmap = QPixmap(w, h);

  if(putUndo)
  {
    // if resize operation in progress
    // calculate new paperView
    if(m_difTopLeft != QPointF(0., 0.)) // top left resized
    {
      QRectF r(m_difTopLeft, QSize(w, h));
      m_paperViewRect = m_matrixPaperViewToVp.inverted().mapRect(r);
    }
    else if(m_difBottomRight != QPointF(0., 0.)) // bottom right resized
    {
      QRectF r(QPoint(0., 0.), QSize(w, h));
      m_paperViewRect = m_matrixPaperViewToVp.inverted().mapRect(r);
    }
  }

  // agora podemos calcular a escala entre o paperView e o viewport
  double scalePaperViewToVp = (double)m_viewPixmap.width() / m_paperViewRect.width();

  // criar a matrix de transformacao paperView to Viewport
  QMatrix matrixOld = m_matrixPaperViewToVp;
  m_matrixPaperViewToVp.reset();
  m_matrixPaperViewToVp.scale(scalePaperViewToVp, -scalePaperViewToVp);
  m_matrixPaperViewToVp.translate(-m_paperViewRect.left(), -m_paperViewRect.bottom());

  m_matrix.reset();
  m_matrix.scale(scalePaperViewToVp, -scalePaperViewToVp);
  double tx = m_paperViewRect.left() + (double)m_verticalRulerWidth / scalePaperViewToVp;
  double ty = m_paperViewRect.bottom() - (double)m_horizontalRulerWidth / scalePaperViewToVp;
  m_matrix.translate(-tx, -ty);

  // insert rulers to totalPixmap
  insertRulers();

  QPainter painter(&m_viewPixmap);
  painter.setMatrix(m_matrixPaperViewToVp);

  // desenhar retangulo do papel
  painter.setBrush(Qt::NoBrush);
  QPen pen(QColor(0, 0, 255, 255));
  pen.setStyle(Qt::DashLine);
  painter.setPen(pen);
  painter.drawRect(QRect(0, 0, m_paperSize.width(), m_paperSize.height()));
  painter.end();

  //// copiar paperView to totalPixmap 
  //// NOTA: totalPixmap eh a area de visualizacao + as reguas (eventualmente pode ser a folha toda... depende do zoom)
  //painter.begin(&m_totalPixmap);
  //painter.drawPixmap(m_verticalRulerWidth, m_horizontalRulerWidth, m_viewPixmap);

  //painter.end();

  m_paperViewReseted = false;

  std::vector<te::qt::widgets::LayoutObject*>::iterator it;
  for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
    (*it)->adjust();

  update();

  if(putUndo)
    appendToUndo();
}

void te::qt::widgets::LayoutEditor::appendToUndo() // usado para quando muda o frame work
{
  if(m_undoBufferSize)
  {
    if(m_editorState == 0)
    {
      m_editorState = new te::qt::widgets::EditorInfo(this);
      copyState();
    }
    te::qt::widgets::EditorInfo* ei = new te::qt::widgets::EditorInfo(*m_editorState);

    // verifique se ultrapassa o numero de undos. Se sim, retire os mais antigos undo.
    while(m_undoLayoutObjects.size() >= (unsigned int)m_undoBufferSize)
    {
      std::vector<te::qt::widgets::LayoutObject*>::iterator it = m_undoLayoutObjects.begin();
      te::qt::widgets::LayoutObject* fa = *it;
      m_undoLayoutObjects.erase(it);
      delete (fa);
    }

    // se a operacao � resize, elimine os desnecessarios
    if(m_resize || m_move) // � resize ou move operation
    {
      te::qt::widgets::LayoutObject *fu = 0, *fap = 0;
      std::vector<te::qt::widgets::LayoutObject*>::reverse_iterator it = m_undoLayoutObjects.rbegin();
      if(it != m_undoLayoutObjects.rend())
      {
        fu = *it;
        ++it;
        if(it != m_undoLayoutObjects.rend())
        {
          fap = *it;

          if(fu && fap)
          {
            if(fu->getId() == 0 && fap->getId() == 0) // � viewer edition
            {
              te::qt::widgets::EditorInfo* eiu = (te::qt::widgets::EditorInfo*)fu;
              te::qt::widgets::EditorInfo* eiap = (te::qt::widgets::EditorInfo*)fap;

              if((eiu->m_resize || eiu->m_move) && (eiap->m_resize || eiap->m_move))
              {
                m_undoLayoutObjects.pop_back();
                delete fu;
                if(m_undoLayoutObjects.size() > 1)
                {
                  m_undoLayoutObjects.pop_back();
                  delete fap;
                }
              }
            }
          }
        }
      }
    }

    m_undoLayoutObjects.push_back(ei);
    m_resize = false;
    m_move = false;
    copyState();
  }
}

int te::qt::widgets::LayoutEditor::getHorizontalRulerWidth()
{
  return m_horizontalRulerWidth;
}

int te::qt::widgets::LayoutEditor::getVerticalRulerWidth()
{
  return m_verticalRulerWidth;
}

void te::qt::widgets::LayoutEditor::setUndoBufferSize(const int& size)
{
  int s = m_undoBufferSize - size;

  if(s > 0)
  {
    std::vector<te::qt::widgets::LayoutObject*>::iterator it;
    for(int i = 0; i != s; ++i)
    {
      it = m_undoLayoutObjects.begin();
      te::qt::widgets::LayoutObject* f = *it;
      m_undoLayoutObjects.erase(it);
      delete (f);
    }
  }
}

int te::qt::widgets::LayoutEditor::getUndoBufferSize()
{
  return m_undoBufferSize;
}

void te::qt::widgets::LayoutEditor::insertRulers()
{
  // calcular horizontal ruler width
  int horizontalRulerWidth = qRound((double)(m_totalPixmap.height() - m_viewPixmap.height()) / 2);

  // calcular vertical ruler width
  int verticalRulerWidth = qRound((double)(m_totalPixmap.width() - m_viewPixmap.width()) / 2);

  // valores minimos e maximos das reguas
  double xmin = m_paperViewRect.left();
  double xmax = m_paperViewRect.right();
  double ymin = m_paperViewRect.top();
  double ymax = m_paperViewRect.bottom();

  // definir a resolucao da regua de acordo com a escala
  double scale = m_matrixPaperViewToVp.m11();
  if(scale > 4)
  {
    m_rulerSmall = 1;
    m_rulerMedium = 5;
    m_rulerLarge = 10;
  }
  else if(scale > 2)
  {
    m_rulerSmall = 2;
    m_rulerMedium = 10;
    m_rulerLarge = 20;
  }
  else if(scale > 1)
  {
    m_rulerSmall = 5;
    m_rulerMedium = 25;
    m_rulerLarge = 50;
  }
  else if(scale > .5)
  {
    m_rulerSmall = 10;
    m_rulerMedium = 50;
    m_rulerLarge = 100;
  }
  else if(scale > .25)
  {
    m_rulerSmall = 20;
    m_rulerMedium = 100;
    m_rulerLarge = 200;
  }
  else if(scale > .125)
  {
    m_rulerSmall = 50;
    m_rulerMedium = 250;
    m_rulerLarge = 500;
  }
  else
  {
    m_rulerSmall = 100;
    m_rulerMedium = 500;
    m_rulerLarge = 1000;
  }

  // create horizontal rulers
  QPixmap rulerBottom(m_viewPixmap.width(), horizontalRulerWidth);
  rulerBottom.fill(Qt::gray);
  QPixmap rulerTop(rulerBottom);

  QString num;
  QPainter painter1(&rulerBottom);
  QPen pen(Qt::black);
  painter1.setPen(pen);
  QFont font("");
  int fontSize = 11; // medida da fonte em pixels
  font.setPixelSize(fontSize);
  painter1.setFont(font);

  QPainter painter2(&rulerTop);
  painter2.setPen(pen);
  painter2.setFont(font);

  int ini = 1;
  int peq = 5;
  int medio = 8;
  int grande = 10;
  QPoint pb1(0, ini);
  QPoint pb2(0, peq);
  QPoint pb3(0, medio);
  QPoint pb4(0, grande);
  QPoint pb5(0, fontSize + grande);
  QPoint pt1(0, horizontalRulerWidth - ini);
  QPoint pt2(0, horizontalRulerWidth - peq);
  QPoint pt3(0, horizontalRulerWidth - medio);
  QPoint pt4(0, horizontalRulerWidth - grande);
  QPoint pt5(0, horizontalRulerWidth - fontSize - grande);
  int a = xmin;
  while(a <= (xmax+1))
  {
    int xp = m_matrixPaperViewToVp.map(QPoint(a, m_paperViewRect.top())).x();
    pb1.setX(xp);
    pb2.setX(xp);
    pb3.setX(xp);
    pb4.setX(xp);
    pb5.setX(xp);
    pt1.setX(xp);
    pt2.setX(xp);
    pt3.setX(xp);
    pt4.setX(xp);
    pt5.setX(xp);
    if((a % m_rulerLarge) == 0)
    {
      painter1.drawLine(pb1, pb4);
      num.setNum(a);
      QRectF br = painter1.boundingRect(QRectF(0, 0, rulerBottom.width(), rulerBottom.height()), Qt::AlignHCenter, num);
      br.moveCenter(pb5);
      painter1.drawText(br, num);

      painter2.drawLine(pt1, pt4);
      br.moveCenter(pt5);
      painter2.drawText(br, num);
    }
    else if((a % m_rulerMedium) == 0)
    {
      painter1.drawLine(pb1, pb3);
      painter2.drawLine(pt1, pt3);
    }
    else if((a % m_rulerSmall) == 0)
    {
      painter1.drawLine(pb1, pb2);
      painter2.drawLine(pt1, pt2);
    }
    a++;
  }
  painter1.end();
  painter2.end();


  // create vertical rules
  QPixmap rulerRight(verticalRulerWidth, m_viewPixmap.height());
  rulerRight.fill(Qt::gray);
  QPixmap rulerLeft(rulerRight);

  painter1.begin(&rulerRight);
  painter1.setPen(pen);
  painter1.setFont(font);

  painter2.begin(&rulerLeft);
  painter2.setPen(pen);
  painter2.setFont(font);

  QPoint pr1(ini, 0);
  QPoint pr2(peq, 0);
  QPoint pr3(medio, 0);
  QPoint pr4(grande, 0);
  QPoint pr5(fontSize + grande, 0);
  QPoint pl1(verticalRulerWidth - ini, 0);
  QPoint pl2(verticalRulerWidth - peq, 0);
  QPoint pl3(verticalRulerWidth - medio, 0);
  QPoint pl4(verticalRulerWidth - grande, 0);
  QPoint pl5(verticalRulerWidth - fontSize - grande, 0);
  a = ymin;
  while(a <= (ymax+1))
  {
    int yp = m_matrixPaperViewToVp.map(QPoint(0, a)).y();
    pr1.setY(yp);
    pr2.setY(yp);
    pr3.setY(yp);
    pr4.setY(yp);
    pr5.setY(yp);
    pl1.setY(yp);
    pl2.setY(yp);
    pl3.setY(yp);
    pl4.setY(yp);
    pl5.setY(yp);
    if((a % m_rulerLarge) == 0)
    {
      painter1.drawLine(pr1, pr4);
      num.setNum(a);
      QRectF br = painter1.boundingRect(QRectF(0, 0, rulerRight.width(), rulerRight.height()), Qt::AlignHCenter, num);
      br.moveCenter(pr5);
      painter1.drawText(br, num);

      painter2.drawLine(pl1, pl4);
      br.moveCenter(pl5);
      painter2.drawText(br, num);
    }
    else if((a % m_rulerMedium) == 0)
    {
      painter1.drawLine(pr1, pr3);
      painter2.drawLine(pl1, pl3);
    }
    else if((a % m_rulerSmall) == 0)
    {
      painter1.drawLine(pr1, pr2);
      painter2.drawLine(pl1, pl2);
    }
    a++;
  }
  painter1.end();
  painter2.end();
  painter1.begin(&m_totalPixmap);

  painter1.drawPixmap(verticalRulerWidth, m_totalPixmap.height() - horizontalRulerWidth, rulerBottom);
  painter1.drawPixmap(verticalRulerWidth, 0, rulerTop);
  painter1.drawPixmap(m_totalPixmap.width() - verticalRulerWidth, horizontalRulerWidth, rulerRight);
  painter1.drawPixmap(0, horizontalRulerWidth, rulerLeft);
  painter1.end();

  createRulerGrid();
}

void te::qt::widgets::LayoutEditor::showRulerGrid()
{
  if(m_showRulerGrid == true)
    return;
  m_showRulerGrid = true;
  draw();
}

void te::qt::widgets::LayoutEditor::hideRulerGrid()
{
  if(m_showRulerGrid == false)
    return;
  m_showRulerGrid = false;
  draw();
}

void te::qt::widgets::LayoutEditor::createRulerGrid()
{
  m_rulerGridPixmap = QPixmap(m_viewPixmap.width(), m_viewPixmap.height());
  m_rulerGridPixmap.fill(Qt::white);

  // valores minimos e maximos das reguas
  int xmin = qRound(m_paperViewRect.left()) - m_rulerLarge;
  int xmax = qRound(m_paperViewRect.right()) + m_rulerLarge;
  int ymin = qRound(m_paperViewRect.top()) - m_rulerLarge;
  int ymax = qRound(m_paperViewRect.bottom()) + m_rulerLarge;

  while(xmin <= xmax)
  {
    if((xmin % m_rulerSmall) == 0)
      break;
    xmin++;
  }

  while(ymin <= ymax)
  {
    if((ymin % m_rulerSmall) == 0)
      break;
    ymin++;
  }

  QPainter painter(&m_rulerGridPixmap);
  painter.setMatrix(m_matrixPaperViewToVp);
  QPen pen(Qt::blue);

  int line;
  //QPoint p1(xmin, m_paperViewRect.top());
  QPoint p1(xmin, ymin);
  QPoint p2(xmin, ymax);
  while(p1.x() <= xmax)
  {
    painter.setPen(QPen(Qt::blue));
    line = 1;
    if((p1.x() % m_rulerLarge) == 0)
    {
      line = 3;
      painter.setPen(QPen(Qt::red));
    }
    else if((p1.x() % m_rulerMedium) == 0)
    {
      line = 2;
      painter.setPen(QPen(Qt::green));
    }

    if(line >= m_rulerGridLines)
      painter.drawLine(p1, p2);

    p1.setX(p1.x() + m_rulerSmall);
    p2.setX(p2.x() + m_rulerSmall);
  }

  //p1 = QPoint(m_paperViewRect.left(), ymin);
  p1 = QPoint(xmin, ymin);
  p2 = QPoint(xmax, ymin);
  while(p1.y() <= ymax)
  {
    painter.setPen(QPen(Qt::blue));
    line = 1;
    if((p1.y() % m_rulerLarge) == 0)
    {
      line = 3;
      painter.setPen(QPen(Qt::red));
    }
    else if((p1.y() % m_rulerMedium) == 0)
    {
      line = 2;
      painter.setPen(QPen(Qt::green));
    }

    if(line >= m_rulerGridLines)
      painter.drawLine(p1, p2);

    p1.setY(p1.y() + m_rulerSmall);
    p2.setY(p2.y() + m_rulerSmall);
  }
}

void te::qt::widgets::LayoutEditor::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  int x = 0;
  int y = 0;

  if(size().width() > m_totalPixmap.width())
    x = qRound((double)(size().width() - m_totalPixmap.width()) / 2);
  if(size().height() > m_totalPixmap.height())
    y = qRound((double)(size().height() - m_totalPixmap.height()) / 2);

  painter.drawPixmap(x, y, m_totalPixmap);
  m_draftLayoutEditor->update(); 
}

void te::qt::widgets::LayoutEditor::resizeEvent(QResizeEvent* event)
{
  m_resize = true;
  copyState();
  int w = width() - m_verticalRulerWidth - m_verticalRulerWidth;
  int h = height() - m_horizontalRulerWidth - m_horizontalRulerWidth;
  m_auxWidget->resize(w, h);
  m_auxWidget->move(m_verticalRulerWidth, m_horizontalRulerWidth);
  m_auxWidget->show();

  m_draftLayoutEditor->resize(w, h);
  m_draftLayoutEditor->getDraftPixmap()->fill(Qt::transparent);

  m_difTopLeft = QPointF(0., 0.);
  m_difBottomRight = QPointF(0., 0.);

  QRect rec(pos().x(), pos().y(), size().width(), size().height());
  if(m_rect.isValid())
  {
    if(rec.topLeft() != m_rect.topLeft())
      m_difTopLeft = rec.topLeft() - m_rect.topLeft();
    else
      m_difBottomRight = rec.bottomRight() - m_rect.bottomRight();
  }
  m_rect = rec;

  createWorkingArea(m_putUndo);
  draw();

  std::vector<te::qt::widgets::LayoutObject*>::iterator it;
  for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
    (*it)->adjust();
}

// O evento de move eh chamado apos mover, mas,
// tambem � chamado antes de resize top/left
void te::qt::widgets::LayoutEditor::moveEvent(QMoveEvent* event)
{ 
  // usado para manter o papel na mesma posicao durante o resize top/left
  if(m_putUndo)
  {
    // informacoes de undo para poder voltar aa posicao anterior
    //if(m_rect.isValid())
    //  appendToUndo();
    if(m_rect.isValid())
    {
      m_move = true;
      copyState();
      m_difTopLeft = pos() - m_rect.topLeft();

      m_rect.moveTopLeft(pos());
      createWorkingArea(m_putUndo);
      draw();

      std::vector<te::qt::widgets::LayoutObject*>::iterator it;
      for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
        (*it)->adjust();
    }
  }
}

void te::qt::widgets::LayoutEditor::setPaperSize(const QSize& size)
{
  m_paperSize = size;
  createWorkingArea();
  draw();
}

QSize te::qt::widgets::LayoutEditor::getPaperSize()
{
  return m_paperSize;
}

QRectF te::qt::widgets::LayoutEditor::getPaperViewRect()
{
  return m_paperViewRect;
}

te::qt::widgets::LayoutObject* te::qt::widgets::LayoutEditor::find(unsigned int id)
{
  std::vector<te::qt::widgets::LayoutObject*>::iterator it;
  for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
  {
    if((*it)->getId() == id)
      return (*it);
  }
  return 0;
}

// n�o use este metodo para inserir frames durante a edi��o
void te::qt::widgets::LayoutEditor::insert(te::qt::widgets::LayoutObject* f)
{
  if(f->getId() == 0)
    f->setId(m_idCount++);
  m_layoutObjects.push_back(f);
}

void te::qt::widgets::LayoutEditor::remove(te::qt::widgets::LayoutObject* f)
{
  std::vector<te::qt::widgets::LayoutObject*>::iterator it;
  for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
  {
    if((*it) == f)
    {
      m_layoutObjects.erase(it);
      break;
    }
  }
}

void te::qt::widgets::LayoutEditor::insertCopy2Undo(te::qt::widgets::LayoutObject* f)
{
  te::qt::widgets::LayoutObject* cf = 0;
  if(f->windowTitle() == "TextFrame")
    cf = new te::qt::widgets::TextFrame(*(te::qt::widgets::TextFrame*)f);
  else if(f->windowTitle() == "DataFrame")
    cf = new te::qt::widgets::DataFrame(*(te::qt::widgets::DataFrame*)f);
  else
    return;

  while(m_undoLayoutObjects.size() >= (unsigned int)m_undoBufferSize)
  {
    std::vector<te::qt::widgets::LayoutObject*>::iterator it = m_undoLayoutObjects.begin();
    te::qt::widgets::LayoutObject* f = *it;
    m_undoLayoutObjects.erase(it);
    delete (f);
  }

  if(f->getId() != 0) // � frame edition
  {
    te::qt::widgets::Frame* frame = (te::qt::widgets::Frame*)f;
    frame->setNew(false);
  }

  m_undoLayoutObjects.push_back(cf);
  cf->hide();

  setFrameSelected(f);
}

void te::qt::widgets::LayoutEditor::removeUndo(te::qt::widgets::LayoutObject* f)
{
  std::vector<te::qt::widgets::LayoutObject*>::iterator it;
  for(it = m_undoLayoutObjects.begin(); it != m_undoLayoutObjects.end(); ++it)
  {
    if((*it) == f)
    {
      m_undoLayoutObjects.erase(it);
      break;
    }
  }
}

void te::qt::widgets::LayoutEditor::pan(const QPointF& p)
{
  QPointF pp = m_matrixPaperViewToVp.inverted().map(p);
  QPointF ps = m_matrixPaperViewToVp.inverted().map(m_startPan);
  m_startPan = p;

  QPointF dif = pp - ps;
  QPointF c = m_paperViewRect.center();
  c -= dif;
  m_paperViewRect.moveCenter(c);

  createWorkingArea();
  draw();
}

void te::qt::widgets::LayoutEditor::draw()
{
  m_viewPixmap.fill(Qt::white);

  QPainter painter(&m_viewPixmap);
  if(m_showRulerGrid)
    painter.drawPixmap(0, 0, m_rulerGridPixmap);

  std::vector<te::qt::widgets::LayoutObject*>::iterator it;
  for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
  {
    if((*it)->windowTitle() == "DataFrame")
    {
      te::qt::widgets::DataFrame* df = (te::qt::widgets::DataFrame*)*it;
      if(df->getFrameRect().intersects(m_paperViewRect))
        df->draw();
    }
  }

  QRect pr = m_matrixPaperViewToVp.mapRect(QRect(0, 0, m_paperSize.width(), m_paperSize.height()));

  // desenhar retangulo do papel
  painter.setBrush(Qt::NoBrush);
  QPen pen(QColor(0, 0, 255, 255));
  pen.setStyle(Qt::DashLine);
  pen.setWidth(3);
  painter.setPen(pen);
  painter.drawRect(pr);
  painter.end();

  // copiar papel to totalPixmap
  painter.begin(&m_totalPixmap);
  painter.drawPixmap(m_verticalRulerWidth, m_horizontalRulerWidth, m_viewPixmap);

  painter.end();

  update();
}

void te::qt::widgets::LayoutEditor::drawButtonClicked()
{
  std::vector<te::qt::widgets::LayoutObject*>::iterator it;
  for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
  {
    if((*it)->windowTitle() == "DataFrame")
    {
      te::qt::widgets::DataFrame* df = (te::qt::widgets::DataFrame*)*it;
      df->drawButtonClicked();
    }
  }
}

void te::qt::widgets::LayoutEditor::mousePressEvent(QMouseEvent* e)
{
  if(e->button() == Qt::MidButton)
  {
    int i = m_zmouseMode + 1;
    if(i > 3)
      i = 0;  
    setMouseMode(i);
    return;
  }

  m_difTopLeft = QPointF(0., 0.);
  m_difBottomRight = QPointF(0., 0.);
  m_zdataFrame = 0;

  m_zpressPoint = e->pos();
  if(m_zmouseMode == 3)
  {
    //unsetCursor();
    //QCursor cursor(Qt::OpenHandCursor);
    //setCursor(cursor);
    m_zpanEnd = false;
    m_zpixmap = QPixmap(m_totalPixmap);

    // desenha os layout objects sobre m_zpixmap
    QPoint p(m_verticalRulerWidth, m_horizontalRulerWidth);
    QPainter painter(&m_zpixmap);

    std::vector<te::qt::widgets::LayoutObject*>::iterator it;
    for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
    {
      QRectF r = (*it)->getFrameRect();
      if(r.isValid())
      {
        QPixmap* pixmap = (*it)->getPixmap();
        if(pixmap && pixmap->isNull() == false)
        {
          r = m_matrixPaperViewToVp.mapRect(r);
          r.moveCenter(r.center() + p);
          painter.drawPixmap(r, *pixmap, pixmap->rect());
        }
        if((*it)->windowTitle() == "DataFrame")
        {
          te::qt::widgets::DataFrame* df = static_cast<te::qt::widgets::DataFrame*>(*it);
          if(df->getUTMGridFrame())
          {
            QRect r = df->getUTMGridFrame()->getFrameRect().toRect();
            QPixmap* pixmap = df->getUTMGridFrame()->getPixmap();
            if(pixmap && pixmap->isNull() == false)
            {
              r = m_matrixPaperViewToVp.mapRect(r);
              r.moveCenter(r.center() + p);
              painter.drawPixmap(r, *pixmap, pixmap->rect());
            }
          }
          if(df->getGeoGridFrame())
          {
            QRect r = df->getGeoGridFrame()->getFrameRect().toRect();
            QPixmap* pixmap = df->getGeoGridFrame()->getPixmap();
            if(pixmap && pixmap->isNull() == false)
            {
              r = m_matrixPaperViewToVp.mapRect(r);
              r.moveCenter(r.center() + p);
              painter.drawPixmap(r, *pixmap, pixmap->rect());
            }
          }
          if(df->getGraphicScaleFrame())
          {
            QRect r = df->getGraphicScaleFrame()->getFrameRect().toRect();
            QPixmap* pixmap = df->getGraphicScaleFrame()->getPixmap();
            if(pixmap && pixmap->isNull() == false)
            {
              r = m_matrixPaperViewToVp.mapRect(r);
              r.moveCenter(r.center() + p);
              painter.drawPixmap(r, *pixmap, pixmap->rect());
            }
          }
        }
      }
    }
  }

  if(e->modifiers() == Qt::ControlModifier)
  {
    //get Data Frame
    m_zdataFrame = 0;
    std::vector<te::qt::widgets::LayoutObject*>::iterator it;
    for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
    {
      if((*it)->windowTitle() == "DataFrame")
      {
        te::qt::widgets::DataFrame* df = (te::qt::widgets::DataFrame*)*it;
        QRectF r = df->getFrameRect();
        QPointF p = QPoint(e->pos().x(), e->pos().y());
        p =  m_matrixPaperViewToVp.inverted().map(p);
        if(r.contains(p))
        {
          if(df->getData())
          {
            m_zdataFrame = df;
            m_zpixmap = QPixmap(*df->getPixmap());
          }
          else
            m_zdataFrame = 0;
          break;
        }
      }
    }
  }
}

void te::qt::widgets::LayoutEditor::mouseMoveEvent(QMouseEvent* e)
{
  if(e->modifiers() == Qt::ControlModifier && m_zdataFrame == 0)
    return;

  m_zpoint = e->pos();

  if(e->buttons() == Qt::LeftButton) //atual estado do botao
  {
    if(m_zmouseMode == 3)
    {
      if(cursor().shape() != Qt::ClosedHandCursor)
      {
        unsetCursor();
        QCursor cursor(Qt::ClosedHandCursor);
        setCursor(cursor);
      }

      if(e->modifiers() == Qt::NoModifier) // pan sobre paper view
      {
        hideAllObjects();
        m_zrect = QRect();
        QPoint pfrom(m_zpressPoint.x(), m_zpressPoint.y());
        QPoint pto(e->pos().x(), e->pos().y());
        QPoint dif = pto - pfrom;

        m_totalPixmap.fill(Qt::white);
        QPainter painter(&m_totalPixmap);
        QRect r = m_totalPixmap.rect();
        QPoint c(r.center() + dif);
        r.moveCenter(c);
        painter.drawPixmap(r, m_zpixmap, m_zpixmap.rect());
        painter.end();
        update();
      }
      else if(e->modifiers() == Qt::ControlModifier && m_zdataFrame) // pan sobre data frame
      {
        m_zrect = QRect();
        QPoint pfrom(m_zpressPoint.x(), m_zpressPoint.y());
        QPoint pto(e->pos().x(), e->pos().y());
        QPoint dif = pto - pfrom;

        QPixmap* pix = m_zdataFrame->getMapDisplay()->getDisplayPixmap();
        pix->fill(Qt::white);
        QPainter painter(pix);
        QRect r = m_zpixmap.rect();
        QPoint c(r.center() + dif);
        r.moveCenter(c);
        painter.drawPixmap(r, m_zpixmap, m_zpixmap.rect());
        painter.end();
        m_zdataFrame->getMapDisplay()->update();
      }
    }
    else if(m_zmouseMode == 2) // zoom out
    {
      m_zrect = QRect();
    }
    else if(m_zmouseMode == 1) // zoom in
    {
      int w = abs(m_zpoint.x() - m_zpressPoint.x()) + 1;
      int h = abs(m_zpoint.y() - m_zpressPoint.y()) + 1;
      if(m_zpoint.x() > m_zpressPoint.x() && m_zpoint.y() > m_zpressPoint.y())
        m_zrect.setRect(m_zpressPoint.x(), m_zpressPoint.y(), w, h);
      else if(m_zpoint.x() > m_zpressPoint.x() && m_zpoint.y() < m_zpressPoint.y())
        m_zrect.setRect(m_zpressPoint.x(), m_zpoint.y(), w, h);
      else if(m_zpoint.x() < m_zpressPoint.x() && m_zpoint.y() < m_zpressPoint.y())
        m_zrect.setRect(m_zpoint.x(), m_zpoint.y(), w, h);
      else
        m_zrect.setRect(m_zpoint.x(), m_zpressPoint.y(), w, h);
      drawRectArea();
    }
  }
}

void te::qt::widgets::LayoutEditor::mouseReleaseEvent(QMouseEvent* e)
{
  double zoomFat = 1.5;

  if(m_zmouseMode == 3)
  {
    unsetCursor();
    QCursor cursor(Qt::OpenHandCursor);
    setCursor(cursor);
  }

  // zoom and Pan on the paper view
  if(e->modifiers() == Qt::NoModifier)
  {
    if(e->button() == Qt::LeftButton) // botao que causou o evento
    {
      if(m_zmouseMode == 1) // zoom in area (paper view)
      {
        if(m_zrect.isValid() && m_zrect.width() > 2 && m_zrect.height() > 2)
        {
          QRectF r(m_zrect.left(), m_zrect.top(), m_zrect.width(), m_zrect.height());
          r = m_matrixPaperViewToVp.inverted().mapRect(r);
          adjustAspectRatio(r, m_paperViewRect);
          m_paperViewRect = r;
          createWorkingArea();
          draw();
        }
        else // zoom in (paper view)
        {
          QPointF c(m_zpressPoint.x(), m_zpressPoint.y());
          QPointF cf = m_matrixPaperViewToVp.inverted().map(c);
          double w = m_paperViewRect.width() / zoomFat;
          double h = m_paperViewRect.height() / zoomFat;
          m_paperViewRect = QRectF(0, 0, w, h);
          m_paperViewRect.moveCenter(cf);
          createWorkingArea();
          draw();
        }
        m_zrect = QRect();
        drawRectArea();
      }
      else if(m_zmouseMode == 2) // zoom out (paper view)
      {
        QPointF c(m_zpressPoint.x(), m_zpressPoint.y());
        QPointF cf = m_matrixPaperViewToVp.inverted().map(c);
        double w = m_paperViewRect.width() * zoomFat;
        double h = m_paperViewRect.height() * zoomFat;
        m_paperViewRect = QRectF(0, 0, w, h);
        m_paperViewRect.moveCenter(cf);
        createWorkingArea();
        draw();
      }
      else if(m_zmouseMode == 3) // pan (paper view)
      {
        m_zpanEnd = true;
        QPointF pfrom(m_zpressPoint.x(), m_zpressPoint.y());
        pfrom =  m_matrixPaperViewToVp.inverted().map(pfrom);

        QPointF pto(e->pos().x(), e->pos().y());
        pto =  m_matrixPaperViewToVp.inverted().map(pto);

        QPointF dif = pto - pfrom;
        QPointF c = m_paperViewRect.center();
        c -= dif;
        m_paperViewRect.moveCenter(c);
        createWorkingArea();
        showAllObjects();
        draw();
      }
    }
  }

  // Zoom and Pan on the data frame
  else if(e->modifiers() == Qt::ControlModifier && m_zdataFrame)
  {
    // data frame em mm
    QRectF fr = m_zdataFrame->getFrameRect();
    // data frame em pixels
    QRectF frp = m_matrixPaperViewToVp.mapRect(fr);

    double magDecl = m_zdataFrame->getMagneticDeclination();
    QMatrix auxMatrix;
    auxMatrix.translate(frp.width()/2., frp.height()/2.);
    auxMatrix.rotate(magDecl);
    auxMatrix.translate(-frp.width()/2., -frp.height()/2.);

    if(e->button() == Qt::LeftButton) // botao que causou o evento
    {
      if(m_zmouseMode == 1) // zoom in area (data frame)
      {
        if(m_zrect.isValid() && m_zrect.width() > 2 && m_zrect.height() > 2)
        {
          insertCopy2Undo(m_zdataFrame);

          // m_zrect tem posicao em relacao ao layout editor
          // mova para ser em relacao ao frame
          QPoint cc = m_zrect.center();
          cc -= m_zdataFrame->pos();
          m_zrect.moveCenter(cc);

          // faca a correcao da declinacao magnetica sobre cc
          cc = auxMatrix.inverted().map(cc);

          // retorne a posicao para ser em relacao ao layout editor
          cc += m_zdataFrame->pos();
          m_zrect.moveCenter(cc);

          QRectF zr(m_zrect.left(), m_zrect.top(), m_zrect.width(), m_zrect.height());

          // area do zoom em mm
          zr = m_matrixPaperViewToVp.inverted().mapRect(zr);         
          // ajusta a relacao de aspecto da area do zoom em relacao ao do frame
          adjustAspectRatio(zr, fr);

          // area do world
          QRectF r = m_zdataFrame->getDataRect();
          
          // calcule new world data rect
          double x1 = r.left() + ((zr.left() - fr.left()) / fr.width()) * r.width();
          double y1 = r.top() + ((zr.top() - fr.top()) / fr.height()) * r.height();
          double w = r.width() * (zr.width() / fr.width());
          double h = r.height() * (zr.height() / fr.height());
          QRectF nr(x1, y1, w, h);
          m_zdataFrame->setDataRect(nr);
          if(m_zdataFrame->getGraphicScaleFrame())
          {
            double step = m_zdataFrame->getGraphicScaleFrame()->getStep();
            step *= zr.width() / fr.width();
            m_zdataFrame->getGraphicScaleFrame()->findNiceStep(step);
          }
          createWorkingArea(false);
          draw();
        }
        else // zoom in (data frame)
        {
          insertCopy2Undo(m_zdataFrame);

          // posicao em relacao ao layout editor
          QPointF c(m_zpressPoint.x(), m_zpressPoint.y());

          // mova para ser em relacao ao frame
          c -= m_zdataFrame->pos();

          // faca a correcao da declinacao magnetica para c
          c = auxMatrix.inverted().map(c);

          // retorne a posicao para ser em relacao ao layout editor
          c += m_zdataFrame->pos();

          // centro do zoom em mm
          QPointF cf = m_matrixPaperViewToVp.inverted().map(c);

          // area do world
          QRectF r = m_zdataFrame->getDataRect();

          // calcule new world data center
          double x1 = r.left() + ((cf.x() - fr.left()) / fr.width()) * r.width();
          double y1 = r.top() + ((cf.y() - fr.top()) / fr.height()) * r.height();

          // calcule novo world data rect
          double w = r.width() / zoomFat;
          double h = r.height() / zoomFat;
          QRectF nr(0, 0, w, h);
          nr.moveCenter(QPointF(x1, y1));

          m_zdataFrame->setDataRect(nr);
          if(m_zdataFrame->getGraphicScaleFrame())
          {
            double step = m_zdataFrame->getGraphicScaleFrame()->getStep();
            step /= zoomFat;
            m_zdataFrame->getGraphicScaleFrame()->findNiceStep(step);
          }
          createWorkingArea(false);
          draw();
        }
        m_zrect = QRect();
        drawRectArea();
      }
      else if(m_zmouseMode == 2) // zoom out (data frame)
      {
        insertCopy2Undo(m_zdataFrame);

        // posicao em relacao ao layout editor
        QPointF c(m_zpressPoint.x(), m_zpressPoint.y());

        // mova para ser em relacao ao frame
        c -= m_zdataFrame->pos();

        // faca a correcao da declinacao magnetica para c
        c = auxMatrix.inverted().map(c);

        // retorne a posicao para ser em relacao ao layout editor
        c += m_zdataFrame->pos();

        // centro do zoom em mm
        QPointF cf = m_matrixPaperViewToVp.inverted().map(c);

        // area do world
        QRectF r = m_zdataFrame->getDataRect();

        // calcule new world data center
        double x1 = r.left() + ((cf.x() - fr.left()) / fr.width()) * r.width();
        double y1 = r.top() + ((cf.y() - fr.top()) / fr.height()) * r.height();

        // calcule novo world data rect
        double w = r.width() * zoomFat;
        double h = r.height() * zoomFat;
        QRectF nr(0, 0, w, h);
        nr.moveCenter(QPointF(x1, y1));

        m_zdataFrame->setDataRect(nr);
        if(m_zdataFrame->getGraphicScaleFrame())
        {
          double step = m_zdataFrame->getGraphicScaleFrame()->getStep();
          step *= zoomFat;
          m_zdataFrame->getGraphicScaleFrame()->findNiceStep(step);
        }
        createWorkingArea(false);
        draw();
      }
      else if(m_zmouseMode == 3) // pan (data frame)
      {
        insertCopy2Undo(m_zdataFrame);

        m_zpanEnd = true;
        QPointF pfrom(m_zpressPoint.x(), m_zpressPoint.y());
        QPointF pto(e->pos().x(), e->pos().y());

        // faca a correcao da declinacao magnetica para pfrom e pto
        pfrom = auxMatrix.inverted().map(pfrom);
        pto = auxMatrix.inverted().map(pto);

        // converta pfrom e pto para mm
        pfrom =  m_matrixPaperViewToVp.inverted().map(pfrom);
        pto =  m_matrixPaperViewToVp.inverted().map(pto);

        // calcule o deslocamento
        QPointF dif = pto - pfrom;

        QPointF fc = fr.center();

        // new center do frame em mm
        fc -= dif;

        // calcule new world rect
        // old world rect
        QRectF r = m_zdataFrame->getDataRect();
        double x1 = r.left() + ((fc.x() - fr.left()) / fr.width()) * r.width();
        double y1 = r.top() + ((fc.y() - fr.top()) / fr.height()) * r.height();

        // new world rect
        r.moveCenter(QPointF(x1, y1));

        m_zdataFrame->setDataRect(r);
        draw();
      }
    }
  }
  m_zdataFrame = 0;
}

void te::qt::widgets::LayoutEditor::drawRectArea()
{
  QPixmap* pixmap = getDraftPixmap();
  pixmap->fill(QColor(0, 0, 0, 0));

  if(m_zrect.isValid())
  {
    QPainter painter(pixmap);
    if(m_zdataFrame)
    {
      painter.setPen(Qt::red);
      painter.setBrush(QColor(255, 0, 0, 80));
    }
    else
    {
      painter.setPen(Qt::blue);
      painter.setBrush(QColor(0, 0, 255, 80));
    }
    painter.drawRect(m_zrect);
  }
  update();
}

void te::qt::widgets::LayoutEditor::setMouseMode(int m)
{
  m_zmouseMode = m;

  if(m_zmouseMode == 0)
    sendEventToChildren(true);
  else
    sendEventToChildren(false);

  QDir dir;
  if(dir.exists("../../../terralib5/resources/themes/terralib/32x32/zoomInCursor.png"))
    if(m_zoomInPixmap == 0)
      m_zoomInPixmap = new QPixmap("../../../terralib5/resources/themes/terralib/32x32/zoomInCursor.png");

  if(dir.exists("../../../terralib5/resources/themes/terralib/32x32/zoomOutCursor.png"))
    if(m_zoomOutPixmap == 0)
      m_zoomOutPixmap = new QPixmap("../../../terralib5/resources/themes/terralib/32x32/zoomOutCursor.png");

  //if(dir.exists("../../../terralib5/resources/themes/terralib/48x48/panCursor.png"))
  //  if(m_panPixmap == 0)
  //    m_panPixmap = new QPixmap("../../../terralib5/resources/themes/terralib/48x48/panCursor.png");
 

  QCursor* cursor = 0;

  if(m_zmouseMode == 0)
    cursor = new QCursor(Qt::ArrowCursor);
  else if(m_zmouseMode == 1)
  {
    if(m_zoomInPixmap)
      cursor = new QCursor(*m_zoomInPixmap, 9, 9);
    else
      cursor = new QCursor(Qt::PointingHandCursor);
  }
  else if(m_zmouseMode == 2)
  {
    if(m_zoomOutPixmap)
      cursor = new QCursor(*m_zoomOutPixmap, 9, 9);
    else
      cursor = new QCursor(Qt::SizeAllCursor);
  }
  else if(m_zmouseMode == 3)
  {
    //if(m_panPixmap)
    //  cursor = new QCursor(*m_panPixmap, 24, 24);
    //else
    //  cursor = new QCursor(Qt::OpenHandCursor);
    cursor = new QCursor(Qt::OpenHandCursor);
  }

  unsetCursor();
  setCursor(*cursor);
  delete cursor;
}

void te::qt::widgets::LayoutEditor::adjustAspectRatio(QRectF& r, const QRectF& ref)
{
  double llx = r.left();
  double lly = r.top();
  double urx = r.right();
  double ury = r.bottom();
  double ww = r.width();
  double wh = r.height();

  double widthByHeight = ref.width() / ref.height();

  if(widthByHeight > ww / wh)
  {
    double v = ww;

    ww = wh * widthByHeight;
    llx = llx - (ww - v) / 2.0;
    urx = llx + ww;
  }
  else
  {
    double v = wh;

    wh = ww / widthByHeight;
    lly = lly - (wh - v) / 2.0;
    ury = lly + wh;
  }

  r = QRectF(llx, lly, urx-llx, ury-lly);
}

void te::qt::widgets::LayoutEditor::hideAllObjects()
{
    std::vector<te::qt::widgets::LayoutObject*>::iterator it;
    for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
      (*it)->hide();
}

void te::qt::widgets::LayoutEditor::showAllObjects()
{
    std::vector<te::qt::widgets::LayoutObject*>::iterator it;
    for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
      (*it)->show();
}

void te::qt::widgets::LayoutEditor::wheelEvent(QWheelEvent* e)
{
  int i = m_zmouseMode;
  int d = e->delta();
  if(d > 0)
    i++;
  else
    i--;

  if(i > 3)
    i = 0;  
  else if(i < 0)
    i = 3;
  setMouseMode(i);
}

void te::qt::widgets::LayoutEditor::sendEventToChildren(bool b)
{
  std::vector<te::qt::widgets::LayoutObject*>::iterator it;
  for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
    (*it)->sendEventToChildren(b);

  for(it = m_undoLayoutObjects.begin(); it != m_undoLayoutObjects.end(); ++it)
    (*it)->sendEventToChildren(b);

  if(b == false)
  {
    setFrameSelected(0);
    while(QApplication::overrideCursor())
      QApplication::restoreOverrideCursor();

    raiseDraftLayoutEditor();    
    m_draftLayoutEditor->installEventFilter(this);
  }
  else
  {
    lowerDraftLayoutEditor();
    m_draftLayoutEditor->removeEventFilter(this);
  }
}

bool te::qt::widgets::LayoutEditor::eventFilter(QObject* obj, QEvent* e)
{
  if(e->type() == QEvent::Paint)
    return false;


  if(obj == m_draftLayoutEditor)
  {
    if(e->type() == QEvent::DragEnter)
    {
      QDragEnterEvent* dragEnterEvent = (QDragEnterEvent*)e;
      const QMimeData* mime = dragEnterEvent->mimeData();
      QString s = mime->data("application/x-terralib;value=\"DraggedItems\"").constData();
      if(s.isEmpty() == false)
      {
        std::vector<te::qt::widgets::LayoutObject*>::iterator it;
        for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
        {
          if((*it)->windowTitle() == "DataFrame")
          {
            (*it)->sendEventToChildren(true);
            (*it)->raise();
          }
        }
        return false;
      }
    }
    else if(e->type() == QEvent::DragMove)
      return false;
    else if(e->type() == QEvent::Drop)
      return false;
    else if(e->type() == QEvent::Resize)
      return false;
    else
      m_draftLayoutEditor->raise();

    if(e->type() == QEvent::MouseButtonPress)
      mousePressEvent((QMouseEvent*)e);
    else if(e->type() == QEvent::MouseMove)
      mouseMoveEvent((QMouseEvent*)e);
    else if(e->type() == QEvent::MouseButtonRelease)
      mouseReleaseEvent((QMouseEvent*)e);
    else if(e->type() == QEvent::Wheel)
      wheelEvent((QWheelEvent*)e);
  }

  return true;
}

void te::qt::widgets::LayoutEditor::keyPressEvent(QKeyEvent* e)
{
  m_difTopLeft = QPointF(0., 0.);
  m_difBottomRight = QPointF(0., 0.);

  int k = e->key();
  int modifier = e->modifiers();
  QString text = e->text();

  switch(e->key())
  {
  case Qt::Key_Minus: // ZOOM OUT
    {
      if(m_layoutObjectSelected)
      {
        double scale = m_layoutObjectSelected->getScale() * 1.2;
        m_layoutObjectSelected->setScale(scale);
        draw();
      }
      else
      {
        double w = m_paperViewRect.width() * 1.2;
        double h = m_paperViewRect.height() * 1.2;
        QPointF c = m_paperViewRect.center();
        m_paperViewRect.setWidth(w);
        m_paperViewRect.setHeight(h);
        m_paperViewRect.moveCenter(c);
        createWorkingArea();
        draw();
      }
    }
    break;
  case Qt::Key_Plus: // ZOOM IN
    {
      if(m_layoutObjectSelected)
      {
        double scale = m_layoutObjectSelected->getScale() / 1.2;
        m_layoutObjectSelected->setScale(scale);
        draw();
      }
      else
      {
        double  w = m_paperViewRect.width() / 1.2;
        double  h = m_paperViewRect.height() / 1.2;
        QPointF c = m_paperViewRect.center();
        m_paperViewRect.setWidth(w);
        m_paperViewRect.setHeight(h);
        m_paperViewRect.moveCenter(c);
        createWorkingArea();
        draw();
      }
    }
    break;
  case Qt::Key_Slash: // ZOOM OUT
    {
      if(m_layoutObjectSelected)
      {
        double scale = m_layoutObjectSelected->getScale() * 2;
        m_layoutObjectSelected->setScale(scale);
        draw();
      }
      else
      {
        double w = m_paperViewRect.width() * 2;
        double h = m_paperViewRect.height() * 2;
        QPointF c = m_paperViewRect.center();
        m_paperViewRect.setWidth(w);
        m_paperViewRect.setHeight(h);
        m_paperViewRect.moveCenter(c);
        createWorkingArea();
        draw();
      }
    }
    break;
  case Qt::Key_Asterisk: // ZOOM IN
    {
      if(m_layoutObjectSelected)
      {
        double scale = m_layoutObjectSelected->getScale() / 2;
        m_layoutObjectSelected->setScale(scale);
        draw();
      }
      else
      {
        double w = m_paperViewRect.width() / 2;
        double h = m_paperViewRect.height() / 2;
        QPointF c = m_paperViewRect.center();
        m_paperViewRect.setWidth(w);
        m_paperViewRect.setHeight(h);
        m_paperViewRect.moveCenter(c);
        createWorkingArea();
        draw();
      }
    }
    break;
  case Qt::Key_F1: // ALL RULLER GRID
    {
      if(m_rulerGridLines != 1 || m_showRulerGrid == false)
      {
        te::qt::widgets::EditorInfo* f = new te::qt::widgets::EditorInfo(*m_editorState);
        m_undoLayoutObjects.push_back(f);

        m_rulerGridLines = 1;
        createRulerGrid();
        m_showRulerGrid = false;
        showRulerGrid();
        copyState();
      }
    }
    break;
  case Qt::Key_F2: // PARCIAL RULLER GRID
    {
      if(m_rulerGridLines != 2 || m_showRulerGrid == false)
      {
        te::qt::widgets::EditorInfo* f = new te::qt::widgets::EditorInfo(*m_editorState);
        m_undoLayoutObjects.push_back(f);

        m_rulerGridLines = 2;
        createRulerGrid();
        m_showRulerGrid = false;
        showRulerGrid();
        copyState();
      }
    }
    break;
  case Qt::Key_F3: // PARCIAL RULLER GFRID
    {
      if(m_rulerGridLines != 3 || m_showRulerGrid == false)
      {
        te::qt::widgets::EditorInfo* f = new te::qt::widgets::EditorInfo(*m_editorState);
        m_undoLayoutObjects.push_back(f);

        m_rulerGridLines = 3;
        createRulerGrid();
        m_showRulerGrid = false;
        showRulerGrid();
        copyState();
      }
    }
    break;
  case Qt::Key_F4: // NO RULLER GRID
    {
      if(m_showRulerGrid == true)
      {
        te::qt::widgets::EditorInfo* f = new te::qt::widgets::EditorInfo(*m_editorState);
        m_undoLayoutObjects.push_back(f);

        hideRulerGrid();
        copyState();
      }
    }
    break;
  case Qt::Key_Left: // ALIGN LEFT or PAN
    {
      if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "TextFrame")
      {
        te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
        if(f->getAlign() != Qt::AlignLeft)
        {
          te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
          f->setAlign(Qt::AlignLeft);
          f->adjust();
          f->showSelectionPoints();
          f->toolTip(f->getGlobalPositionCenter(), "Align");
          insertCopy2Undo(f);
        }
      }
      else if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "DataFrame")
      {
        te::qt::widgets::DataFrame* f = (te::qt::widgets::DataFrame*)m_layoutObjectSelected;
        QRectF r = f->getDataRect();
        double w = r.width();
        r = QRectF(r.left() - w / 5, r.top(), w, r.height());
        f->setDataRect(r);
        draw();
      }
      else
      {
        double w = m_paperViewRect.width();
        m_paperViewRect = QRectF(m_paperViewRect.left() - w / 5, m_paperViewRect.top(), w, m_paperViewRect.height());
        createWorkingArea();
        draw();
      }
    }
    break;
  case Qt::Key_Right: // ALIGN RIGHT or PAN
    {
      if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "TextFrame")
      {
        te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
        if(f->getAlign() != Qt::AlignRight)
        {
          te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
          f->setAlign(Qt::AlignRight);
          f->adjust();
          f->showSelectionPoints();
          f->toolTip(f->getGlobalPositionCenter(), "Align");
          insertCopy2Undo(f);
        }
      }
      else if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "DataFrame")
      {
        te::qt::widgets::DataFrame* f = (te::qt::widgets::DataFrame*)m_layoutObjectSelected;
        QRectF r = f->getDataRect();
        double w = r.width();
        r = QRectF(r.left() + w / 5, r.top(), w, r.height());
        f->setDataRect(r);
        draw();
      }
      else
      {
        double w = m_paperViewRect.width();
        m_paperViewRect = QRectF(m_paperViewRect.left() + w / 5, m_paperViewRect.top(), w, m_paperViewRect.height());
        createWorkingArea();
        draw();
      }
    }
    break;
  case Qt::Key_Up: // ALIGN CENTER or PAN
    {
      if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "TextFrame")
      {
        te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
        if(f->getAlign() != Qt::AlignHCenter)
        {
          te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
          f->setAlign(Qt::AlignHCenter);
          f->adjust();
          f->showSelectionPoints();
          f->toolTip(f->getGlobalPositionCenter(), "Align");
          insertCopy2Undo(f);
        }
      }
      else if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "DataFrame")
      {
        te::qt::widgets::DataFrame* f = (te::qt::widgets::DataFrame*)m_layoutObjectSelected;
        QRectF r = f->getDataRect();
        double h = r.height();
        r = QRectF(r.left(), r.top() + h / 5, r.width(), h);
        f->setDataRect(r);
        draw();
      }
      else
      {
        double h = m_paperViewRect.height();
        m_paperViewRect = QRectF(m_paperViewRect.left(), m_paperViewRect.top() + h/5, m_paperViewRect.width(), h);
        createWorkingArea();
        draw();
      }
    }
    break;
  case Qt::Key_Down: // ALIGN CENTER or PAN
    {
      if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "TextFrame")
      {
        te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
        if(f->getAlign() != Qt::AlignHCenter)
        {
          te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
          f->setAlign(Qt::AlignHCenter);
          f->adjust();
          f->showSelectionPoints();
          f->toolTip(f->getGlobalPositionCenter(), "Align");
          insertCopy2Undo(f);
        }
      }
      else if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "DataFrame")
      {
        te::qt::widgets::DataFrame* f = (te::qt::widgets::DataFrame*)m_layoutObjectSelected;
        QRectF r = f->getDataRect();
        double h = r.height();
        r = QRectF(r.left(), r.top() - h / 5, r.width(), h);
        f->setDataRect(r);
        draw();
      }
      else
      {
        double h = m_paperViewRect.height();
        m_paperViewRect = QRectF(m_paperViewRect.left(), m_paperViewRect.top() - h/5, m_paperViewRect.width(), h);
        createWorkingArea();
        draw();
      }
    }
    break;
  case Qt::Key_W: // WEIGHT
    {
      if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "TextFrame")
      {
        te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
        QFont::Weight weight = f->getWeight();
        if(modifier == Qt::NoModifier)
        {
          if(weight == QFont::Light)
            weight = QFont::Normal;
          else if(weight == QFont::Normal)
            weight = QFont::DemiBold;
          else if(weight == QFont::DemiBold)
            weight = QFont::Bold;
          else if(weight == QFont::Bold)
            weight = QFont::Black;
          else
            weight = QFont::Light;
        }
        else
        {
          if(weight == QFont::Light)
            weight = QFont::Black;
          else if(weight == QFont::Black)
            weight = QFont::Bold;
          else if(weight == QFont::Bold)
            weight = QFont::DemiBold;
          else if(weight == QFont::DemiBold)
            weight = QFont::Normal;
          else
            weight = QFont::Light;
        }

        f->setWeight(weight);
        f->adjust();
        f->showSelectionPoints();
        f->toolTip(f->getGlobalPositionCenter(), "Weight");
        insertCopy2Undo(f);
      }
    }
    break;
  case Qt::Key_C: // CAPITALIZATION
    {
      if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "TextFrame")
      {
        te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
        QFont::Capitalization cap = f->getCapitalization();
        if(cap == QFont::MixedCase)
          cap = QFont::AllUppercase;
        else if(cap == QFont::AllUppercase)
          cap = QFont::AllLowercase;
        else if(cap == QFont::AllLowercase)
          cap = QFont::SmallCaps;
        else if(cap == QFont::SmallCaps)
          cap = QFont::Capitalize;
        else
          cap = QFont::MixedCase;

        f->setCapitalization(cap);
        f->adjust();
        f->showSelectionPoints();
        f->toolTip(f->getGlobalPositionCenter(), "Capitalization");
        insertCopy2Undo(f);
      }
    }
    break;
  case Qt::Key_O: // OVERLINE
    {
      if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "TextFrame")
      {
        te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
        if(f->getOverline())
          f->setOverline(false);
        else
          f->setOverline(true);

        f->adjust();
        f->showSelectionPoints();
        f->toolTip(f->getGlobalPositionCenter(), "Overline");
        insertCopy2Undo(f);
      }
    }
    break;
  case Qt::Key_U: // UNDERLINE
    {
      if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "TextFrame")
      {
        te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
        if(f->getUnderline())
          f->setUnderline(false);
        else
          f->setUnderline(true);

        f->adjust();
        f->showSelectionPoints();
        f->toolTip(f->getGlobalPositionCenter(), "Underline");
        insertCopy2Undo(f);
      }
    }
    break;
  case Qt::Key_K: // STRIKE OUT
    {
      if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "TextFrame")
      {
        te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
        if(f->getStrikeOut())
          f->setStrikeOut(false);
        else
          f->setStrikeOut(true);

        f->adjust();
        f->showSelectionPoints();
        f->toolTip(f->getGlobalPositionCenter(), "StrikeOut");
        insertCopy2Undo(f);
      }
    }
    break;
  case Qt::Key_S: // FONT STYLE
    {
      if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "TextFrame")
      {
        te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
        QFont::Style style = f->getStyle();
        if(style == QFont::StyleNormal)
          style = QFont::StyleItalic;
        else if(style == QFont::StyleItalic)
          style = QFont::StyleOblique;
        else
          style = QFont::StyleNormal;

        f->setStyle(style);
        f->adjust();
        f->showSelectionPoints();
        f->toolTip(f->getGlobalPositionCenter(), "Style");
        insertCopy2Undo(f);
      }
    }
    break;
  case Qt::Key_Space: // LETTER SPACING
    {
      if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "TextFrame")
      {
        te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
        double letterSpacing = f->getLetterSpacing();
        if(modifier == Qt::NoModifier)
          letterSpacing = letterSpacing * 1.1;
        else
          letterSpacing = letterSpacing / 1.1;

        f->setLetterSpacing(letterSpacing);
        f->adjust();
        f->showSelectionPoints();
        f->toolTip(f->getGlobalPositionCenter(), "LetterSpacing");
        insertCopy2Undo(f);
      }
    }
    break;
  case Qt::Key_L: // WORD SPACING
    {
      if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "TextFrame")
      {
        te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
        double wordSpacing = f->getWordSpacing();
        if(modifier == Qt::NoModifier)
          wordSpacing += 1;
        else
          wordSpacing -= 1;

        f->setWordSpacing(wordSpacing);
        f->adjust();
        f->showSelectionPoints();
        f->toolTip(f->getGlobalPositionCenter(), "WordSpacing");
        insertCopy2Undo(f);
      }
    }
    break;
  case Qt::Key_T:
    {
      if(modifier == Qt::AltModifier)
      {
        if(m_layoutObjectSelected)
        {
          if(m_layoutObjectSelected->windowTitle() == "TextFrame") // change text string
          {
            te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
            QString s = f->getText();
            bool ok;
            QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                                      tr("Change Text:"), QLineEdit::Normal,
                                                      s, &ok);
            if(ok && !text.isEmpty())
            {
              f->setText(text);
              f->adjust();
              f->showSelectionPoints();
            }
            
            insertCopy2Undo(f);
            draw();
          }
          if(m_layoutObjectSelected->windowTitle() == "DataFrame") // change geographical data
          {
            QMessageBox::information(this, "Change Geographical Data", "Use drag drop...");
            return;
          }
        }
        else // insert Text
        {
          QPointF c =  m_paperViewRect.center();
          te::qt::widgets::TextFrame* nf = new te::qt::widgets::TextFrame(c , "new text", 50, this, Qt::Widget);
          nf->setId(m_idCount++); // numero do frame
          nf->setNew(true);  // para o control Z poder deletar o frame criado
          insertCopy2Undo(nf); // esta linha tem que vir antes da insercao do frame no map editor (para preparar o undo).
          insert(nf);
          nf->adjust();
          nf->showSelectionPoints();
          m_layoutObjectSelected = nf;
          draw();
        }
      }
    }
    break;
  case Qt::Key_Delete: // DELETE FRAME
    {
      if(m_layoutObjectSelected)
      {
        m_layoutObjectSelected->hideSelectionPoints();
        remove(m_layoutObjectSelected);
        insertCopy2Undo(m_layoutObjectSelected);
        m_layoutObjectSelected->hide();
        delete m_layoutObjectSelected;
        m_layoutObjectSelected = 0;
        te::qt::widgets::LayoutObject* f = m_undoLayoutObjects[m_undoLayoutObjects.size()-1];
        f->hideSelectionPoints();
        f->hide();
        setFrameSelected(0);
        update();
      }
    }
    break;
  case Qt::Key_Z: // UNDO
    {
      if(modifier == Qt::ControlModifier)
      {
        m_layoutObjectSelected = 0;
        while(QApplication::overrideCursor())
          QApplication::restoreOverrideCursor();

        te::qt::widgets::LayoutObject* undof = 0;
        std::vector<te::qt::widgets::LayoutObject*>::reverse_iterator it = m_undoLayoutObjects.rbegin();
        if(it == m_undoLayoutObjects.rend())
        {
          QMessageBox::information(this, "Undo", "Undo Buffer is Empty");
          return;
        }
        getDraftPixmap()->fill(Qt::transparent);

        undof = *it;
        if(undof->getId() != 0)
        {
          te::qt::widgets::LayoutObject* f = find(undof->getId()); // busca o frame no vetor de layoutObject
          if(f) // recuperando frame editado
          {
            if(undof->isNew()) // este frame foi criado por edicao - remova da lista de frames
            {
              remove(f);
              delete f;
              removeUndo(undof); // remove do buffer de undo
              delete undof;
              undof = 0;
            }
            else
            {
              // troque o frame pelo frame do undo
              remove(f); // remove frame do vetor de layoutObject
              removeUndo(undof); // remove do buffer de undo
              delete f;
              insert(undof); // faca a troca
            }
          }
          // se o frame nao existe � porque ele foi deletado.
          else // recuperando frame deletado
          {
            removeUndo(undof);
            insert(undof);
          }

          if(undof)
          {
            if(undof->windowTitle() == "DataFrame")
            {
              te::qt::widgets::DataFrame* df = (te::qt::widgets::DataFrame*)undof;
              te::map::AbstractLayerPtr data(df->getData());
              QRectF dr = df->getDataRect();
              df->setData(0, df->getMapDisplay()->getSRID(), df->getDataRect());
              df->setData(data, df->getMapDisplay()->getSRID(), df->getDataRect());
              df->draw();
            }
            undof->show();
            undof->raise();
            undof->adjust();
          }
          update();
        }
        // id = 0 significa alteracao no state do map editor. Recupere o state anterior.
        else // working area is changed
        {
          te::qt::widgets::EditorInfo* ei = (te::qt::widgets::EditorInfo*)undof;
          m_paperViewRect = ei->m_paperViewRect;
          m_paperSize = ei->m_paperSize;
          m_rulerGridLines = ei->m_rulerGridLines;
          QRect r = ei->m_rect;
          m_putUndo = false;
          if(r.topLeft() != pos())
            move(r.topLeft());
          if(r.size() != rect().size())
            resize(r.width(), r.height());
          else
            createWorkingArea(false);
          m_putUndo = true;

          if(ei->m_showRulerGrid)
          {
            m_showRulerGrid = false;
            showRulerGrid();
          }
          else
            draw();
          removeUndo(undof);
          delete undof;
          copyState();
        }
      }
    }
    break;
  case Qt::Key_V: // COPY FRAME
    {
      if(modifier == Qt::ControlModifier)
      {
        if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "TextFrame")
        {
          te::qt::widgets::TextFrame* f = (te::qt::widgets::TextFrame*)m_layoutObjectSelected;
          te::qt::widgets::TextFrame* nf = new te::qt::widgets::TextFrame(*f);
          nf->setId(m_idCount++); // numero do frame
          nf->setNew(true); // para o control Z poder deletar o frame criado
          insertCopy2Undo(nf); // esta linha tem que vir antes da insercao do frame no map editor (para preparar o undo).
          insert(nf);
          nf->adjust();
          nf->showSelectionPoints();
          m_layoutObjectSelected = nf;
          update();
        }
        else if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "DataFrame")
        {
          te::qt::widgets::DataFrame* f = (te::qt::widgets::DataFrame*)m_layoutObjectSelected;
          te::qt::widgets::DataFrame* nf = new te::qt::widgets::DataFrame(*f);
          nf->setId(m_idCount++); // numero do frame
          nf->setNew(true); // para o control Z poder deletar o frame criado
          insertCopy2Undo(nf); // esta linha tem que vir antes da insercao do frame no map editor (para preparar o undo).
          insert(nf);
          nf->adjust();
          nf->showSelectionPoints();
          m_layoutObjectSelected = nf;
          nf->getMapDisplay()->refresh();
          update();
        }
      }
    }
    break;
  case Qt::Key_R: // REMOVE MAP DISPLAY EVENT FILTER
    sendEventToChildren(false);
    break;
  case Qt::Key_I: // INSERT MAP DISPLAY EVENT FILTER
    sendEventToChildren(true);
    break;

    case Qt::Key_P: // CHANGE PROJECTION
      if(m_layoutObjectSelected && m_layoutObjectSelected->windowTitle() == "DataFrame")
      {
        te::qt::widgets::DataFrame* f = (te::qt::widgets::DataFrame*)m_layoutObjectSelected;
        if(f->getMapDisplay()->getSRID() == 3031)
          f->getMapDisplay()->setSRID(f->getData()->getSRID());
        else
          f->getMapDisplay()->setSRID(3031); // TE_SRS_WGS84_ANTARTIC_POLAR_STEREOGRAPHIC 
        draw();
      }
      break;

    case Qt::Key_F: // CHANGE PAPER FORMAT (PORTRAIT / LANDSCAPE)
      {
        copyState();
        //m_paperViewRect = QRectF();
        setPaperSize(QSize(m_paperSize.height(), m_paperSize.width()));
      }
      break;

    case Qt::Key_A: // FIT PAPER
      {
        copyState();
        resetPaperView();
        setPaperSize(m_paperSize);
      }
      break;

    case Qt::Key_0: // A0
      {
        if(m_paperSize.width() > m_paperSize.height())
          m_paperSize = QSize(1189, 841);
        else
          m_paperSize = QSize(841, 1189);
        setPaperSize(m_paperSize);
        copyState();
      }
      break;

    case Qt::Key_1: // A1
      {
        if(m_paperSize.width() > m_paperSize.height())
          m_paperSize = QSize(841, 594);
        else
          m_paperSize = QSize(594, 841);
        setPaperSize(m_paperSize);
        copyState();
      }
      break;

    case Qt::Key_2: // A2
      {
        if(m_paperSize.width() > m_paperSize.height())
          m_paperSize = QSize(594, 420);
        else
          m_paperSize = QSize(420, 594);
        setPaperSize(m_paperSize);
        copyState();
      }
      break;

    case Qt::Key_3: // A3
      {
        if(m_paperSize.width() > m_paperSize.height())
          m_paperSize = QSize(420, 297);
        else
          m_paperSize = QSize(297, 420);
        setPaperSize(m_paperSize);
        copyState();
      }
      break;

    case Qt::Key_4: // A4
      {
        if(m_paperSize.width() > m_paperSize.height())
          m_paperSize = QSize(297, 210);
        else
          m_paperSize = QSize(210, 297);
        setPaperSize(m_paperSize);
        copyState();
      }
      break;

    default:
      break;
  }
}

QMatrix te::qt::widgets::LayoutEditor::getMatrixPaperViewToVp()
{
  return m_matrixPaperViewToVp;
}

QMatrix te::qt::widgets::LayoutEditor::getMatrix()
{
  return m_matrix;
}

void te::qt::widgets::LayoutEditor::setFrameSelected(te::qt::widgets::LayoutObject* f)
{
  if(m_layoutObjectSelected)
      m_layoutObjectSelected->hideSelectionPoints();

  m_layoutObjectSelected = f;
  if(m_layoutObjectSelected)
    m_layoutObjectSelected->showSelectionPoints();
}

te::qt::widgets::LayoutObject* te::qt::widgets::LayoutEditor::getFrameSelected()
{
  return m_layoutObjectSelected;
}

//void te::qt::widgets::LayoutEditor::setDataFrameSelected(te::qt::widgets::DataFrame* df)
//{
//  if(m_dataFrameSelected)
//    m_dataFrameSelected->hideSelectionPoints();
//
//  m_dataFrameSelected = df;
//  if(m_dataFrameSelected)
//    m_dataFrameSelected->showSelectionPoints();
//}
//
//te::qt::widgets::DataFrame* te::qt::widgets::LayoutEditor::getDataFrameSelected()
//{
//  return m_dataFrameSelected;
//}
//
//void te::qt::widgets::LayoutEditor::setTextFrameSelected(te::qt::widgets::TextFrame* tf)
//{
//  if(m_textFrameSelected)
//    m_textFrameSelected->hideSelectionPoints();
//
//  m_textFrameSelected = tf;
//  if(m_textFrameSelected)
//    m_textFrameSelected->showSelectionPoints();
//}
//
//te::qt::widgets::TextFrame* te::qt::widgets::LayoutEditor::getTextFrameSelected()
//{
//  return m_textFrameSelected;
//}

QWidget* te::qt::widgets::LayoutEditor::getAuxWidget()
{
  return m_auxWidget;
}

QPixmap* te::qt::widgets::LayoutEditor::getDraftPixmap()
{
  return m_draftLayoutEditor->getDraftPixmap();
}

void te::qt::widgets::LayoutEditor::raiseDraftLayoutEditor()
{
  m_draftLayoutEditor->raise();
  m_draftLayoutEditor->getDraftPixmap()->fill(Qt::transparent);
}

void te::qt::widgets::LayoutEditor::lowerDraftLayoutEditor()
{
  m_draftLayoutEditor->lower();
}

void te::qt::widgets::LayoutEditor::drawLayersSelection()
{
  std::vector<te::qt::widgets::LayoutObject*>::iterator it;
  for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
  {
    if((*it)->windowTitle() == "DataFrame")
    {
      te::qt::widgets::DataFrame* df = (te::qt::widgets::DataFrame*)(*it);
      QPixmap* pixmap = df->getLastDisplayContent();
      QPixmap* content = df->getMapDisplay()->getDisplayPixmap();
      content->fill(Qt::transparent);

      QPainter painter(content);
      painter.drawPixmap(0, 0, *pixmap);
      painter.end();

      if(df->getFrameRect().intersects(m_paperViewRect))
        df->drawLayerSelection();
    }
  }
}

void te::qt::widgets::LayoutEditor::setSelectionColor(QColor selColor)
{
  std::vector<te::qt::widgets::LayoutObject*>::iterator it;
  for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
  {
    if((*it)->windowTitle() == "DataFrame")
    {
      te::qt::widgets::DataFrame* df = (te::qt::widgets::DataFrame*)*it;
      df->setSelectionColor(selColor);
    }
  }
}

void te::qt::widgets::LayoutEditor::setDisplayBackgroundColor(QColor cor)
{
  std::vector<te::qt::widgets::LayoutObject*>::iterator it;
  for(it = m_layoutObjects.begin(); it != m_layoutObjects.end(); ++it)
  {
    if((*it)->windowTitle() == "DataFrame")
    {
      te::qt::widgets::DataFrame* df = (te::qt::widgets::DataFrame*)*it;
      df->getMapDisplay()->setBackgroundColor(cor);
    }
  }
}