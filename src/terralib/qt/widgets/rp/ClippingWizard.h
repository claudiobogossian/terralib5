/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/rp/ClippingWizard.h

  \brief A Qt dialog that allows users to run a clipping operation defined by RP module.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_CLIPPINGWIZARD_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_CLIPPINGWIZARD_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QWizard>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class ClippingWizardPage;
      class LayerSearchWizardPage;
      class RasterInfoWizardPage;

      /*!
        \class ClippingWizard

        \brief This class is GUI used to define the clipping wizard for the RP constast operation.
      */
      class TEQTWIDGETSEXPORT ClippingWizard : public QWizard
      {
        public:

          ClippingWizard(QWidget* parent);

          ~ClippingWizard();

        public:

          virtual bool validateCurrentPage();

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          void setLayer(te::map::AbstractLayerPtr layer);

          te::map::AbstractLayerPtr getOutputLayer();

        protected:

          void addPages();

          bool execute();

          bool executeExtentClipping();

          bool executeDimensionClipping();

          bool executeLayerClipping();

        private:

          std::auto_ptr<te::qt::widgets::ClippingWizardPage> m_clippingPage;
          std::auto_ptr<te::qt::widgets::LayerSearchWizardPage> m_layerSearchPage;
          std::auto_ptr<te::qt::widgets::RasterInfoWizardPage> m_rasterInfoPage;

          te::map::AbstractLayerPtr m_outputLayer;
      };
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_CLIPPINGWIZARD_H
