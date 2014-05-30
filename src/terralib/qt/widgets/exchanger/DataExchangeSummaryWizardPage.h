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
  \file terralib/plugins/exchanger/DataExchangeSummaryWizardPage.h

  \brief A Qt dialog for ....
*/

#ifndef __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DATAEXCHANGESUMMARYWIZARDPAGE_H
#define __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DATAEXCHANGESUMMARYWIZARDPAGE_H

// TerraLib
#include "../Config.h"

// STL
#include <list>
#include <memory>

// Qt
#include <QWizardPage>

// Forward declaration
namespace Ui { class DataExchangeSummaryWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      struct DataExchangeStatus;

      class TEQTWIDGETSEXPORT DataExchangeSummaryWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          DataExchangeSummaryWizardPage(QWidget* parent = 0);

          ~DataExchangeSummaryWizardPage();

          bool isComplete() const;

          void set(const std::list<DataExchangeStatus>& result);

        private:

          std::auto_ptr<Ui::DataExchangeSummaryWizardPageForm> m_ui;

          bool m_status;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_PLUGINS_EXCHANGER_INTERNAL_DATAEXCHANGESUMMARYWIZARDPAGE_H
