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

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_GRID_FRAME_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_GRID_FRAME_H

// TerraLib
#include "../Config.h"
#include "Frame.h"

// Qt
#include <QtGui/QPixmap>
#include <QtGui/QFont>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DataFrame;
      /*!
        \class GridFrame

        \brief A GridFrame ....

      */
      class TEQTWIDGETSEXPORT GridFrame : public Frame
      {
        public:
          GridFrame(te::qt::widgets::DataFrame* df, Qt::WindowFlags f = Qt::Window);
          GridFrame(const GridFrame& rhs);
          ~GridFrame();

          GridFrame& operator=(const GridFrame& rhs);

          virtual void adjust();

          void copyToDraftMapDisplay();

          void setDataFrame(te::qt::widgets::DataFrame* df);

          QFont& getFont();

          unsigned int getVerticalLabelSpace();
          void setVerticalLabelSpace(unsigned int);

          unsigned int getHorizontalLabelSpace();
          void setHorizontalLabelSpace(unsigned int);

          int getVerticalLabelAngle();
          void setVerticalLabelAngle(int);

          int getHorizontalLabelAngle();
          void setHorizontalLabelAngle(int);

          double getInitialCoordX();
          void setInitialCoordX(double);

          double getDeltaX();
          void setDeltaX(double);

          double getInitialCoordY();
          void setInitialCoordY(double);

          double getDeltaY();
          void setDeltaY(double);

          unsigned int getHSpace();
          void setHSpace(unsigned int);

          unsigned int getVSpace();
          void setVSpace(unsigned int);

        protected:
          te::qt::widgets::DataFrame* m_dataFrame;
          QPixmap m_pixmap;
          QFont m_font;
          unsigned int m_verticalLabelSpace; // em mm
          unsigned int m_horizontalLabelSpace; // em mm
          int m_verticalLabelAngle; // em graus
          int m_horizontalLabelAngle; // em graus
          double m_initialCoordX;
          double m_deltaX;
          double m_initialCoordY;
          double m_deltaY;
          unsigned int m_hSpace; // espaco horizontal (mm) em torno do data frame
          unsigned int m_vSpace; // espaco vertical (mm) em torno do data frame
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_GRID_FRAME_H
