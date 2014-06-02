/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/AbstractFillWidget.h

  \brief Abstract class that represents a widget that can be used to build a fill element.
*/

#ifndef __TERRALIB_QT_SE_INTERNAL_ABSTRACTFILLWIDGET_H
#define __TERRALIB_QT_SE_INTERNAL_ABSTRACTFILLWIDGET_H

// Qt
#include <QWidget>

namespace te
{
// Forward declarations
  namespace se
  {
    class Fill;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class AbstractFillWidget

        \brief Abstract class that represents a widget that can be used to build a fill element.

        \sa BasicFillWidget, GraphicFillWidget
      */
      class AbstractFillWidget : public QWidget
      {
        public:

          /** @name Initializer Methods
            *  Methods related to instantiation and destruction.
            */
          //@{

          /*! \brief Constructs a widget which is a child of parent, with widget flags set to f. */
          AbstractFillWidget(QWidget* parent = 0, Qt::WindowFlags f = 0) : QWidget(parent, f) { }

          /*! \brief Virtual destructor. */
          virtual ~AbstractFillWidget() { }

          //@}

          /*!
            \brief Sets a fill element to this widget.

            \param fill A valid fill element.

            \return It return true if the widget can deal with the given fill. Otherwise, it returns false.

            \note The widget will NOT take the ownership of the given fill.
            \note The widget form will be update based on given fill parameters.
          */
          virtual bool setFill(const te::se::Fill* fill) = 0;

          /*!
            \brief Gets the configured fill element.

            \return The configured fill element.
            
            \note The caller will take the ownership of the returned fill.
          */
          virtual te::se::Fill* getFill() const = 0;

          /*! \brief Pure virtual method that should return a "user friendly" string that informs the fill type that can be built by the widget. */
          virtual QString getFillType() const = 0;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_SE_INTERNAL_ABSTRACTFILLWIDGET_H
