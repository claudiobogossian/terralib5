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
  \file terralib/vp/qt/IntersectionDialog.h

  \brief A dialog intersection operation
*/

#ifndef __TERRALIB_VP_QT_INTERNAL_INTERSECTIONDIALOG_H
#define __TERRALIB_VP_QT_INTERNAL_INTERSECTIONDIALOG_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#include "../core/Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QDialog>

namespace Ui { class IntersectionDialogForm; }

namespace te
{
  namespace vp
  {
// Forward declarations
    class LayerTreeModel;

    /*!
      \class IntersectionDialog

      \brief A dialog used to execute vector intersection.
    */
    class TEVPEXPORT IntersectionDialog : public QDialog
    {
      Q_OBJECT

      public:

        IntersectionDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~IntersectionDialog();

        /*!
          \brief Set the layer that can be used

          \param layers   List of AbstractLayerPtr
        */
        void setLayers(std::list<te::map::AbstractLayerPtr> layers);

      protected slots:

        void onFilterLineEditTextChanged(const QString& text);

        void onHelpPushButtonClicked();

        void onOkPushButtonClicked();

        void onTargetDatasourceToolButtonPressed();

        void onTargetFileToolButtonPressed();

      private:

        std::auto_ptr<Ui::IntersectionDialogForm> m_ui;
        te::da::DataSourceInfoPtr m_outputDatasource;   //!< DataSource information.
        std::list<te::map::AbstractLayerPtr> m_layers;  //!< List of layers.
        LayerTreeModel* m_model;                        //!< Layer Tree Model.
    };
  }   // end namespace vp
}     // end namespace te

#endif  // __TERRALIB_VP_QT_INTERNAL_INTERSECTIONDIALOG_H