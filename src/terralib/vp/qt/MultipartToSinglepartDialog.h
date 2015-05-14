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
  \file terralib/vp/qt/MultipartToSinglepartDialog.h

  \brief A Multipart to Singlepart operation dialog
*/

#ifndef __TERRALIB_VP_INTERNAL_MULTIPARTTOSINGLEPARTDIALOG_H
#define __TERRALIB_VP_INTERNAL_MULTIPARTTOSINGLEPARTDIALOG_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../../statistics/core/Enums.h"
#include "../Config.h"
#include "../Enums.h"

// STL
#include <list>
#include <map>
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class MultipartToSinglepartDialogForm; }

// Forward declarations
class QListWidgetItem;
class QModelIndex;
class QTreeWidgetItem;

namespace te
{
  namespace vp
  {
// Forward declarations
    class LayerTreeModel;

    class TEVPEXPORT MultipartToSinglepartDialog : public QDialog
    {
      Q_OBJECT

      public:

        MultipartToSinglepartDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~MultipartToSinglepartDialog();

        /*!
          \brief Set the layer that can be used

          \param layers List of AbstractLayerPtr
        */
        void setLayers(std::list<te::map::AbstractLayerPtr> layers);        

        /*!
          \brief Get the generated layer.

          \return the generated layer.
        */
        te::map::AbstractLayerPtr getLayer();

      protected slots:

        void onLayerComboBoxChanged(int index);

        void onTargetDatasourceToolButtonPressed();

        void onTargetFileToolButtonPressed();

        void onHelpPushButtonClicked();

        void onOkPushButtonClicked();

        void onCancelPushButtonClicked();

      private:

        std::auto_ptr<Ui::MultipartToSinglepartDialogForm> m_ui;
        te::da::DataSourceInfoPtr m_outputDatasource;                 //!< DataSource information.
        std::list<te::map::AbstractLayerPtr> m_layers;                //!< List of layers.
        te::map::AbstractLayerPtr m_selectedLayer;                    //!< Layer used for operation
        te::map::AbstractLayerPtr m_layer;                            //!< Generated Layer.
        bool m_toFile;
    };
  }   // end namespace vp
}     // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_MULTIPARTTOSINGLEPARTDIALOG_H
