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
  \file terralib/qt/widgets/layer/utils/SaveLayerAsDialog.h

  \brief This interface is used to create a new layer based on a layer with selected objects.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_SAVELAYERASDIALOG_H
#define __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_SAVELAYERASDIALOG_H

// TerraLib
#include "../../../../maptools/AbstractLayer.h"
#include "../../Config.h"

// STL
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class SaveLayerAsDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class SaveLayerAsWidget;

      /*!
        \class CreateLayerDialog

        \brief This interface is used to create a new layer based on a layer with selected objects.
      */
      class TEQTWIDGETSEXPORT SaveLayerAsDialog : public QDialog
      {
        Q_OBJECT

        public:

          SaveLayerAsDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~SaveLayerAsDialog();

        public:

          /*! \brief Set a layer. */
          void setParameters(te::map::AbstractLayerPtr layer);

        protected slots:

          void onOkPushButtonClicked();

          void onCancelPushButtonClicked();

        private:

          std::auto_ptr<Ui::SaveLayerAsDialogForm> m_ui;

          te::qt::widgets::SaveLayerAsWidget* m_saveLayerAsWidget;
      }; 
    }     // end namespace widgets
  }       // end namespace qt
}         // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_SAVELAYERASDIALOG_H

