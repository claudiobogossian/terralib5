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
  \file terralib/qt/widgets/rp/MixtureModelWizard.h

  \brief A Qt dialog that allows users to run a mixture model operation defined by RP module.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_MIXTUREMODELWIZARD_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_MIXTUREMODELWIZARD_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../rp/MixtureModel.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWizard>
#include <QActionGroup>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class MixtureModelWizardPage;
      class LayerSearchWizardPage;
      class RasterInfoWizardPage;

      /*!
        \class MixtureModelWizard

        \brief This class is GUI used to define the mixture model wizard for the RP constast operation.
      */
      class TEQTWIDGETSEXPORT MixtureModelWizard : public QWizard
      {
        Q_OBJECT

        public:

          MixtureModelWizard(QWidget* parent);

          ~MixtureModelWizard(); 

        public:

          virtual bool validateCurrentPage();

          std::list<te::map::AbstractLayerPtr> getOutputLayers();

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          void setMapDisplay(te::qt::widgets::MapDisplay* mapDisplay);

          void setActionGroup(QActionGroup* actionGroup);

          void setLayer(std::list<te::map::AbstractLayerPtr> layers);

        protected:
          bool decompose(te::rp::MixtureModel::InputParameters &algoInputParams, te::rp::MixtureModel::OutputParameters &algoOutputParams);

          void addPages();

          bool execute();

        signals:

          void addLayer(te::map::AbstractLayerPtr layer);

        private:

          std::auto_ptr<te::qt::widgets::MixtureModelWizardPage> m_mixtureModelPage;
          std::auto_ptr<te::qt::widgets::LayerSearchWizardPage> m_layerSearchPage;
          std::auto_ptr<te::qt::widgets::RasterInfoWizardPage> m_rasterInfoPage;

          std::list<te::map::AbstractLayerPtr> m_outputLayerList;

          int m_layerSearchId;
      };
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_MIXTUREMODELWIZARD_H
