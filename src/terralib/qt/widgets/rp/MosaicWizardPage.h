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
  \file terralib/qt/widgets/rp/MosaicWizardPage.h

  \brief This file defines a class for a Mosaic Wizard page.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_MOSAICWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_MOSAICWIZARDPAGE_H

// TerraLib
#include "../../../geometry/GTParameters.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../rp/GeoMosaic.h"
#include "../../../rp/SequenceMosaic.h"
#include "../../../rp/TiePointsMosaic.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWizardPage>


// Forward declaration
namespace Ui { class MosaicWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class TiePointLocatorParametersWidget;

      /*!
        \class MosaicWizardPage

        \brief This class is GUI used to define the mosaic parameters for the RP mosaic operation.
      */
      class TEQTWIDGETSEXPORT MosaicWizardPage : public QWizardPage
      {
        Q_OBJECT

          enum MosaicTypes
          {
            MOSAIC_GEO,
            MOSAIC_TIEPOINT,
            MOSAIC_SEQUENCE
          };

        public:

          MosaicWizardPage(QWidget* parent = 0);

          ~MosaicWizardPage();

          bool isComplete() const;

        public:

          bool isGeoMosaic();

          bool isTiePointMosaic();

          bool isSequenceMosaic();

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          te::rp::GeoMosaic::InputParameters getInputGeoParams();

          te::rp::GeoMosaic::OutputParameters getOutputGeoParams();

          te::rp::TiePointsMosaic::InputParameters getInputTPParams();

          te::rp::TiePointsMosaic::OutputParameters getOutputTPParams();

          te::rp::SequenceMosaic::InputParameters getInputSeqParams();

          te::rp::SequenceMosaic::OutputParameters getOutputSeqParams();

          te::map::AbstractLayerPtr getTiePointMosaicLayerA();

          te::map::AbstractLayerPtr getTiePointMosaicLayerB();

        public slots:

          void onTiePointsAcquiredToolButtonClicked();

        protected:

          void fillMosaicTypes();

        private:

          std::auto_ptr<Ui::MosaicWizardPageForm> m_ui;

          te::qt::widgets::TiePointLocatorParametersWidget* m_tiePointParameters;     //!<Tie Point parameters widget
          
          std::list<te::map::AbstractLayerPtr> m_layerList;

          std::vector< std::vector< te::gm::GTParameters::TiePoint > > m_tiePoints; //!< Tie-points between each adjacent raster pair (te::gm::GTParameters::TiePoint::first are raster (with index i) lines/columns, te::gm::GTParameters::TiePoint::second are raster (with index I+1) lines/columns ,and so on).

      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_MOSAICWIZARDPAGE_H
