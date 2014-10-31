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
  \file terralib/attributefill/qt/VectorToVectorDialog.h

  \brief Raster to vector attributefill dialog.
*/

#ifndef __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTORDIALOG_H
#define __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTORDIALOG_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../../statistics/core/Utils.h"
#include "../Config.h"
#include "../Enums.h"

// STL
#include <list>
#include <map>
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class VectorToVectorDialogForm; }

// Forward declarations
class QListWidgetItem;
class QModelIndex;
class QTreeWidgetItem;

namespace te
{
  namespace attributefill
  {
    class TEATTRIBUTEFILLEXPORT VectorToVectorDialog : public QDialog
    {
      Q_OBJECT

      public:

        VectorToVectorDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~VectorToVectorDialog();


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

        void setFunctionsByLayer(te::map::AbstractLayerPtr layer);

        void setStatisticalSummary();

        te::map::AbstractLayerPtr getCurrentFromLayer();

        te::map::AbstractLayerPtr getCurrentToLayer();

        std::map<std::string, std::vector<te::attributefill::OperationType> > getSelections();

        bool isPolygon(te::gm::GeomType type);

        bool isPoint(te::gm::GeomType type);

        bool isValidPropertyType(const int type);

        bool isClassType(const int type);

        te::gm::GeomType getCurrentToLayerGeomType();

        bool isNumProperty(const int type);

      protected slots:

        void onTargetDatasourceToolButtonPressed();

        void onTargetFileToolButtonPressed();

        void onFromLayerComboBoxCurrentIndexChanged(int index);

        void onToLayerComboBoxCurrentIndexChanged(int index);

        void onHelpPushButtonClicked();

        void onOkPushButtonClicked();

        void onCancelPushButtonClicked();

        void onSelectAllComboBoxChanged(int index);

        void onRejectAllComboBoxChanged(int index);

        void onStatisticsListWidgetItemPressed(QListWidgetItem * item);

      private:

        typedef std::map<te::stat::StatisticalSummary, std::string> StaticalSummaryMap;

        std::auto_ptr<Ui::VectorToVectorDialogForm> m_ui;             //!< User interface.
        te::da::DataSourceInfoPtr m_outputDatasource;                 //!< DataSource information.
        std::list<te::map::AbstractLayerPtr> m_layers;                //!< List of layers.
        std::string m_path;                                           //!< Output layer path;
        bool m_toFile;
    };
  }   // end namespace attributefill
}     // end namespace te

#endif  // __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTORDIALOG_H
