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
  \file terralib/qt/widgets/rp/ContrastWizardPage.h

  \brief This file defines a class for a Contrast Wizard page.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_CONTRASTWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_CONTRASTWIZARDPAGE_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../../../rp/Contrast.h"
#include "../Config.h"

// Qt
#include <QtGui/QWizardPage>

// Forward declaration
namespace Ui { class ContrastWizardPageForm; }

namespace te
{
  namespace gm { class Envelope; }

  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;

      /*!
        \class ContrastWizardPage

        \brief This class is GUI used to define the contrast parameters for the RP constast operation.
      */
      class TEQTWIDGETSEXPORT ContrastWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          ContrastWizardPage(QWidget* parent = 0);

          ~ContrastWizardPage();

        public:

          /*!
            \brief This method is used to set the selected layer for contrast operation
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void set(te::map::AbstractLayerPtr layer);

          te::rp::Contrast::InputParameters getInputParams();

          te::rp::Contrast::OutputParameters getOutputParams();

        protected:

          void apply();

          void preview();

          te::gm::Envelope calculateExtent();

          void fillContrastTypes();

          void listBands();

        private:

          std::auto_ptr<Ui::ContrastWizardPageForm> m_ui;
          std::auto_ptr<te::qt::widgets::MapDisplay> m_mapDisplay;

          te::map::AbstractLayerPtr m_layer;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_CONTRASTWIZARDPAGE_H
