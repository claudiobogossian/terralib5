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
  \file src/terraMobilePlugin/qt/BuilderOutputWizardPage.h

  \brief This interface is used to get the output parameters for GeoPackage Builder wizard operation.
*/

#ifndef __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_BUILDEROUTPUTWIZARDPAGE_H
#define __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_BUILDEROUTPUTWIZARDPAGE_H

// TerraLib
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWizardPage>

// Forward declaration
namespace Ui { class BuilderOutputWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace terramobile
      {
        /*!
          \class BuilderOutputWizardPage

          \brief This interface is used to get the output parameters for GeoPackage Builder wizard operation.
        */
        class BuilderOutputWizardPage : public QWizardPage
        {
          Q_OBJECT

          public:

            BuilderOutputWizardPage(QWidget* parent = 0);

            ~BuilderOutputWizardPage();

          public:

            bool isComplete() const;

            std::string getGeoPackageFilePath();

            bool useVisibleArea();

            void appendLogMesssage(std::string msg);

          public slots:

            void onDirToolButtonPressed();

          private:

            std::auto_ptr<Ui::BuilderOutputWizardPageForm> m_ui;
        }; 
      }   // end namespace thirdParty
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te

#endif  // __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_BUILDEROUTPUTWIZARDPAGE_H

