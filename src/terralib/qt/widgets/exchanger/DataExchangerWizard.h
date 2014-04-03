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
  \file terralib/plugins/exchanger/DataExchangerWizard.h

  \brief A Qt dialog for ....
*/

#ifndef __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DATAEXCHANGERWIZARD_H
#define __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DATAEXCHANGERWIZARD_H

// TerraLib
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../Config.h"

// STL
#include <memory>
#include <set>

// Qt
#include <QWizard>

// Forward declaration
namespace Ui { class DataExchangerWizardForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DataExchangeSummaryWizardPage;
      class DataSetOptionsWizardPage;
      class DataSetSelectorWizardPage;
      class DataSourceSelectorWizardPage;

      /*!
        \class DataExchangerWizard

        \brief  Qt dialog for ....

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT DataExchangerWizard : public QWizard
      {
        Q_OBJECT

        public:

          DataExchangerWizard(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~DataExchangerWizard();

          int nextId() const;

          te::da::DataSourceInfoPtr getDataSource() const;

          te::da::DataSourceInfoPtr getTargetDataSource() const;

        protected slots:

          void back();

          void next();

          void commit();

        private:

          enum
          {
            PAGE_DATASOURCE_SELECTION,
            PAGE_DATASET_SELECTION,
            PAGE_TARGET_DATASOURCE,
            PAGE_DATASET_OPTIONS,
            PAGE_SUMMARY
          };

          std::auto_ptr<Ui::DataExchangerWizardForm> m_ui;
          std::auto_ptr<DataSourceSelectorWizardPage> m_datasourceSelectorPage;
          std::auto_ptr<DataSetSelectorWizardPage> m_datasetSelectorPage;
          std::auto_ptr<DataSourceSelectorWizardPage> m_targetSelectorPage;
          std::auto_ptr<DataSetOptionsWizardPage> m_datasetOptionsPage;
          std::auto_ptr<DataExchangeSummaryWizardPage> m_summaryPage;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DATAEXCHANGERWIZARD_H
