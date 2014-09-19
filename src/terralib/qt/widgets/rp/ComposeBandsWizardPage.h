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
  \file terralib/qt/widgets/rp/ComposeBandsWizardPage.h

  \brief This file defines a class for a Compose / Decompose Bands Wizard page.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_COMPOSEBANDSWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_COMPOSEBANDSWIZARDPAGE_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../../../raster/Interpolator.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWizardPage>


// Forward declaration
namespace Ui { class ComposeBandsWizardPageForm; }

namespace te
{
  namespace rst { class Raster; }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ComposeBandsWizardPage

        \brief This class is GUI used to defines a class for a Compose / Decompose Bands Wizard page.
      */
      class TEQTWIDGETSEXPORT ComposeBandsWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          ComposeBandsWizardPage(QWidget* parent = 0);

          ~ComposeBandsWizardPage();

          bool isComplete() const;

        public:

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          bool isCompose();

          bool isDecompose();

          void getComposeParameters(std::vector<const te::rst::Raster*>& rasters, std::vector<unsigned int>& bands, te::rst::Interpolator::Method& interpMethod);

          void getDecomposeParameters(te::rst::Raster*& raster, std::vector<unsigned int>& bands);

        protected:

          void fillInterpolatorTypes();

        private:

          std::auto_ptr<Ui::ComposeBandsWizardPageForm> m_ui;

          std::list<te::map::AbstractLayerPtr> m_layerList;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_COMPOSEBANDSWIZARDPAGE_H
