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

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_GRAPHIC_SCALE_FRAME_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_GRAPHIC_SCALE_FRAME_H

// TerraLib
#include "../Config.h"
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
      class DataFrame;
      /*!
        \class ScaleFrame

        \brief A ScaleFrame ....

      */
      class TEQTWIDGETSEXPORT GraphicScaleFrame : public Frame
      {
        public:
          GraphicScaleFrame(te::qt::widgets::DataFrame* df, Qt::WindowFlags f = Qt::Widget);

          GraphicScaleFrame(const GraphicScaleFrame& rhs);

          virtual ~GraphicScaleFrame();

          GraphicScaleFrame& operator=(const GraphicScaleFrame& rhs);

          void paintEvent(QPaintEvent* event);

          void setDataFrame(te::qt::widgets::DataFrame* df);

          te::qt::widgets::DataFrame* getDataFrame();

          QPixmap* getPixmap();

          void draw();

          void adjust();

          void calculateSelectionPoints();

          QFont getFont();
          void setFont(const QFont&);
          void setPointSize(int);
          void getLabels();
          void labelLengthCalculation();
          void drawLabels();
          void drawBar1();
          void drawBar2();
          void drawBar3();
          void drawBar4();
          void mousePressEvent(QMouseEvent*);
          void mouseMoveEvent(QMouseEvent*);
          void mouseReleaseEvent(QMouseEvent*);
          void enterEvent(QEvent*);
          void leaveEvent(QEvent*);
          QColor getLabelColor();
          void setLabelColor(const QColor&);
          QColor getBarColor();
          void setBarColor(const QColor&);
          QRectF getBarRect();
          void setBarRect(const QRectF&);
          void setType(int t);
          int getType();
          bool isLabelAbove();
          void setLabelAbove(bool);
          bool isUnitAbove();
          void setUnitAbove(bool);
          double getSubTickSize();
          void setSubTickSize(double);
          bool isTickCentralized();
          void setTickCentralized(bool);
          double getStep();
          void findNiceStep(double newStep = 0);
          void setStep(double);
          int getDivisions();
          void setDivisions(int);
          int getSubDivisions();
          void setSubDivisions(int);
          double getSpace();
          void setSpace(double);
          QString getUnit();
          void setUnit(const QString&);
          void rubberBand();
          void showSelectionPoints();
          void hideSelectionPoints();
          void getSelectionPoint(const QPoint& p);
          QPoint getCenterSelected();
          void setCursor();
          void toolTip(const QPoint&, const QString&);
          void createMenu();
          void setTypeAction();
          void setFamilyAction();
          void setLabelColorAction();
          void setBarColorAction();
          void setBarHeightAction();
          void setOffsetOriginAction();
          void setLabelAboveAction();
          void setUnitAboveAction();
          void setUnitAction();
          void setDivisionsAction();
          void setSubDivisionsAction();
          void setStepAction();
          void setShowTooTipAction();


      private:
        te::qt::widgets::DataFrame* m_dataFrame; // pointer to data frame
        int m_type; // 1 = line, 2 = steped, 3 = hollow bar, 4 = alternating bar
        QFont m_font; // font for label and unit
        int m_pointSize; // point size for label and unit
        QColor m_labelColor; // label and uinit color
        QColor m_barColor; // bar color
        double m_step; // step
        int m_divisions; // number of divisions
        int m_subDivisions; // number of subdivisions. Used for first division
        bool m_labelAbove; // true = label above the bar
        double m_subTickSize; // subdivision tick size (mm). Used for only type = 1
        bool m_tickCentralized; // true = centralized ticks, false = follow the label position. Used for only type = 1
        double m_space; // space (mm) for labels and unit
        QString m_unit;  // Meters, Kilometers, Miles, Leagues, Inches, Feet, Hand, Fingers, Nautic Miles, Nautic Fathom, Decimal Degrees, Yards, Fourlongs, Chains, Marathons, Unknow
        bool m_unitAbove; // true = unit above the bar
        bool m_offsetOrigin; // true = offset origin

        bool m_undo;
        QPixmap m_pixmap; // widget pixmap
        std::vector<QString> m_labels; // numeric labels
        QString m_subDivisionString; // subDivision string
        std::vector<double> m_labelLengths; // length (mm)
        double m_labelHeight; // label height
        QRectF m_barRect; // bar rect (mm). If type = 1: use this width for tick size
        int m_precision; // label decimal precision
        double m_subDivisionLabelLength; // subdivision label length
        double m_toKm; // projection data unit to kilometer
        double m_kmToUnit; // convert km to Unit
        double m_stepmm; // step in milimeters

        QMenu *m_menu;
        QAction *m_setTypeAction;
        QAction *m_setFamilyAction;
        QAction *m_setLabelColorAction;
        QAction *m_setBarColorAction;
        QAction *m_setBarHeightAction;
        QAction *m_offsetOriginAction;
        QAction *m_setLabelAboveAction;
        QAction *m_setUnitAboveAction;
        QAction *m_setUnitAction;
        QAction *m_setDivisionsAction;
        QAction *m_setSubDivisionsAction;
        QAction *m_setStepAction;
        QAction *m_showTooTipAction;
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_GRAPHIC_SCALE_FRAME_H
