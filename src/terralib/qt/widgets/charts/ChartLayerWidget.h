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
  \file terralib/qt/widgets/charts/ChartLayerWidget.h

  \brief A widget used to build a chart.
*/

#ifndef __TERRALIB_QT_WIDGETS_CHARTS_INTERNAL_CHARTLAYERWIDGET_H
#define __TERRALIB_QT_WIDGETS_CHARTS_INTERNAL_CHARTLAYERWIDGET_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// QT
#include <QtGui/QWidget>

// Forward declaraion
namespace Ui { class ChartLayerWidgetForm; }

namespace te
{
// Forward declarations

  namespace qt
  {
    namespace widgets
    {
      // Forward declarations

      /*!
        \class ChartLayerWidget

        \brief A widget used to build a grouping.

        \sa 
      */
      class TEQTWIDGETSEXPORT ChartLayerWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a basic fill widget which is a child of parent, with widget flags set to f. */
          ChartLayerWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~ChartLayerWidget();

          //@}

        public:

          void setLayer(te::map::AbstractLayerPtr layer);


        protected:

          /*! \brief Internal method to initialize the widget (e.g.: color, combos, icons, etc.) */
          void initialize();

          /*! \brief Updates the widget form based on internal fill element. */
          void updateUi();

          void listAttributes();


        public slots:

          void onAddToolButtonClicked();

          void onRemoveToolButtonClicked();

        private:

          std::auto_ptr<Ui::ChartLayerWidgetForm> m_ui;             //!< Widget form.

          te::map::AbstractLayerPtr m_layer;                        //!< TerraLib layer auto ptr          
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_CHARTLAYERWIDGET_H
