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
  \file terralib/vp/qt/SummarizationDialog.h

  \brief A dialog summarization operation
*/

#ifndef __TERRALIB_VP_INTERNAL_SUMMARIZATIONDIALOG_H
#define __TERRALIB_VP_INTERNAL_SUMMARIZATIONDIALOG_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../Config.h"
#include "../Enums.h"

// STL
#include <list>
#include <map>
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class SummarizationDialogForm; }

// Forward declarations
class QListWidgetItem;
class QModelIndex;

namespace te
{
  namespace vp
  {
// Forward declarations
    class LayerTreeModel;

    class TEVPEXPORT SummarizationDialog : public QDialog
    {
      Q_OBJECT

      public:

        SummarizationDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~SummarizationDialog();

        /*!
          \brief Set the layer that can be used

          \param layers List of AbstractLayerPtr
        */
        void setLayers(std::list<te::map::AbstractLayerPtr> layers);

      protected slots:
        
        void onHelpPushButtonClicked();

        void onOkPushButtonClicked();

        void onCancelPushButtonClicked();

      private:

        std::auto_ptr<Ui::SummarizationDialogForm> m_ui;
        te::da::DataSourceInfoPtr m_outputDatasource;                                     //!< DataSource information.
        std::list<te::map::AbstractLayerPtr> m_layers;                                    //!< List of layers.
        te::map::AbstractLayerPtr m_selectedLayer;                                        //!< Layer used for summarization
    };
  }   // end namespace vp
}     // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_SUMMARIZATIONDIALOG_H
