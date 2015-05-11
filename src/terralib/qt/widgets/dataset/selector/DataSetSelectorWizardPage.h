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
  \file terralib/qt/widgets/dataset/selector/DataSetSelectorWizardPage.h

  \brief ...
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASET_SELECTOR_INTERNAL_DATASETSELECTORWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_DATASET_SELECTOR_INTERNAL_DATASETSELECTORWIZARDPAGE_H

// TerraLib
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../Config.h"

// STL
#include <list>
#include <memory>

// Qt
#include <QWizardPage>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DataSetItem;
      class DataSetSelectorWidget;

      class TEQTWIDGETSEXPORT DataSetSelectorWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          enum CompleteModeType
          {
            HAVING_CHECKED_ITEMS,
            HAVING_SELECTED_ITEMS
          };

          DataSetSelectorWizardPage(QWidget* parent = 0);

          virtual ~DataSetSelectorWizardPage();

          void initializePage();

          bool isComplete() const;

          void set(const te::da::DataSourceInfoPtr& ds, bool useCheckableItems = false);

          std::list<te::da::DataSetTypePtr> getCheckedDataSets() const;

          std::list<te::da::DataSetTypePtr> getSelectedDataSets() const;

          void setCompleteMode(CompleteModeType m);

          DataSetSelectorWidget* getSelectorWidget() const;

        protected slots:

          void onDataSetToggled(DataSetItem* item);

          void onDataSetClicked(DataSetItem* item);

        private:

          std::auto_ptr<DataSetSelectorWidget> m_selectorW;
          CompleteModeType m_completeMode;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_DATASET_SELECTOR_INTERNAL_DATASETSELECTORWIZARDPAGE_H

