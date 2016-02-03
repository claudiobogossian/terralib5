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
  \file terralib/qt/widgets/externaltable/FieldsDialog.h

  \brief A Qt Dialog that allows users to modify which fields will be included in a query
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_FIELDSDIALOG_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_FIELDSDIALOG_H

// TerraLib
#include "../../../dataaccess/query/Fields.h"
#include "../../../dataaccess/Enums.h"
#include "../Config.h"

//QT
#include <QDialog>

namespace Ui { class FieldsDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DoubleListWidget;

      /*!
        \class FieldsWidget

        \brief This class represents a FieldsWidget component.
      */
      class TEQTWIDGETSEXPORT FieldsDialog : public QDialog
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor

            It constructs a FieldsWidget.

            \param parent this widget's parent
            \param f Window flags used to configure this widget

          */
          FieldsDialog(QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~FieldsDialog();

          /*!
            \brief Clears the input values list
          */
          void clearInputValues();

          /*!
            \brief Clears the output values list
          */
          void clearOutputValues();

          /*!
            \brief Sets the input values list
          */
          void setInputValues(std::vector<std::string> values);

          /*!
            \brief Sets the output values list
          */

          void setOutputValues(std::vector<std::string> values);

          /*!
            \brief Sets the fixed output values list
          */
          void setFixedOutputValues(std::vector<std::string> values, std::string iconName);

          /*!
            \brief Sets the label displayed above the left list widget
          */
          void setLeftLabel(const QString& value);

          /*!
            \brief Sets the label displayed above the right list widget
          */
          void setRightLabel(const QString& value);

          /*!
            \brief Returns the selected fields
            \note By default, all fields will be used
            \note The caller will take ownership of the returned pointer
          */
          te::da::Fields* getFields();

          /*!
            \brief Returns the selected JoinType
          */
          te::da::JoinType getJoinType();

        protected

          slots:

            void onOkPushButtonClicked();

            void onCancelPushButtonClicked();

        private:

          std::auto_ptr<DoubleListWidget>      m_fieldsWidget;  //!< The widget used to select which fields will be added to the query.
          std::auto_ptr<Ui::FieldsDialogForm>  m_ui;            //!< The widget's form.
      };
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_FIELDSDIALOG_H