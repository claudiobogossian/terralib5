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
  \file terralib/qt/plugins/terralib5/TL4ConverterWizard.h

  \brief A wizard for guiding the conversion of a TerraLib 4.x database to a OGC SFS compliant format or any other format known by OGR and GDAL.
*/

#ifndef __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4CONVERTERWIZARD_H
#define __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4CONVERTERWIZARD_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "Config.h"

// STL
#include <map>
#include <memory>
#include <string>

// Qt
#include <QTableWidget>
#include <QWizard>
#include <QWizardPage>

// Forward declaration
namespace Ui { class TL4ConverterWizardForm; }

namespace te
{
  namespace da { class DataSource; }

  namespace qt
  {
    namespace widgets{class DataSourceSelectorWizardPage;}

    namespace plugins
    {
      namespace terralib4
      {
        class TL4ConnectorWizardPage;
        class TL4LayerSelectionWizardPage;
        class TL4RasterFolderSelectionWizardPage;
        class TL4FinalPageWizardPage;
        class TL4ThemeSelectionWizardPage;

        class TEQTPLUGINTERRALIB4EXPORT TL4ConverterWizard : public QWizard
        {
          Q_OBJECT

          public:

            TL4ConverterWizard(QWidget* parent = 0, Qt::WindowFlags f = 0);

            ~TL4ConverterWizard();

            int nextId() const;

            bool validateCurrentPage();

          private:

            bool validTerraLib4Connection();

            bool validLayerSelection();

            bool validLayerNames();

            std::string getOriginalName(const std::string& targetName);

            std::string getNewName(const std::string& originalName);

          protected slots:

            void back();

            void next();

            void commit();

            void finish();

            void help();

          private:

            enum
            {
              PAGE_TERRALIB4_CONNECTOR,
              PAGE_LAYER_SELECTION,
              PAGE_DATASOURCE_SELECTOR,
              PAGE_RASTERFOLDER_SELECTOR,
              PAGE_NAME_RESOLVE_SELECTOR,
              PAGE_FINALPAGE,
              PAGE_THEME_SELECTION
            };

            bool m_hasNonRaster;
            bool m_hasRaster;
            bool m_rollback;

            std::string m_rasterFolderPath;

            std::auto_ptr<te::da::DataSource> m_tl4Database;

            te::da::DataSourceInfoPtr m_targetDataSource;

            std::auto_ptr<Ui::TL4ConverterWizardForm> m_ui;
            std::auto_ptr<TL4ConnectorWizardPage> m_connectorPage;
            std::auto_ptr<te::qt::widgets::DataSourceSelectorWizardPage>m_datasourceSelectorPage;
            std::auto_ptr<TL4LayerSelectionWizardPage> m_layerSelectionPage;
            std::auto_ptr<TL4RasterFolderSelectionWizardPage> m_rasterFolderSelectionPage;
            std::auto_ptr<QWizardPage> m_resolveNamePage;
            std::auto_ptr<TL4FinalPageWizardPage> m_finalPage;
            std::auto_ptr<TL4ThemeSelectionWizardPage> m_themeSelection;

            std::auto_ptr<QTableWidget> m_resolveNameTableWidget;
        };
      } // end namespace terralib4
    } // end namespace plugins
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_PLUGINS_TERRALIB4_INTERNAL_TL4CONVERTERWIZARD_H
