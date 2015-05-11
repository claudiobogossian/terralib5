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
  \file terralib/qt/widgets/se/PointSymbolizerWidget.h

  \brief A widget used to build a point symbolizer element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_POINTSYMBOLIZERWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_POINTSYMBOLIZERWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

namespace te
{
// Forward declarations
  namespace se
  {
    class PointSymbolizer;
    class Symbolizer;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class GraphicSelectorWidget;

      /*!
        \class PointSymbolizerWidget

        \brief A widget used to build a point symbolizer element.
      */
      class TEQTWIDGETSEXPORT PointSymbolizerWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a point symbolizer widget which is a child of parent, with widget flags set to f. */
          PointSymbolizerWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~PointSymbolizerWidget();

          //@}

        public:

          /*!
            \brief Sets a point symbolizer element to this widget.

            \param symb A valid point symbolizer element.

            \note The widget will NOT take the ownership of the given symbolizer.
            \note The widget form will be update based on given symbolizer parameters.
          */
          void setSymbolizer(const te::se::PointSymbolizer* symb);

          /*!
            \brief Gets the configured point symbolizer element.

            \return The configured point symbolizer element.
            
            \note The caller will take the ownership of the returned symbolizer.
          */
          te::se::Symbolizer* getSymbolizer() const;

        protected:

          /*! \brief Updates the widget form based on internal point symbolizer element. */
          void updateUi();

        protected slots:

          void onGraphicChanged();

        signals:

          /*! This signal is emitted when the internal point symbolizer element is changed. */
          void symbolizerChanged();

        private:

          GraphicSelectorWidget* m_graphicSelector;  //!< Graphic selector use to select the point symbolizer graphic element.
          te::se::PointSymbolizer* m_symb;           //!< Point symbolizer element that will be configured by this widget.
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_POINTSYMBOLIZERWIDGET_H
