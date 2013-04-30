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
  \file terralib/vp/qt/AggregationDialog.h

  \brief A dialog intersection operation
*/

#ifndef __TERRALIB_VP_INTERNAL_AGGREGATIONDIALOG_H
#define __TERRALIB_VP_INTERNAL_AGGREGATIONDIALOG_H

// TerraLib
#include "../../maptools/AbstractLayer.h"
#include "../core/Config.h"

// STL
#include <list>
#include <memory>

// Qt
#include <QtGui/QDialog>

namespace Ui { class AggregationDialogForm; }

class QModelIndex;
class QTreeWidgetItem;

namespace te
{
  namespace vp
  {
    class TEVPEXPORT AggregationDialog : public QDialog
    {
      Q_OBJECT

      public:

        AggregationDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~AggregationDialog();

        void setLayers(std::list<te::map::AbstractLayerPtr> layers);

        void setSelectedLayers(std::vector<std::string> selectedLayers);

        void setOperations();

      private:

      protected slots:

        void onLayerTreeViewClicked(QTreeWidgetItem * item, int column);

        void onFilterLineEditTextChanged(const QString& text);

        void onCancelPushButtonClicked();

        void onHelpPushButtonClicked();

      private:

        std::auto_ptr<Ui::AggregationDialogForm> m_ui;
        std::list<te::map::AbstractLayerPtr> m_layers;
        std::vector<std::string> m_selectedLayers;
    };
  }   // end namespace vp
}     // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_AGGREGATIONDIALOG_H