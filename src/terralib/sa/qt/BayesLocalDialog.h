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
  \file terralib/sa/qt/BayesLocalDialog.h

  \brief A dialog to calculate the local empirical bayes of a dataset.
*/

#ifndef __TERRALIB_SA_INTERNAL_BAYESLOCALDIALOG_H
#define __TERRALIB_SA_INTERNAL_BAYESLOCALDIALOG_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#include "../core/GeneralizedProximityMatrix.h"
#include "../Config.h"

// Qt
#include <QDialog>

namespace Ui { class BayesLocalDialogForm; }

// Forward declarations

namespace te
{
  namespace sa
  {
// Forward declarations


    class TESAEXPORT BayesLocalDialog : public QDialog
    {
      Q_OBJECT

      public:

        BayesLocalDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~BayesLocalDialog();

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

        void fillRateCorrection();

        std::auto_ptr<te::sa::GeneralizedProximityMatrix> loadGPM();

      private:

        std::auto_ptr<Ui::BayesLocalDialogForm> m_ui;

        te::da::DataSourceInfoPtr m_outputDatasource;

        te::map::AbstractLayerPtr m_outputLayer;                                          //!< Generated Layer.

        bool m_toFile;
    };
  }   // end namespace sa
}     // end namespace te

#endif  // __TERRALIB_SA_INTERNAL_BAYESLOCALDIALOG_H
