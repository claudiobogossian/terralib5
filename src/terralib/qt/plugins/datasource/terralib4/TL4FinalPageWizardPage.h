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

#ifndef __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4FINALPAGEWIZARDPAGE_H
#define __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4FINALPAGEWIZARDPAGE_H

// TerraLib
#include "Config.h"

// STL
#include <memory>

// Qt
#include <QWizardPage>

// Forward declaration
namespace Ui { class TL4FinalPageWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace terralib4
      {
        class TL4FinalPageWizardPage : public QWizardPage
        {
          Q_OBJECT

          public:

            TL4FinalPageWizardPage(QWidget* parent = 0);

            ~TL4FinalPageWizardPage();

            void setDataSets(const std::vector<std::string>& datasets);

            void setThemes(const std::vector<std::pair<std::string, std::string> >& layers_themes);

            std::vector<std::string> getSelected();

            std::vector<std::pair<std::string, std::string> > getSelectedThemes();

          private:

            bool parentLayerIsSeleted(std::string layerName);

          protected slots:

            void onSelectAllPushButtonClicked();

            void onDeselectAllPushButtonClicked();

          private:

            std::auto_ptr<Ui::TL4FinalPageWizardPageForm> m_ui;
        };
      } // end namespace terralib4
    } // end namespace plugins
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4FINALPAGEWIZARDPAGE_H
