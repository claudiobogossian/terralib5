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
  \file terralib/vp/qt/BufferDialog.h

  \brief A dialog buffer operation
*/

#ifndef __TERRALIB_VP_INTERNAL_BUFFERDIALOG_H
#define __TERRALIB_VP_INTERNAL_BUFFERDIALOG_H

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
#include <QtGui/QDialog>

namespace Ui { class BufferDialogForm; }

// Forward declarations
class QListWidgetItem;
class QModelIndex;

namespace te
{
  namespace vp
  {
// Forward declarations
    class LayerTreeModel;

    class TEVPEXPORT BufferDialog : public QDialog
    {
      Q_OBJECT

      public:

        BufferDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~BufferDialog();

        /*!
          \brief Set the layer that can be used

          \param layers   List of AbstractLayerPtr
        */
        void setLayers(std::list<te::map::AbstractLayerPtr> layers);

        void setPossibleLevels();

        void setAttributesForDistance(std::vector<te::dt::Property*> properties);

      private:

      protected slots:

        void onLayerComboBoxChanged(int index);

        void onFilterLineEditTextChanged(const QString& text);

        void onFixedDistanceToggled();

        void onAttDistanceToggled();

        void onRuleInOutToggled();

        void onRuleOutToggled();

        void onRuleInToggled();

        void onWithoutBoundToggled();

        void onWithBoundToggled();

        void onHelpPushButtonClicked();

        void onOkPushButtonClicked();

        void onCancelPushButtonClicked();

      private:

        std::auto_ptr<Ui::BufferDialogForm> m_ui;
        te::da::DataSourceInfoPtr m_outputDatasource;                                     //!< DataSource information.
        std::list<te::map::AbstractLayerPtr> m_layers;                                    //!< List of layers.
        te::map::AbstractLayerPtr m_selectedLayer;                                        //!< Layer used for buffer
        std::vector<te::dt::Property*> m_properties;                                      //!< Properties related to the selected Layer
    };
  }   // end namespace vp
}     // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_BUFFERDIALOG_H