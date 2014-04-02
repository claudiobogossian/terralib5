/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/utils/ColorPickerToolButton.h

  \brief Custom widget used to pick a color.
*/

#ifndef __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_COLORPICKERTOOLBUTTON_H
#define __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_COLORPICKERTOOLBUTTON_H

// TerrLib
#include "../Config.h"

// Qt
#include <QApplication>
#include <QColor>
#include <QColorDialog>
#include <QFrame>
#include <QGridLayout>
#include <QHash>
#include <QMouseEvent>
#include <QPainter>
#include <QRectF>
#include <QToolButton>

#define COLORSIZE 20
#define COLS 8
#define SPACE 4
#define MARGINY 19
#define MARGINX 4

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ColorKeyInfo

        \brief Associate a rect to a color.
      */
      class ColorKeyInfo
      {
        public:

          ColorKeyInfo(const QColor& color, const QRectF& rect)
          {
            m_color = color;
            m_rect = rect;
          }

          QColor m_color;
          QRectF m_rect;
      };

      /*!
        \class ColorPickerPopup

        \brief Create a frame with a default color palette.
      */
      class ColorPickerPopup : public QFrame
      {
        Q_OBJECT

        public:

          ColorPickerPopup(QWidget* parent = 0) : QFrame(parent)
          {
            setWindowFlags(Qt::Popup);
            setFrameShape(QFrame::StyledPanel);
            setFrameShadow(QFrame::Plain);
            setStyleSheet("QFrame{background-color: rgb(255, 255, 255);border-color: rgb(0, 0, 0);}");

            // Create default colors
            int i = 0; 
            for(int g = 0; g < 4; ++g)
              for(int r = 0;  r < 4; ++r)
                for(int b = 0; b < 3; ++b)
                  m_stdrgb[i++] = qRgb(r * 255 / 3, g * 255 / 3, b * 255 / 2);

            int width = COLS;
            int height = 48 / COLS;

            width = width * COLORSIZE + SPACE + MARGINX + 4;
            height = height * COLORSIZE + SPACE + MARGINY + 32;
            resize(width, height);

            setMouseTracking(true);
          }

          void paintButton(QPainter* painter, const QRectF& rc, const QString& text, bool hover)
          {
            !hover ? painter->setBrush(Qt::lightGray) : painter->setBrush(Qt::gray);
            painter->drawRect(rc);
            painter->setPen(Qt::black);
            painter->drawText(rc, text, Qt::AlignHCenter | Qt::AlignVCenter);
          }

          void paintEvent(QPaintEvent* /*e*/)
          {
            QPainter painter(this);

            painter.drawRect(0, 0, width() - 1, height() - 1);
            painter.drawText(QRect(10, 1, this->width(), 19), tr("Basic Colors"), Qt::AlignLeft | Qt::AlignVCenter);

            int col, row;
            m_colorInfos.clear();
            painter.setPen(Qt::darkGray);
            for(int i = 0; i < 48; ++i)
            {
              QRectF rc;
              col = i % COLS;
              row = (int)(i / COLS);

              rc.setLeft(col * COLORSIZE + SPACE + MARGINX);
              rc.setTop(row * COLORSIZE + SPACE + MARGINY);

              rc.setWidth(COLORSIZE - SPACE);
              rc.setHeight(COLORSIZE - SPACE);

              QColor color(m_stdrgb[i]);
              painter.setBrush(color);
              painter.drawRect(rc);
              m_colorInfos.append(ColorKeyInfo(color, rc));
            }

            QPointF mousePos = mapFromGlobal(QCursor::pos());

            m_buttonKeyRect = QRect(SPACE, (row + 1) * COLORSIZE + SPACE + MARGINY + 5, width() - 2 * SPACE, 20);
            
            paintButton(&painter, m_buttonKeyRect, tr("More Colors..."), m_buttonKeyRect.contains(mousePos));

            for(int i = 0; i < m_colorInfos.count(); ++i)
            {
              if(m_colorInfos[i].m_rect.contains(mousePos))
              {
                m_hoverColor = m_colorInfos[i].m_color;

                QRectF rc = m_colorInfos[i].m_rect;
                rc.setTop(rc.top() - 2);
                rc.setLeft(rc.left() - 2);
                rc.setBottom(rc.bottom() + 2);
                rc.setRight(rc.right() + 2);

                painter.setBrush(m_hoverColor);
                painter.drawRect(rc);

                break;
              }
            }
          }

          void mouseMoveEvent(QMouseEvent* /*e*/)
          {
            repaint();
          }

          void mousePressEvent(QMouseEvent* e)
          {
            if(!rect().contains(e->pos()))
              close();

            for(int i = 0; i < m_colorInfos.count(); ++i)
            {
              if(m_colorInfos[i].m_rect.contains(e->pos()))
              {
                m_hoverColor = m_colorInfos.at(i).m_color;
                emit selected(m_hoverColor);
                close();
              }
            }

#if (QT_VERSION >= 0x050000)
            if(m_buttonKeyRect.contains(e->localPos()))
#else
            if(m_buttonKeyRect.contains(e->posF()))
#endif
            {
              // Show original color dialog
              QColor color = QColorDialog::getColor(Qt::white, this, "", QColorDialog::ShowAlphaChannel);
              if(!color.isValid())
                return;

              m_hoverColor = color;
              emit selected(m_hoverColor);
              close();
            }
          }

        signals:

          void selected(const QColor& color);

        public:

          QColor m_hoverColor;
          QRgb m_stdrgb[48];
          QList<ColorKeyInfo> m_colorInfos;
          QRectF m_buttonKeyRect;
      };

      /*!
        \class ColorPickerToolButton

        \brief Custom tool button used to pick a color.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT ColorPickerToolButton : public QToolButton
      {
        Q_OBJECT

        public:

          ColorPickerToolButton(QWidget* parent = 0);

          ~ColorPickerToolButton();

          void setColor(const QColor& color);

          QColor getColor() const;

        protected:

          void resizeEvent(QResizeEvent* e);

          void mousePressEvent(QMouseEvent* e);

          void updateIcon();

        protected slots:

          void onPopupSelected(const QColor& color);

        signals:

          void colorChanged(const QColor&);

        private:

          ColorPickerPopup* m_popup;
          QColor m_selectedColor;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif // __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_COLORPICKERTOOLBUTTON_H
