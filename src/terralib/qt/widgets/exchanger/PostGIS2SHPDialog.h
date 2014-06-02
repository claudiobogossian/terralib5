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
  \file terralib/qt/widgets/exchanger/PostGIS2SHPDialog.h

  \brief A exchanger dialog from PostGis to SHP operation
*/

#ifndef __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_POSTGIS2SHPDIALOG_H
#define __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_POSTGIS2SHPDIALOG_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class PostGis2SHPDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declarations

      class TEQTWIDGETSEXPORT PostGIS2SHPDialog : public QDialog
      {
        Q_OBJECT

        public:

          PostGIS2SHPDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~PostGIS2SHPDialog();

          /*!
            \brief Set the layer that can be used

            \param layers   List of AbstractLayerPtr
          */
          void setLayers(std::list<te::map::AbstractLayerPtr> layers);

        protected slots:

          void onHelpPushButtonClicked();

          void onOkPushButtonClicked();

          void onDirToolButtonClicked();

        private:

          std::auto_ptr<Ui::PostGis2SHPDialogForm> m_ui;
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_POSTGIS2SHPDIALOG_H