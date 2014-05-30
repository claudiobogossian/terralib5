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
  \file terralib/qt/widgets/utils/ParameterDialog.h

  \brief A dialog to be used as input of key/value pairs.
*/

#ifndef __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_PARAMETERDIALOG_H
#define __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_PARAMETERDIALOG_H

// TerraLib
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QDialog>

// Forward declaraion
namespace Ui { class ParameterDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ParameterDialog

        \brief A dialog to be used as input of key/value pairs.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT ParameterDialog : public QDialog
      {
        Q_OBJECT

        public:

          ParameterDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~ParameterDialog();

          QString getName() const;

          void setName(const QString& name);

          QString getValue() const;

          void setValue(const QString& value);

        protected slots:

          void onParamNameTextEdited(const QString& nextTxt);

          void onParamValueTextEdited(const QString& nextTxt);

        private:

          std::auto_ptr<Ui::ParameterDialogForm> m_ui;
          QString m_pname;
          QString m_pvalue;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_PARAMETERDIALOG_H

