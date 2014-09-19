/*  Copyright (C) 2010-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/externaltable/TableLinkDialog.h

  \brief A Qt dialog that allows users to create a new query layer based on the information of two distinct datasets
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TABLELINKDIALOG_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TABLELINKDIALOG_H

// TerraLib
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/query/Fields.h"
#include "../../../maptools/DataSetLayer.h"
#include "../Config.h"

//QT
#include <QDialog>

namespace Ui { class TableLinkDialogForm; }

namespace te
{
  namespace da { class Join;
                 class Select; }

  namespace qt
  {
    namespace widgets
    {
      class DataSetTableView;
      class FieldsDialog;

      /*!
        \class TableLinkDialog

        \brief This class represents a TableLinkDialog component.
      */
      class TEQTWIDGETSEXPORT TableLinkDialog : public QDialog
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor

            It constructs a TableLinkDialog.

            \param parent this dialogs's parent
            \param f Window flags used to configure this dialog

          */
          TableLinkDialog(QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~TableLinkDialog();

          /*!
            \brief It sets the Datasource that is being used to generate the new Layer.

            \param datasource The dasource being used.
          */
          void setInputLayer(te::map::AbstractLayerPtr inLayer);

          /*!
            \brief Returns the generated Join.
            \note The caller will take ownership of the returned pointer
          */
          te::da::Join* getJoin();

          /*!
            \brief Returns a new query based on the requested parameters.
          */
          te::da::Select getSelectQuery();

          /*!
            \brief Returns a new query layer based on the requested parameters.
          */
          te::map::AbstractLayerPtr getQueryLayer();

        protected: 

          void getDataSets();

          void getProperties();

        protected

          slots:

            void  done(int r);

            void onDataCBIndexChanged(int index);

            /*!
              \brief Displays or hides the data of the tabular Dataset
            */
            void onDataToolButtonnClicked();

            /*!
              \brief Displays or hides the widget's used to configure the query's fields.
            */
            void onAdvancedToolButtonnClicked();

            void onOkPushButtonClicked();

        private:

          te::map::DataSetLayerPtr                m_inputLayer;    //!< The layer the will serve as the base for the link.
          te::da::DataSourcePtr                   m_ds;            //!< The dasource of the layers to be linked.
          std::auto_ptr<DataSetTableView>         m_tabularView;   //!< The widget used to preview the data of the tabular dataset.
          std::auto_ptr<FieldsDialog>             m_fieldsDialog;  //!< The widget used to select which fields will be added to the query.
          std::auto_ptr<Ui::TableLinkDialogForm>  m_ui;            //!< The widget's form.
      };
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TABLELINKDIALOG_H