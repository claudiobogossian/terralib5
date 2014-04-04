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
  \file terralib/qt/widgets/se/PolygonSymbolizerWidget.h

  \brief A widget used to build a polygon symbolizer element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_POLYGONSYMBOLIZERWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_POLYGONSYMBOLIZERWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

// STL
#include <memory>

// Forward declaraion
class QStackedWidget;
namespace Ui { class PolygonSymbolizerWidgetForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class PolygonSymbolizer;
    class Symbolizer;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class BasicStrokeWidget;

      /*!
        \class PolygonSymbolizerWidget

        \brief A widget used to build a polygon symbolizer element.
      */
      class TEQTWIDGETSEXPORT PolygonSymbolizerWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a polygon symbolizer widget which is a child of parent, with widget flags set to f. */
          PolygonSymbolizerWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~PolygonSymbolizerWidget();

          //@}

        public:

          /*!
            \brief Sets a polygon symbolizer element to this widget.

            \param symb A valid polygon symbolizer element.

            \note The widget will NOT take the ownership of the given symbolizer.
            \note The widget form will be update based on given symbolizer parameters.
          */
          void setSymbolizer(const te::se::PolygonSymbolizer* symb);

          /*!
            \brief Gets the configured polygon symbolizer element.

            \return The configured polygon symbolizer element.
            
            \note The caller will take the ownership of the returned symbolizer.
          */
          te::se::Symbolizer* getSymbolizer() const;

        protected:

          /*! \brief Updates the widget form based on internal polygon symbolizer element. */
          void updateUi();

        protected slots:

          void onPolygonSymbolizerTypeComboBoxCurrentIndexChanged(int index);

          void onStrokeChanged();

          void onStrokeGroupBoxToggled(bool on);

          void onFillChanged();

          void onFillGroupBoxToggled(bool on);

        signals:

          /*! This signal is emitted when the internal polygon symbolizer element is changed. */
          void symbolizerChanged();

        private:

          std::auto_ptr<Ui::PolygonSymbolizerWidgetForm> m_ui; //!< Widget form.
          QStackedWidget* m_fillWidgets;                       //!< Set of Fill Widgets that can be used to configure the polygon symbolizer element. TODO: need review!
          te::qt::widgets::BasicStrokeWidget* m_strokeWidget;  //!< Basic Stroke Widget used to configure the polygon symbolizer stroke element.
          te::se::PolygonSymbolizer* m_symb;                   //!< Polygon symbolizer element that will be configured by this widget.
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_POLYGONSYMBOLIZERWIDGET_H
