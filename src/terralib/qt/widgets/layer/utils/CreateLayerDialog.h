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
  \file terralib/qt/widgets/layer/utils/CreateLayerDialog.h

  \brief This interface is used to create new layer operation.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_CREATELAYERDIALOG_H
#define __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_CREATELAYERDIALOG_H

// TerraLib
#include "../../../../maptools/AbstractLayer.h"
#include "../../Config.h"

// STL
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class CreateLayerDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class CreateLayerWidget;

      /*!
        \class CreateLayerDialog

        \brief This interface is used to create new layer operation.
      */
      class TEQTWIDGETSEXPORT CreateLayerDialog : public QDialog
      {
        Q_OBJECT

        public:

          CreateLayerDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~CreateLayerDialog();

        public:

          te::map::AbstractLayerPtr getLayer();

        protected slots:

          void onOkPushButtonClicked();

        private:

          std::auto_ptr<Ui::CreateLayerDialogForm> m_ui;

          te::qt::widgets::CreateLayerWidget* m_createLayerWidget;
      }; 
    }     // end namespace widgets
  }       // end namespace qt
}         // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_CREATELAYERDIALOG_H

