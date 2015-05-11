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
  \file terralib/qt/widgets/rp/ClippingWizardPage.h

  \brief This file defines a class for a Clipping Wizard page.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_CLIPPINGWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_CLIPPINGWIZARDPAGE_H

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWizardPage>

// Forward declaration
namespace Ui { class ClippingWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class RasterNavigatorWidget;

      /*!
        \class ClippingWizardPage

        \brief This class is GUI used to define the clipping parameters for the RP constast operation.
      */
      class TEQTWIDGETSEXPORT ClippingWizardPage : public QWizardPage
      {
        Q_OBJECT

          enum CLIPPING_TYPE
          {
            CLIPPING_EXTENT,
            CLIPPING_DIMENSION,
            CLIPPING_LAYER
          };

        public:

          ClippingWizardPage(QWidget* parent = 0);

          ~ClippingWizardPage();

          bool isComplete() const;

        public:

          /*!
            \brief This method is used to set the selected layer for clipping operation
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void set(te::map::AbstractLayerPtr layer);

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          te::map::AbstractLayerPtr get();

          bool isExtentClipping();

          bool isDimensionClipping();

          bool isLayerClipping();

          bool isSingleRasterResult();

          void getExtentClipping(te::gm::Envelope& env);

          void getDimensionClipping(int& x, int& y, int& width, int& height);

          void getLayerClipping( std::auto_ptr< te::gm::GeometryCollection >& geomColl);

        public slots:

          void onStrategyTypeComboBoxActivated(int index);

          void onStrategyTypeComboBoxChanged(int index);

          void onLayerComboBoxActivated(int index);
          
          void onLayerComboBoxChanged(int index);

          void onEnvelopeAcquired(te::gm::Envelope env);

          void drawGeom();

        protected:

          void fillClippingTypes();

        private:

          std::auto_ptr<Ui::ClippingWizardPageForm> m_ui;
          std::auto_ptr<te::qt::widgets::RasterNavigatorWidget> m_navigator;

          te::map::AbstractLayerPtr m_layer;

          te::gm::Envelope m_envExt;
          te::gm::Envelope m_envDim;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_CLIPPINGWIZARDPAGE_H
