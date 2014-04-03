/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A Free and Open Source GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*!
  \file terralib/qt/widgets/canvas/FrameData.h

  \brief The main class of FrameData.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TEXT_FRAME_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TEXT_FRAME_H

// TerraLib
#include "../Config.h"
#include "../canvas/MultiThreadMapDisplay.h"
#include "Frame.h"

// Qt
#include <QRect>
#include <QRectF>
#include <QPixmap>
#include <QWidget>

class QMenu;
class QAction;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class LayoutEditor;
      /*!
        \class TextFrame

        \brief A TextFrame ....

      */
      class TEQTWIDGETSEXPORT TextFrame : public Frame
      {
        public:
          TextFrame(const QPointF& p, const QString& s, const int& pointSize, te::qt::widgets::LayoutEditor* me, Qt::WindowFlags f = Qt::Widget);

          TextFrame(const TextFrame& rhs);

          virtual ~TextFrame();

          TextFrame& operator=(const TextFrame& rhs);

          // apos setar qualquer propriedade chame o metodo adjustSize() para desenhar o texto
          QString getType();
          void setText(const QString& t);
          QString getText();
          QString getFamily();
          void setFamily(QString);
          int getPointSize();
          void setPointSize(int ps);
          int getStretch();
          void setStretch(int s);
          int getAlign();
          void setAlign(int);
          QFont::Style getStyle();
          void setStyle(QFont::Style);
          QFont::Weight getWeight();
          void setWeight(QFont::Weight);
          QFont::Capitalization getCapitalization();
          void setCapitalization(QFont::Capitalization);
          bool getOverline();
          void setOverline(bool);
          bool getStrikeOut();
          void setStrikeOut(bool);
          bool getUnderline();
          void setUnderline(bool);
          double getLetterSpacing();
          void setLetterSpacing(double);
          double getWordSpacing();
          void setWordSpacing(double);

          void calculateTextRect(QRectF&);
          void adjust();
          void adjustSize(QPointF& p);
          void draw();
          QRectF getTextRect();
          QPixmap* getPixmap();
          void rubberBand();
          void paintEvent(QPaintEvent* event);
          void getSelectionPoint(const QPoint& p);
          QPoint getCenterSelected();
          void verifyConstraints();
          void showSelectionPoints();
          void hideSelectionPoints();
          QMatrix getAuxMatrix();
          double getAngle(QPoint&);
          void adjustAngleRange(double&);
          void toolTip(const QPoint&, const QString&);
          QPoint getGlobalPositionCenter();
          void setCursor();
          void createMenu();
          void mousePressEvent(QMouseEvent*);
          void mouseMoveEvent(QMouseEvent*);
          void mouseReleaseEvent(QMouseEvent*);
          void enterEvent(QEvent*);
          void leaveEvent(QEvent*);

        public slots:
          void changeFontFamily();
          void changeTextColor();
          void changeText();

        private:
          QColor m_cor;
          QPointF m_point; // text point (bottom left) em milimetros do papel
          double m_angle; // degree
          QString m_family;
          int m_stretch; // default = 100 (normalmente usado entre 50 e 200)
          int m_align; // Qt::AlignLeft, Qt::AlignHCenter, Qt::AlignRight
          QFont::Style m_style; // QFont::StyleNormal, QFont::StyleItalic, QFont::StyleOblique
          bool m_overline;
          bool m_strikeOut;
          bool m_underline;
          double m_letterSpacing; // A value of 100 will keep the spacing unchanged; a value of 200 will enlarge the spacing after a character by the width of the character itself
          double m_wordSpacing; // A positive value increases the word spacing by a corresponding amount of pixels, while a negative value decreases the inter-word spacing accordingly.
          QFont::Capitalization m_capitalization; // default = QFont::MixedCase, QFont::AllUppercase,QFont::AllLowercase,QFont::SmallCaps, QFont::Capitalize
          QFont::Weight m_weight; // QFont::Light, QFont::Normal, QFont::DemiBold, QFont::Bold, QFont::Black
          QString m_text;

          bool m_undo;
          QFont* m_font;
          int m_pointSize;
          double m_auxPointSize;
          double m_auxHeight;
          double m_auxWidth;
          double m_auxStretch;
          double m_auxAngle;
          double m_iniAngle;
          double m_refAngle;
          QPixmap m_pixmap;
          QMenu *m_menu;
          QAction *m_setTextAction;
          QAction *m_setFamilyAction;
          QAction *m_setColorAction;
          QAction *m_showTooTipAction;
          QAction *m_hideTooTipAction;
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TEXT_FRAME_H
