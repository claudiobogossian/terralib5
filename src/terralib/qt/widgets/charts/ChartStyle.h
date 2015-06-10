/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/charts/ChartStyle.h

  \brief A class used to define a chartDisplay's style
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLE_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLE_H

//TerraLib
#include "../Config.h"

//QT
#include <qcolor.h>
#include <QFont>

//STL
#include <string.h>

namespace te
{
  namespace se 
  {
    //forward declarations
    class Fill;
    class Font;
    class Stroke;
  }

  namespace qt
  {

    namespace widgets
    {
      class TEQTWIDGETSEXPORT ChartStyle
      {

        public:

          /*!
            \brief Constructor
          */
          ChartStyle();

          /*!
            \brief Constructor

            \param title The style's title
            \param titleFont The title's font
            \param axisX The x axis' label
            \param axisY The y axis' label
            \param axisFont The axis' font. Will be used for both x and y axis.
            \param fill The title's fill
            \param stroke The title's stroke
            \param gridChecked Boolean used to decided weather to display the grid or not
            \param color The style's background ccolor
          */
          ChartStyle(QString title, QFont titleFont, QString axisX, QString axisY, QFont axisFont, te::se::Fill* fill, te::se::Stroke* stroke, bool gridChecked,  QColor color);

          /*! 
            \brief Destructor.
          */
          ~ChartStyle();

          /*! 
            \brief Returns a reference to the style's Title

            \return A QString that represents the style's Title.
          */
          QString& getTitle();

          /*!
            \brief It sets the style's title

            \param newTitle The new title.
          */
          void setTitle(QString newTitle);

          /*! 
            \brief Returns a reference to the title's font.

            \return A QFont that represents the title's font.
          */
          QFont& getTitleFont();

          /*!
            \brief It sets the title's font.

            \param newTitle The new title's font..
          */
          void setTitleFont(QFont newTitleFont);

          /*! 
            \brief Returns a reference to the style's x axis label

            \return A QString that represents the style's x axis label.
          */
          QString& getAxisX();

          /*!
            \brief It sets the style's x axis label

            \param newAxisX The new x axis label.
          */
          void setAxisX(QString newAxisX);

          /*! 
            \brief Returns a reference to the style's y axis label

            \return A QString that represents the style's y axis label.
          */
          QString& getAxisY();

          /*!
            \brief It sets the style's y axis label

            \param newAxisX The new y axis label.
          */
          void setAxisY(QString newAxisY);

          /*! 
            \brief Returns a reference to the axis' font.

            \return A QFont that represents the axis' font.
          */
          QFont& getAxisFont();

          /*!
            \brief It sets the axis' font.

            \param newAxisX The new axis' font.
          */
          void setAxisFont(QFont newAxisFont);

          /*!            
            \brief Returns a pointer to the style's fill

            \return A Fill type pointer to the style's fill.
            \note The caller will not take ownership of the returned pointer. 
          */
          te::se::Fill* getFill();

          /*!            
            \brief It sets the style's fill

            \param newFill The new fill.
            \note It will take ownership of the given pointer
          */
          void setFill(te::se::Fill* newFill);

          /*!
            \brief Returns a pointer to the style's stroke

            \return A Fill type pointer to the style's stroke.
            \note The caller will not take ownership of the returned pointer. 
          */
          te::se::Stroke* getStroke();

          /*!            
            \brief It sets the style's stroke

            \param newStroke The new stroke.
            \note It will take ownership of the given pointer
          */
          void setStroke(te::se::Stroke* newStroke);

          /*! 
            \brief Returns a reference to the style's color

            \return A QColor that represents the style's color.
          */
          QColor& getColor();

          /*!
            \brief It sets the style's color

            \param newColor The new color.
          */
          void setColor(QColor newColor);

          /*! 
            \brief Returns true if the grid is to be displayed, otherwise returns false

            \return A boolean used to decided weather to display the grid or not
          */
          bool getGridChecked();

          /*! 
            \brief It sets the boolean used to decided weather to display the grid or not

            \param newGridChecked A boolean used to decided weather to display the grid or not
          */
          void setGridChecked(bool newGridChecked);

        private:

          QString          m_title;         //!< The chart's title.
          QFont            m_titleFont;     //!< The title's font.
          QString          m_axisX;         //!< The chart X axis' label.
          QString          m_axisY;         //!< The chart Y axis' label.
          QFont            m_axisFont;       //!< The axis' font.
          bool             m_gridChecked;   //!< The boolean used to decide if the chart will have a background grid .
          te::se::Fill*    m_fill;          //!< The chart's fill.
          te::se::Stroke*  m_stroke;        //!< The chart's stroke.
          QColor           m_backColor;     //!< The chart's background color.

      };
    }  //end namespace widgets
  }    //end namespace qt
}      // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLE_H