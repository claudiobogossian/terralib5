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
  \file terralib/qt/widgets/query/GroupByWizardPage.h

  \brief This file has the GroupByWizardPage class.
*/

#ifndef __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_GROUPBYWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_GROUPBYWIZARDPAGE_H

// TerraLib
#include "../../../dataaccess/query/GroupBy.h"
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
        \class GroupByWizardPage

        \brief This class is a wizard page with the DoubleListWidget component,
		       used to create the GROUPBY object of a select operation. 
      */
      class TEQTWIDGETSEXPORT GroupByWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          GroupByWizardPage(QWidget* parent = 0);

          virtual ~GroupByWizardPage();

          bool isComplete() const;

          DoubleListWidget* getWidget() const;

          te::da::GroupBy* getGroupBy();

        private:

          std::auto_ptr<DoubleListWidget> m_widget;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_GROUPBYWIZARDPAGE_H

