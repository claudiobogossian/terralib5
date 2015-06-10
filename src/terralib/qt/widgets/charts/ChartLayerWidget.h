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
  \file terralib/qt/widgets/charts/ChartLayerWidget.h

  \brief A widget used to build a chart.
*/

#ifndef __TERRALIB_QT_WIDGETS_CHARTS_INTERNAL_CHARTLAYERWIDGET_H
#define __TERRALIB_QT_WIDGETS_CHARTS_INTERNAL_CHARTLAYERWIDGET_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <map>
#include <memory>

// QT
#include <QWidget>

// Forward declaraion
namespace Ui { class ChartLayerWidgetForm; }

namespace te
{
// Forward declarations
  namespace map { class Chart; }

  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class ColorPickerToolButton;

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

          /*! \brief Set a layer. */
          void setLayer(te::map::AbstractLayerPtr layer);

          /*! 
            \brief Creates the te::map::Chart object using the interface parameters

            \return true if the object was build and false in other case.

            \note This method set in layer the chart object.
          
          */
          bool buildChart();

          /*! 
            \brief Update the interface with the chart properties
          */
          void setChart(te::map::Chart* chart);

        protected:

          /*! \brief Internal method to initialize the widget (e.g.: color, combos, icons, etc.) */
          void initialize();

          /*! \brief List the layer attributes (from layer schema) */
          void listAttributes();


        public slots:

          /*! \brief Updates the widget form based on internal fill element. */
          void updateUi();

          /*! 
            \brief Function used when the user clicked over the add tool button. 

            \note This function adds a new attribute to be used in Chart object.
          */
          void onAddToolButtonClicked();

          /*! 
            \brief Function used when the user clicked over the remove tool button. 

            \note This function removes a selected attribute used in Chart object.
          */
          void onRemoveToolButtonClicked();

          /*! 
            \brief Function used when the user clicked over the table (color column). 

            \note This function gives a new color to a selected attribute.
          */
          void onItemClicked(int row, int column);

          double getMaxValue(te::map::Chart* chart);

        private:

          std::auto_ptr<Ui::ChartLayerWidgetForm> m_ui;             //!< Widget form.
          ColorPickerToolButton* m_colorPicker;                     //!< The color picker used to customise the color of several chart parameters.
          te::map::AbstractLayerPtr m_layer;                        //!< TerraLib layer auto ptr
          std::vector<std::pair<std::string, QColor> > m_chartVec;  //!< Chart Vec
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_CHARTLAYERWIDGET_H
