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
  \file terralib/sa/qt/ProximityMatrixCreatorDialog.h

  \brief A dialog for Proximity Matrix Creation
*/

#ifndef __TERRALIB_SA_INTERNAL_PROXIMITYMATRIXCREATORDIALOG_H
#define __TERRALIB_SA_INTERNAL_PROXIMITYMATRIXCREATORDIALOG_H

// TerraLib
#include "../../maptools/AbstractLayer.h"
#include "../Config.h"
#include "../Enums.h"

// Qt
#include <QDialog>

namespace Ui { class ProximityMatrixCreatorDialogForm; }

// Forward declarations

namespace te
{
  namespace sa
  {
// Forward declarations


    class TESAEXPORT ProximityMatrixCreatorDialog : public QDialog
    {
      Q_OBJECT

      public:

        ProximityMatrixCreatorDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~ProximityMatrixCreatorDialog();

        /*!
          \brief Set the layer that can be used

          \param layers   List of AbstractLayerPtr
        */
        void setLayers(std::list<te::map::AbstractLayerPtr> layers);

      protected slots:

        void onInputLayerComboBoxActivated(int index);

        void onFileToolButtonClicked();

        void onOkPushButtonClicked();

      private:

        std::auto_ptr<Ui::ProximityMatrixCreatorDialogForm> m_ui;
    };
  }   // end namespace sa
}     // end namespace te

#endif  // __TERRALIB_SA_INTERNAL_PROXIMITYMATRIXCREATORDIALOG_H
