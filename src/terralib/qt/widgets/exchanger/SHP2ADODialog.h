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
  \file terralib/qt/widgets/exchanger/SHP2ADODialog.h

  \brief A exchanger dialog from SHP to ADO operation
*/

#ifndef __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_SHP2ADODIALOG_H
#define __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_SHP2ADODIALOG_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class SHP2ADODialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declarations

      class TEQTWIDGETSEXPORT SHP2ADODialog : public QDialog
      {
        Q_OBJECT

        public:

          SHP2ADODialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~SHP2ADODialog();

          /*!
            \brief Set the layer that can be used

            \param layers   List of AbstractLayerPtr
          */
          void setLayers(std::list<te::map::AbstractLayerPtr> layers);

          /*!
            \brief Set the list of data sources that can be used
          */
          void setDataSources();

        protected slots:

          void onHelpPushButtonClicked();

          void onOkPushButtonClicked();

        private:

          std::auto_ptr<Ui::SHP2ADODialogForm> m_ui;
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_SHP2ADODIALOG_H