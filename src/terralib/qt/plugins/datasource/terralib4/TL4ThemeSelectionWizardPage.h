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
  \file terralib/qt/plugins/TL4RasterFolderSelectionWizardPage.h

  \brief A wizard page for selecting converted datasets to layers in TerraLib 5.
*/

#ifndef __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4THEMESELECTIONWIZARDPAGE_H
#define __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4THEMESELECTIONWIZARDPAGE_H

// TerraLib
#include "../../../../terralib4/ThemeInfo.h"
#include "Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QWizardPage>

// Forward declaration
namespace Ui { class TL4ThemeSelectionWizardPageForm; }

struct ThemeInfo;

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace terralib4
      {
        class TL4ThemeSelectionWizardPage : public QWizardPage
        {
          Q_OBJECT

          public:

            TL4ThemeSelectionWizardPage(QWidget* parent = 0);

            ~TL4ThemeSelectionWizardPage();

            void setThemes(const std::vector<::terralib4::ThemeInfo>& themes);

            std::vector<::terralib4::ThemeInfo> getThemes();

          private:

            std::vector<std::string> getNames(const std::string& names);

          protected slots:

            void onSelectAllPushButtonClicked();

            void onDeselectAllPushButtonClicked();

          private:

            std::auto_ptr<Ui::TL4ThemeSelectionWizardPageForm> m_ui;
        };
      } // end namespace terralib4
    } // end namespace plugins
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4THEMESELECTIONWIZARDPAGE_H
