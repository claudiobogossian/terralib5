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
  \file terralib/qt/widgets/rp/MosaicWizard.h

  \brief A Qt dialog that allows users to run a mosaic operation defined by RP module.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_MOSAICWIZARD_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_MOSAICWIZARD_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWizard>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class MosaicWizardPage;
      class LayerSearchWizardPage;
      class RasterInfoWizardPage;

      /*!
        \class MosaicWizard

        \brief This class is GUI used to define the mosaic wizard for the RP mosaic operation.
      */
      class TEQTWIDGETSEXPORT MosaicWizard : public QWizard
      {

        public:

          MosaicWizard(QWidget* parent);

          ~MosaicWizard();

        public:

          virtual bool validateCurrentPage();

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          std::list<te::map::AbstractLayerPtr> getOutputLayers();

        protected:

          void addPages();

          bool execute();

          bool executeGeoMosaic();

          bool executeTiePointMosaic();

          bool executeSequenceMosaic();

        private:

          std::auto_ptr<te::qt::widgets::MosaicWizardPage> m_mosaicPage;
          std::auto_ptr<te::qt::widgets::LayerSearchWizardPage> m_layerSearchPage;
          std::auto_ptr<te::qt::widgets::RasterInfoWizardPage> m_rasterInfoPage;

          std::list<te::map::AbstractLayerPtr> m_outputLayerList;
      };
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_MOSAICWIZARD_H
