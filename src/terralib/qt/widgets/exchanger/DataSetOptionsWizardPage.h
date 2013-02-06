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
  \file terralib/plugins/exchanger/DataSetOptionsWizardPage.h

  \brief A Qt dialog for ....
*/

#ifndef __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DATASETOPTIONSWIZARDPAGE_H
#define __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DATASETOPTIONSWIZARDPAGE_H

// TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../Config.h"

// STL
#include <list>
#include <memory>

// Qt
#include <QtGui/QWizardPage>

// Forward declaration
namespace Ui { class DataSetOptionsWizardPageForm; }

class QListWidgetItem;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT DataSetOptionsWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          DataSetOptionsWizardPage(QWidget* parent = 0);

          ~DataSetOptionsWizardPage();

          void set(const std::list<te::da::DataSetTypePtr>& datasets,
                   const te::da::DataSourceInfoPtr& datasource,
                   const te::da::DataSourceInfoPtr& targetDatasource);

          const std::list<te::da::DataSetTypePtr>& getDatasets() const;

        protected slots:

          void addUkToolButtonPressed();

          void removeUkToolButtonPressed();

          void editUkToolButtonPressed();

          void addIndexToolButtonPressed();

          void removeIndexToolButtonPressed();

          void editIndexToolButtonPressed();

          void sridSearchToolButtonPressed();

          void applyChangesPushButtonPressed();

          void datasetPressed(QListWidgetItem* item);

          void enableApplyButton();

          //void checkModificationsOnFocusChange(QWidget* old, QWidget* now);

        protected:

          te::da::DataSetTypePtr getSelectedDataSet() const;

        private:

          void clearForm();

          void setControlsEnabled(bool enabled);

        private:

          struct FindByName
          {
            std::string m_name;

            FindByName(const QString& name);

            bool operator()(const te::da::DataSetTypePtr& dataset) const;
          };

        private:

          std::auto_ptr<Ui::DataSetOptionsWizardPageForm> m_ui;
          std::list<te::da::DataSetTypePtr> m_datasets;
          te::da::DataSourceInfoPtr m_datasource;
          te::da::DataSourceInfoPtr m_targetDatasource;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DATASETOPTIONSWIZARDPAGE_H
