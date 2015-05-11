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
  \file terralib/qt/widgets/rp/RasterNavigatorWizardPage.h

  \brief This file defines a class for a Raster Navigator Wizard page.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_RASTERNAVIGATORWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_RASTERNAVIGATORWIZARDPAGE_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWizardPage>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class RasterNavigatorWidget;

      /*!
        \class RasterNavigatorWizardPage

        \brief This class is a wizard page for the RasterNavigator widget.
      */
      class TEQTWIDGETSEXPORT RasterNavigatorWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          RasterNavigatorWizardPage(QWidget* parent = 0);

          ~RasterNavigatorWizardPage();

          bool isComplete() const;

        public:

          te::qt::widgets::RasterNavigatorWidget* getWidget();

          /*!
            \brief This method is used to set the selected layer for mixture model operation
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void set(te::map::AbstractLayerPtr layer);

        private:

          std::auto_ptr<te::qt::widgets::RasterNavigatorWidget> m_navigator;

          te::map::AbstractLayerPtr m_layer;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_RASTERNAVIGATORWIZARDPAGE_H
