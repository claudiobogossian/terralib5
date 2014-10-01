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
  \file terralib/qt/widgets/dataset/selector/DataSetSelectorDialog.h

  \brief ....
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASET_SELECTOR_INTERNAL_DATASETSELECTORDIALOG_H
#define __TERRALIB_QT_WIDGETS_DATASET_SELECTOR_INTERNAL_DATASETSELECTORDIALOG_H

// TerraLib
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../Config.h"

// Qt
#include <QDialog>

namespace Ui { class DataSetSelectorDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DataSetItem;
      class DataSetGroupItem;
      class DataSetSelectorWidget;

      /*!
        \class DataSetSelectorDialog

        \brief ....

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT DataSetSelectorDialog : public QDialog
      {
        Q_OBJECT

        public:

          enum SelectModeType
          {
            CHECKED_ITEMS,
            SELECTED_ITEMS
          };

          DataSetSelectorDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~DataSetSelectorDialog();

          void set(const te::da::DataSourceInfoPtr& ds, bool useCheckableItems = false);

          std::list<te::da::DataSetTypePtr> getCheckedDataSets() const;

          std::list<std::string> getCheckedGeomProperties() const;

          std::list<te::da::DataSetTypePtr> getSelectedDataSets() const;

          std::list<std::string> getSelectedGeomProperties() const;

          void setSelectMode(SelectModeType m);

          DataSetSelectorWidget* getSelectorWidget() const;

        protected slots:

          void onDataSetToggled(DataSetItem* item);

          void onDataSetClicked(DataSetItem* item);

          void onDataSetGroupToggled(DataSetGroupItem* item);

          void onDataSetGroupClicked(DataSetGroupItem* item);

        protected:

          void updateSelectPushButton();

        private:

          std::auto_ptr<Ui::DataSetSelectorDialogForm> m_ui;
          std::auto_ptr<DataSetSelectorWidget> m_selectorW;
          SelectModeType m_selectMode;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_DATASET_SELECTOR_INTERNAL_DATASETSELECTORDIALOG_H

