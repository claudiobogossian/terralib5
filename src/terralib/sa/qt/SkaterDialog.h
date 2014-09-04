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
  \file terralib/sa/qt/SkaterDialog.h

  \brief A dialog for Spatial 'K'luster Analysis by Tree Edge Removal.
*/

#ifndef __TERRALIB_SA_INTERNAL_SKATERDIALOG_H
#define __TERRALIB_SA_INTERNAL_SKATERDIALOG_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/widgets/utils/DoubleListWidget.h"
#include "../core/GeneralizedProximityMatrix.h"
#include "../Config.h"

// Qt
#include <QDialog>

namespace Ui { class SkaterDialogForm; }

// Forward declarations

namespace te
{
  namespace sa
  {
// Forward declarations

    class TESAEXPORT SkaterDialog : public QDialog
    {
      Q_OBJECT

      public:

        SkaterDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~SkaterDialog();

        /*!
          \brief Set the layer that can be used

          \param layers   List of AbstractLayerPtr
        */
        void setLayers(std::list<te::map::AbstractLayerPtr> layers);

        te::map::AbstractLayerPtr getOutputLayer();

      protected slots:

        void onInputLayerComboBoxActivated(int index);

        void onOkPushButtonClicked();

        void onTargetDatasourceToolButtonPressed();

        void onTargetFileToolButtonPressed();

        void onGPMToolButtonClicked();

      protected:

        std::auto_ptr<te::sa::GeneralizedProximityMatrix> loadGPM();

      private:

        std::auto_ptr<Ui::SkaterDialogForm> m_ui;

        te::qt::widgets::DoubleListWidget* m_doubleListWidget;                            //!< Widget used to select attributes.

        te::da::DataSourceInfoPtr m_outputDatasource;

        te::map::AbstractLayerPtr m_outputLayer;                                          //!< Generated Layer.

        bool m_toFile;
    };
  }   // end namespace sa
}     // end namespace te

#endif  // __TERRALIB_SA_INTERNAL_SKATERDIALOG_H
