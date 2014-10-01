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
  \file terralib/qt/widgets/rp/RasterInfoWizardPage.h

  \brief This file defines a class for a Raster Info Wizard page.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_RASTERINFOWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_RASTERINFOWIZARDPAGE_H

// TerraLib
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
      class RasterInfoWidget;

      /*!
        \class RasterInfoWizardPage

        \brief This class is GUI used to define the raster info parameters for raster factory.
      */
      class TEQTWIDGETSEXPORT RasterInfoWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          RasterInfoWizardPage(QWidget* parent = 0);

          ~RasterInfoWizardPage();

          bool isComplete() const;

        public:

          te::qt::widgets::RasterInfoWidget* getWidget();

        protected slots:

          void onChanged();

          void onChanged(QString);

        private:

          std::auto_ptr<te::qt::widgets::RasterInfoWidget> m_rinfo;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_RASTERINFOWIZARDPAGE_H
