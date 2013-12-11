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
  \file terralib/qt/plugins/terralib4/TL4ConnectorWizardPage.h

  \brief A wizard page that provides the capability of connecting to a TerraLib 4.x database.
*/

#ifndef __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4CONNECTORWIZARDPAGE_H
#define __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4CONNECTORWIZARDPAGE_H

// TerraLib
#include "Config.h"

// STL
#include <map>
#include <memory>
#include <string>

// Qt
#include <QtGui/QWizardPage>

// Forward declaration
namespace Ui { class TL4ConnectorWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace terralib4
      {
        /*!
          \class TL4ConnectorWizardPage

          \brief A wizard page that provides the capability of connecting to a TerraLib 4.x database.
        */
        class TL4ConnectorWizardPage : public QWizardPage
        {
          Q_OBJECT

          public:

            TL4ConnectorWizardPage(QWidget* parent = 0);

            ~TL4ConnectorWizardPage();

            std::map<std::string, std::string> getConnInfo();

          private:

            std::string getDriverName(const std::string& displayName);

          protected slots:

            void onDatabaseTypeComboBoxActivated(const QString& text);

            void onDirectoryPushButtonClicked();

          private:

            std::auto_ptr<Ui::TL4ConnectorWizardPageForm> m_ui;
        };

      } // end namespace terralib4
    } // end namespace plugins
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4CONNECTORWIZARDPAGE_H
