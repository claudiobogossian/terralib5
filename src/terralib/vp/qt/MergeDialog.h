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
  \file terralib/vp/qt/MergeDialog.h

  \brief A dialog merge operation
*/

#ifndef __TERRALIB_VP_INTERNAL_MERGEDIALOG_H
#define __TERRALIB_VP_INTERNAL_MERGEDIALOG_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class MergeDialogForm; }

// Forward declarations
class QComboBox;

namespace te
{
  namespace dt { class Property; }
  namespace vp
  {
// Forward declarations
    class LayerTreeModel;

    class TEVPEXPORT MergeDialog : public QDialog
    {
      Q_OBJECT

      public:

        MergeDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~MergeDialog();

        /*!
          \brief Set the layer that can be used

          \param layers   List of AbstractLayerPtr
          \param selectedLayer   Selected AbstractLayerPtr
        */
        void setLayers(std::list<te::map::AbstractLayerPtr> layers);

        /*!
          \brief Get the generated layer.

          \return the generated layer.
        */
        te::map::AbstractLayerPtr getLayer();

      private:

        void updateFirstLayerComboBox();

        void updateSecondLayerComboBox();

        void updateAttrTableWidget();

        QComboBox* getPossibleAttributesComboBox(te::dt::Property* mainProp, std::vector<te::dt::Property*> props);

        QComboBox* getAllAttributesComboBox(std::vector<te::dt::Property*> props);

        std::vector<std::pair<std::string, std::string> > getTablePropertiesNames();

      protected slots:

        void onFirstLayerComboBoxChanged(int index);

        void onSecondLayerComboBoxChanged(int index);

        void onTargetDatasourceToolButtonPressed();

        void onTargetFileToolButtonPressed();

        void onOkPushButtonClicked();

        void onCancelPushButtonClicked();

      private:

        std::auto_ptr<Ui::MergeDialogForm> m_ui;

        te::da::DataSourceInfoPtr m_outputDatasource;                 //!< DataSource information.
        std::list<te::map::AbstractLayerPtr> m_layers;                //!< List of layers.
        te::map::AbstractLayerPtr m_outputLayer;                                          //!< Generated Layer.
        bool m_toFile;
        
        te::map::AbstractLayerPtr m_firstSelectedLayer;   //!< First layer selected.
        te::map::AbstractLayerPtr m_secondSelectedLayer;  //!< Second layer selected.
    };
  }   // end namespace vp
}     // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_MERGEDIALOG_H
