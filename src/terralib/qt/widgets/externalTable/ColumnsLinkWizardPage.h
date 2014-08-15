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

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_COLUMNSLINKWIZARDPAGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_COLUMNSLINKWIZARDPAGET_H

// TerraLib
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
      class ColumnsLinkWidget;
      /*!
        \class ColumnLinkWizardPage

        \brief This class is a wizard page with the ColumnsLinkWidget component.
      */
      class TEQTWIDGETSEXPORT ColumnsLinkWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          ColumnsLinkWizardPage(QWidget* parent = 0);

          virtual ~ColumnsLinkWizardPage();

          bool isComplete() const;

          ColumnsLinkWidget* getWidget() const;


        public slots:

          void onItemChanged();


        private:

          std::auto_ptr<ColumnsLinkWidget> m_widget;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_COLUMNSLINKWIDGET_H