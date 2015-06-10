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
  \file terralib/addressgeocoding/qt/ConfigNumberDialog.h

  \brief Associates address number.
*/

#ifndef __TERRALIB_ADDRESSGEOCODING_INTERNAL_CONFIGNUMBERDIALOG_H
#define __TERRALIB_ADDRESSGEOCODING_INTERNAL_CONFIGNUMBERDIALOG_H

// TerraLib
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <map>
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class ConfigNumberDialogForm; }

// Forward declarations
class QModelIndex;

namespace te
{
  namespace addressgeocoding
  {
    class TEADDRESSGEOCODINGEXPORT ConfigNumberDialog : public QDialog
    {
      Q_OBJECT

      public:

        ConfigNumberDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~ConfigNumberDialog();

        void setLayer(te::map::AbstractLayerPtr layer);

        std::string getInitialLeft();

        std::string getFinalLeft();

        std::string getInitialRight();

        std::string getFinalRight();

      protected slots:

        void onHelpPushButtonClicked();

        void onOkPushButtonClicked();

        void onCancelPushButtonClicked();

      private:

        std::auto_ptr<Ui::ConfigNumberDialogForm> m_ui;
        te::da::DataSourcePtr m_dataSource;
        std::auto_ptr<te::da::DataSet> m_dataSet;
        te::map::AbstractLayerPtr m_selectedLayer;
        std::vector<te::dt::Property*> m_properties;
    };
  }   // end namespace addressgeocoding
}     // end namespace te

#endif  // __TERRALIB_ADDRESSGEOCODING_INTERNAL_CONFIGNUMBERDIALOG_H
