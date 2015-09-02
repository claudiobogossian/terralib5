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
  \file terralib/qt/widgets/rp/FusionWizardPage.h

  \brief This file defines a class for a Fusion Wizard page.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_FUSIONWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_FUSIONWIZARDPAGE_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../../../rp/IHSFusion.h"
#include "../../../rp/PCAFusion.h"
#include "../../../rp/WisperFusion.h"
#include "../../../qt/widgets/charts/ChartDisplay.h"
#include "../../../qt/widgets/charts/Scatter.h"
#include "../../../qt/widgets/charts/ScatterChart.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QComboBox>
#include <QToolButton>
#include <QWizardPage>


// Forward declaration
namespace Ui { class FusionWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      enum FusionTypes
      {
        FUSION_IHS,
        FUSION_PCA,
        FUSION_WISPER
      };

      /*!
        \class FusionWizardPage

        \brief This class is GUI used to define the fusion parameters for the RP fusion operation.
      */
      class TEQTWIDGETSEXPORT FusionWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          FusionWizardPage(QWidget* parent = 0);

          ~FusionWizardPage();

          bool isComplete() const;

        public:

          /*!
            \brief This method is used to set the selected layer for classifier operation
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void setLower(te::map::AbstractLayerPtr layer);

          /*!
            \brief This method is used to set the selected layer for classifier operation
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void setHigher(te::map::AbstractLayerPtr layer);

          bool isIHSFusion();

          bool isPCAFusion();

          bool isWisperFusion();

          bool cropRasters();

          te::rp::IHSFusion::InputParameters getInputIHSParams();

          te::rp::IHSFusion::OutputParameters getOutputIHSParams();

          te::rp::PCAFusion::InputParameters getInputPCAParams();

          te::rp::PCAFusion::OutputParameters getOutputPCAParams();

          te::rp::WisperFusion::InputParameters getInputWisperParams();

          te::rp::WisperFusion::OutputParameters getOutputWisperParams();

        protected slots:

          void onHighResSensorTypeActivated(int idx);

          void onLowResSensorTypeActivated(int idx);

          void onHighCsvToolButtonClicked();

          void onLowCsvToolButtonClicked();

        protected:

          void fillFusionTypes();

          void fillSensorTypes(QComboBox* combo);

          void fillWaveletTypes();

          void listBandsLower();

          void listBandsHigher();

          std::map<double, double> getSRFMap(const int& st, std::string stName);

        private:

          std::auto_ptr<Ui::FusionWizardPageForm> m_ui;

          QDialog* m_chartDialog;

          te::qt::widgets::ChartDisplay* m_chartDisplay;

          te::qt::widgets::Scatter* m_scatterHighRes;
          te::qt::widgets::ScatterChart* m_scatterChartHighRes;

          std::vector<te::qt::widgets::Scatter*> m_scatterLowResVec;
          std::vector<te::qt::widgets::ScatterChart*> m_scatterChartLowResVec;

          std::map<QComboBox*, int> m_comboMap;
          std::map<QToolButton*, int> m_buttonMap;

          te::map::AbstractLayerPtr m_layerLower;
          te::map::AbstractLayerPtr m_layerHigher;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_FUSIONWIZARDPAGE_H
