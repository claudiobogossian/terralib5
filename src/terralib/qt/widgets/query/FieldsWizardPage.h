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
  \file terralib/qt/widgets/query/FieldsWizardPage.h

  \brief This file has the FieldsWizardPage class.
*/

#ifndef __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_FIELDSWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_FIELDSWIZARDPAGE_H

// TerraLib
#include "../../../dataaccess/query/Fields.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWizardPage>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DoubleListWidget;

      /*!
        \class FieldsWizardPage

        \brief This class is a wizard page with the DoubleListWidget component,
		       used to create the FIELD object of a select operation.
      */
      class TEQTWIDGETSEXPORT FieldsWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          FieldsWizardPage(QWidget* parent = 0);

          virtual ~FieldsWizardPage();

          bool isComplete() const;

          DoubleListWidget* getWidget() const;

          te::da::Fields* getFields();

          void setFields(const te::da::Fields* fields);

        public slots:

          void onItemChanged();


        private:

          std::auto_ptr<DoubleListWidget> m_widget;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_FIELDSWIZARDPAGE_H

