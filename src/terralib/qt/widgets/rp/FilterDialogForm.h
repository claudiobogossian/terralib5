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
  \file terralib/qt/widgets/rp/FilterDialogForm.h

  \brief This file has the FilterDialogForm class.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_FILTERDIALOGFORM_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_FILTERDIALOGFORM_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../../maptools/AbstractLayer.h"
#include "../../../rp/Filter.h"
#endif
#include "../Config.h"
#include "InputLayerWidget.h"
#include "OutputRasterWidget.h"

// STL
#include <memory>

//Boost
#include <boost/numeric/ublas/matrix.hpp>

// Qt
#include <QDialog>

namespace Ui { class FilterDialogForm; }

namespace te
{
  namespace qt
  {
    namespace af
    {
      namespace evt
      {
        struct Event;
      }
    }

    namespace widgets
    {
      //forward declaration
      class Canvas;
      class MapDisplay;

      class TEQTWIDGETSEXPORT FilterDialogForm : public QDialog
      {
        Q_OBJECT

        public:

          FilterDialogForm(QWidget* parent = 0);

          ~FilterDialogForm();

          bool isComplete() const;

        public:

          /*!
            \brief This method is used to set the selected layer for filter operation
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void setParams();

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          void setMapDisplay(te::qt::widgets::MapDisplay* mapDisplay);

          te::rp::Filter::InputParameters getInputParams();

        public slots:

          void onFilterTypeComboBoxActivated(int index);

          void onMaskToolButtonClicked();

          void onLoadMaskToolButtonClicked();

          void onNewROIPushButtonClicked();

          void onPreviewCheckBoxToggled(bool isChecked);

          void onOkPushButtonClicked();

          void apply();

          void layerChanged();

          void onEnvelopeAcquired(te::gm::Envelope env);

        signals:

          void geomAquired(te::gm::Polygon* poly);

          void addLayer(te::map::AbstractLayerPtr layer);

          void closeTool();

        protected:

          bool execute();

          void listBands();

          void listFilterTypes();

          void getRasterFromROI();

          void applyPreview();

          void drawPreview(te::rst::Raster* raster);

          void closeEvent(QCloseEvent* e);

        private:

          std::auto_ptr<Ui::FilterDialogForm> m_ui;
          //std::auto_ptr<RasterNavigatorWidget> m_navigator;

          te::map::AbstractLayerPtr m_layer;
          te::map::AbstractLayerPtr m_outputLayer;
          te::rst::Raster* m_rasterPreview;
          te::gm::Geometry* m_geom;
          te::qt::widgets::MapDisplay* m_mapDisplay;
          te::qt::widgets::Canvas* m_canvas;
          te::qt::widgets::InputLayerWidget* m_inputWidget;
          te::qt::widgets::OutputRasterWidget* m_outputWidget;

          boost::numeric::ublas::matrix<double> m_window; //!< User defined convolution window.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_FILTERDIALOGFORM_H
