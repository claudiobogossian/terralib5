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
  \file terralib/qt/widgets/rp/FilterDialog.h

  \brief This file has the FilterDialog class.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_FILTERDIALOG_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_FILTERDIALOG_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

//Boost
#include <boost/numeric/ublas/matrix.hpp>

// Qt
#include <QtGui/QDialog>

namespace Ui { class FilterDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      //forward declaration
      class LayerSelectorWidget;
      class OutputRasterParametersWidget;
      class RasterNavigatorWidget;

      class TEQTWIDGETSEXPORT FilterDialog : public QDialog
      {
        Q_OBJECT

        public:

          FilterDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~FilterDialog();

        public:

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          te::map::AbstractLayerPtr getOutputLayer();

        public slots:

          void onFilterTypeComboBoxActivated(int index);

          void onMaskToolButtonClicked();

          void onLoadMaskToolButtonClicked();

          void onLayerSelected(te::map::AbstractLayerPtr l);

          void apply();

          void onOkPushButtonClicked();

        protected:

          void listBands();

          void listFilterTypes();

        private:

          std::auto_ptr<Ui::FilterDialogForm> m_ui;
          std::auto_ptr<LayerSelectorWidget> m_layerSelector;
          std::auto_ptr<OutputRasterParametersWidget> m_outputRaster;
          std::auto_ptr<RasterNavigatorWidget> m_navigator;

          te::map::AbstractLayerPtr m_outputLayer;
          te::map::AbstractLayerPtr m_layer;

          boost::numeric::ublas::matrix<double> m_window; //!< User defined convolution window.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_FILTERDIALOG_H
