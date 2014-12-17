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
  \file terralib/addressgeocoding/qt/ImportTableDialog.h

  \brief A dialog to import table with address to geocoding operation
*/

#ifndef __TERRALIB_ADDRESSGEOCODING_INTERNAL_IMPORTTABLEDIALOG_H
#define __TERRALIB_ADDRESSGEOCODING_INTERNAL_IMPORTTABLEDIALOG_H

// TerraLib
#include "../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../memory/DataSet.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/widgets/table/DataSetTableView.h"
#include "../Config.h"

// STL
#include <map>
#include <memory>

// Qt
#include <QDialog>
#include <QWidget>

namespace Ui { class ImportTableDialogForm; }

// Forward declarations
class QModelIndex;

namespace te
{
  namespace addressgeocoding
  {
    class TEADDRESSGEOCODINGEXPORT ImportTableDialog : public QDialog
    {
      Q_OBJECT

      public:

        ImportTableDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~ImportTableDialog();
        
        te::da::DataSourcePtr getDataSource();

        std::auto_ptr<te::da::DataSet> getDataSet();

      protected slots:

        void onInputDataToolButtonTriggered();

        void onHelpPushButtonClicked();

        void onOkPushButtonClicked();

        void onCancelPushButtonClicked();

      private:

        std::auto_ptr<Ui::ImportTableDialogForm>  m_ui;
        std::auto_ptr<te::qt::widgets::DataSetTableView>  m_tblView;
        std::auto_ptr<te::da::DataSetTypeConverter> m_dsConverter;
        std::auto_ptr<te::da::DataSet>  m_dataSet;
        //te::mem::DataSet* m_dataSetMem;
        std::auto_ptr<te::da::DataSetType>  m_dataType;
        te::da::DataSourcePtr m_dataSource;
    };
  }   // end namespace addressgeocoding
}     // end namespace te

#endif  // __TERRALIB_ADDRESSGEOCODING_INTERNAL_IMPORTTABLEDIALOG_H
