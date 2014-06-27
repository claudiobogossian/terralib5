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
  \file terralib/qt/widgets/charts/ChartLayerDialog.h

  \brief A dialog used to build a chart.
*/

#ifndef __TERRALIB_QT_WIDGETS_CHARTS_INTERNAL_CHARTLAYERDIALOG_H
#define __TERRALIB_QT_WIDGETS_CHARTS_INTERNAL_CHARTLAYERDIALOG_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// Qt
#include <QDialog>

// STL
#include <memory>

// Forward declaraion
namespace Ui { class ChartLayerDialogForm; }

namespace te
{
// Forward declarations
  namespace map { class Chart; }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class ChartLayerWidget;

      /*!
        \class ChartLayerDialog

        \brief A dialog used to build a chart.
              
      */
      class TEQTWIDGETSEXPORT ChartLayerDialog : public QDialog
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a basic fill dialog which is a child of parent, with widget flags set to f. */
          ChartLayerDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~ChartLayerDialog();

          //@}

        public:

          /*! \brief Set a layer. */
          void setLayer(te::map::AbstractLayerPtr layer);

          /*! 
            \brief Update the interface with the chart properties
          */
          void setChart(te::map::Chart* chart);

        public slots:

          /*! \brief Function used when the user clicked over the ok push button. */
          void onPushButtonClicked();

        private:

          std::auto_ptr<Ui::ChartLayerDialogForm> m_ui;        //!< Dialog form.
          te::qt::widgets::ChartLayerWidget* m_chartWidget;    //!< ChartLayer Widget used to configure the grouping operation.
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CHARTS_INTERNAL_CHARTLAYERDIALOG_H
