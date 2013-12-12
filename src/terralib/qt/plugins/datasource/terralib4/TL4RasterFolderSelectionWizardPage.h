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

  \brief A wizard for enabling the location to save raster files from an input TerraLib 4.x database.
*/

#ifndef __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4RASTERFOLDERSELECTIONWIZARDPAGE_H
#define __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4RASTERFOLDERSELECTIONWIZARDPAGE_H

// TerraLib
#include "Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QWizardPage>

// Forward declaration
namespace Ui { class TL4RasterFolderSelectionWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace widgets { class FileChooser; }

    namespace plugins
    {
      namespace terralib4
      {
        class TL4RasterFolderSelectionWizardPage : public QWizardPage
        {
          Q_OBJECT

          public:

            TL4RasterFolderSelectionWizardPage(QWidget* parent = 0);

            ~TL4RasterFolderSelectionWizardPage();

            std::string getPath();

          protected slots:

            void onChooseToolButtonClicked();

          private:

            std::auto_ptr<te::qt::widgets::FileChooser> m_fileChooser;
            std::auto_ptr<Ui::TL4RasterFolderSelectionWizardPageForm> m_ui;
        };
      } // end namespace terralib4
    } // end namespace plugins
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4RASTERFOLDERSELECTIONWIZARDPAGE_H
