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
\file src/terraMobilePlugin/qt/GeoPackageSynchronizerDialog.h

\brief This interface is used to get the input parameters for GeoPackage Synchronizer operation.
*/

#ifndef __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_GEOPACKAGESYNCHRONIZERDIALOG_H
#define __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_GEOPACKAGESYNCHRONIZERDIALOG_H

// TerraLib
#include "../../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class GeoPackageSynchronizerDialogForm; }

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace terramobile
      {
        /*!
          \class GeoPackageSynchronizerDialog

          \brief This interface is used to get the input parameters for GeoPackage Synchronizer operation.
        */
        class GeoPackageSynchronizerDialog : public QDialog
        {
          Q_OBJECT

          public:

            GeoPackageSynchronizerDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

            ~GeoPackageSynchronizerDialog();

          protected slots:

            void onGeopackageToolButtonClicked();

            void onSynchronizePushButtonClicked();

          public:

            void setLayerList(std::list<te::map::AbstractLayerPtr> list);

          private:

            std::auto_ptr<Ui::GeoPackageSynchronizerDialogForm> m_ui;

        }; 
      }   // end namespace thirdParty
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te

#endif  // __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_GEOPACKAGESYNCHRONIZERDIALOG_H

