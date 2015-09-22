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
\file src/terraMobilePlugin/qt/BuilderGatheringLayersWizardPage.h

\brief This interface is used to get the gathering layers parameters for GeoPackage Builder wizard operation.
*/

#ifndef __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_BUILDERGATHERINGLAYERSWIZARDPAGE_H
#define __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_BUILDERGATHERINGLAYERSWIZARDPAGE_H

// TerraLib
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
      class DoubleListWidget;
    }

    namespace plugins
    {
      namespace terramobile
      {
        /*!
          \class BuilderGatheringLayersWizardPage

          \brief This interface is used to get the gathering layers parameters for GeoPackage Builder wizard operation.
        */
        class BuilderGatheringLayersWizardPage : public QWizardPage
        {
          Q_OBJECT

          public:

            BuilderGatheringLayersWizardPage(QWidget* parent = 0);

            ~BuilderGatheringLayersWizardPage();

          public:

            bool isComplete() const;

            te::qt::widgets::DoubleListWidget* getWidget() const;

          public slots:

            void onItemChanged();

          private:

            std::auto_ptr<te::qt::widgets::DoubleListWidget> m_widget;
        }; 
      }   // end namespace thirdParty
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te

#endif  // __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_BUILDERGATHERINGLAYERSWIZARDPAGE_H

