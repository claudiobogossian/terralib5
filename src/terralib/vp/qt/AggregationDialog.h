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
  \file terralib/vp/qt/AggregationDialog.h

  \brief A dialog aggregation operation
*/

#ifndef __TERRALIB_VP_INTERNAL_AGGREGATIONDIALOG_H
#define __TERRALIB_VP_INTERNAL_AGGREGATIONDIALOG_H

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

namespace Ui { class AggregationDialogForm; }

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

    class TEVPEXPORT AggregationDialog : public QDialog
    {
      Q_OBJECT

      public:

        AggregationDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~AggregationDialog();

        /*!
          \brief Set the layer that can be used

          \param layers   List of AbstractLayerPtr
        */
        void setLayers(std::list<te::map::AbstractLayerPtr> layers);

        /*!
          \brief Get the Grouping Functions Type based on selected QListWidgetItem.

          \return Map with Properties and Grouping Functions Type.
        */
        std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > getStatisticalSummary();

        /*!
          \brief Get the selected property based on selected QListWidgetItem using the name of property.

          \return a property.
        */
        te::dt::Property* getSelectedPropertyByName(std::string propertyName);

        /*!
          \brief Get the selected properties based on selected QListWidgetItem.

          \return a Vector with selected properties.
        */
        std::vector<te::dt::Property*> getSelectedProperties();

        /*!
          \brief Get the generated layer.

          \return the generated layer.
        */
        te::map::AbstractLayerPtr getLayer();

        /*!
          \brief Set Statistical Summary Type for combobox 'm_selectAllComboBox' and 'm_rejectAllComboBox' based on a enum.
        */
        void setStatisticalSummary();

        /*!
          \brief Map Statistical Summary Type enum for an intuitive name.
        */
        void setStatisticalSummaryMap();

        /*!
          \brief Set Grouping Functions Type for 'm_outputListWidget' based on Selected Layer.
        */
        void setFunctionsByLayer(std::vector<te::dt::Property*> properties);

      protected slots:

        void onLayerComboBoxChanged(int index);

        void onFilterLineEditTextChanged(const QString& text);

        void onCalculateStatistics(bool visible);

        void onSelectAllComboBoxChanged(int index);

        void onRejectAllComboBoxChanged(int index);

        void onOutputListWidgetClicked(QListWidgetItem * item);

        void onTargetDatasourceToolButtonPressed();

        void onTargetFileToolButtonPressed();

        void onHelpPushButtonClicked();

        void onOkPushButtonClicked();

        void onCancelPushButtonClicked();

      private:

        typedef std::map<te::stat::StatisticalSummary, std::string> StaticalSummaryMap;

        std::auto_ptr<Ui::AggregationDialogForm> m_ui;
        te::da::DataSourceInfoPtr m_outputDatasource;                 //!< DataSource information.
        std::list<te::map::AbstractLayerPtr> m_layers;                //!< List of layers.
        te::map::AbstractLayerPtr m_selectedLayer;                    //!< Layer used for aggregation
        std::vector<te::dt::Property*> m_properties;                  //!< Properties related to the selected Layer
        StaticalSummaryMap m_StatisticalSummaryMap;                   //!< Maping of Statistical Summary enum
        te::map::AbstractLayerPtr m_layer;                            //!< Generated Layer.
        bool m_toFile;
    };
  }   // end namespace vp
}     // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_AGGREGATIONDIALOG_H
