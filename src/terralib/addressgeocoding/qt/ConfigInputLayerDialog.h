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
  \file terralib/addressgeocoding/qt/ConfigInputLayerDialog.h

  \brief A dialog configure the input layer to address geocoding operation
*/

#ifndef __TERRALIB_ADDRESSGEOCODING_INTERNAL_CONFIGINPUTLAYERDIALOG_H
#define __TERRALIB_ADDRESSGEOCODING_INTERNAL_CONFIGINPUTLAYERDIALOG_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../datatype/Property.h"
#include "../../qt/widgets/utils/DoubleListWidget.h"
#include "../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <map>
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class ConfigInputLayerDialogForm; }

// Forward declarations
class QModelIndex;

namespace te
{
  namespace addressgeocoding
  {
    class TEADDRESSGEOCODINGEXPORT ConfigInputLayerDialog : public QDialog
    {
      Q_OBJECT

      public:

        ConfigInputLayerDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~ConfigInputLayerDialog();

        /*!
          \brief Set the layer that can be used

          \param layers   List of AbstractLayerPtr
        */
        void setLayers(std::list<te::map::AbstractLayerPtr> layers);

        te::map::AbstractLayerPtr getLayer();

        te::da::DataSourcePtr getDataSource();

      protected slots:

        void onInputLayerComboBoxChanged(int index);

        void onNumberPushButtonClicked();

        void onHelpPushButtonClicked();

        void onOkPushButtonClicked();

        void onCancelPushButtonClicked();

      private:

        std::auto_ptr<Ui::ConfigInputLayerDialogForm> m_ui;
        std::auto_ptr<te::qt::widgets::DoubleListWidget> m_widget;
        std::list<te::map::AbstractLayerPtr> m_layers;  //!< List of layers.
        te::da::DataSourcePtr m_dataSource;
        te::map::AbstractLayerPtr m_selectedLayer;  //!< Layer used for address geocoding
        std::vector<std::string> m_selectedProps; //!< Selected properties related to the selected Layer

    };
  }   // end namespace addressgeocoding
}     // end namespace te

#endif  // __TERRALIB_ADDRESSGEOCODING_INTERNAL_CONFIGINPUTLAYERDIALOG_H
