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
\file terralib/mnt/qt/ImageGenerationDialog.h

\brief A dialog Shadow image generation
*/

#ifndef __TERRALIB_MNT_INTERNAL_IMAGEGENERATIONDIALOG_H
#define __TERRALIB_MNT_INTERNAL_IMAGEGENERATIONDIALOG_H

// Terralib
#ifndef Q_MOC_RUN
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#endif
#include "../../qt/widgets/canvas/MapDisplay.h"
#include "../core/Config.h"

// STL
#include <list>
#include <map>

// Qt
#include <QDialog>

namespace Ui { class ImageGenerationDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class RasterNavigatorWidget;
    }
  }

  namespace mnt
  {
    class TEMNTEXPORT ImageGenerationDialog : public QDialog
    {
      Q_OBJECT

    public:
      ImageGenerationDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

      ~ImageGenerationDialog();

      /*!
      \brief Set the layer that can be used
      \param layers   List of AbstractLayerPtr
      */
      void setLayers(std::list<te::map::AbstractLayerPtr> layers);

      te::map::AbstractLayerPtr getLayer();

      void setSRID(int newSRID);

      void setMapDisplay(te::qt::widgets::MapDisplay* mapDisplay);
      void getRaster();

      void drawPreview(std::unique_ptr<te::rst::Raster>& raster);

      void resetDraw();

      protected slots:

      void onInputComboBoxChanged(int index);
      void onlayerSearchToolButtonClicked();
      void onPreviewPushButtonClicked();

      void onResXLineEditEditingFinished();
      void onResYLineEditEditingFinished();
      void onTargetFileToolButtonPressed();
      void onHelpPushButtonClicked();
      void onOkPushButtonClicked();
      void onCancelPushButtonClicked();
      void onSrsToolButtonClicked();

    private:

      std::unique_ptr<Ui::ImageGenerationDialogForm> m_ui;

      te::da::DataSourceInfoPtr m_outputDatasource;                                     //!< DataSource information.
      std::string m_outputArchive;                                                      //!< Archive information.
      std::list<te::map::AbstractLayerPtr> m_layers;                                    //!< List of layers.
      te::map::AbstractLayerPtr m_inputLayer;                                        //!< Input layer
      te::qt::widgets::MapDisplay* m_mapDisplay;
      te::qt::widgets::Canvas* m_canvas;
      std::unique_ptr<te::rst::Raster> m_previewRaster;
      te::map::AbstractLayerPtr m_outputLayer;                                          //!< Generated Layer.
      te::da::DataSourcePtr     m_inDataSource;   //!< input DataSourcePtr
      std::string m_inSetName;  //!< input DataSetLayer name

      double m_min;
      double m_max;
      double m_dummy;
      int m_outsrid;
    }; //class ImageGenerationDialog
  } //mnt
} //te

#endif
