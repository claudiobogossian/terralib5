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
  \file terralib/vp/qt/VectorToRasterDialog.h

  \brief A dialog vector to raster operation
*/

#ifndef __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTORASTERDIALOG_H
#define __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTORASTERDIALOG_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/widgets/utils/DoubleListWidget.h"
#include "../../statistics/core/Enums.h"
#include "../Config.h"

// STL
#include <list>
#include <map>
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class VectorToRasterDialogForm; }

namespace te
{
  namespace attributefill
  {
    class TEATTRIBUTEFILLEXPORT VectorToRasterDialog : public QDialog
    {
      Q_OBJECT

      public:

        VectorToRasterDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~VectorToRasterDialog();

        /*!
          \brief Set the layer that can be used

          \param layers   List of AbstractLayerPtr
        */
        void setLayers(std::list<te::map::AbstractLayerPtr> layers);

        /*!
          \brief Get the generated layer.

          \return the generated layer.
        */
        te::map::AbstractLayerPtr getLayer();

      private:

        void showSRS();

      protected slots:

        void onVectorComboBoxChanged(int index);

        void onResXLineEditEditingFinished();

        void onResYLineEditEditingFinished();

        void onColsLineEditEditingFinished();

        void onRowsLineEditEditingFinished();

        void onTargetFileToolButtonPressed();

        void onHelpPushButtonClicked();

        void onOkPushButtonClicked();

        void onCancelPushButtonClicked();

      private:

        std::auto_ptr<Ui::VectorToRasterDialogForm> m_ui;
        std::list<te::map::AbstractLayerPtr> m_layers;                //!< List of layers.
        int m_srid;                                                   //!< SRID of selected layer.
        std::auto_ptr<te::qt::widgets::DoubleListWidget> m_widget;    //!< Double list widget.
        te::da::DataSourceInfoPtr m_outputDatasource;                 //!< DataSource information.
        std::vector<te::dt::Property*> m_properties;                  //!< Properties related to the selected Layer
        te::map::AbstractLayerPtr m_selectedLayer;                    //!< Selected layer.
        te::map::AbstractLayerPtr m_outLayer;                         //!< Generated Layer.
        std::string m_path;                                           //!< Output layer path;
        bool m_toFile;
    };
  }   // end namespace attributefill
}     // end namespace te

#endif  // __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTORASTERDIALOG_H
