/*  Copyright (C) 2001-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file RenameProperty.h

  \brief It changes the name of a property.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_RENAMEPROPERTY_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_RENAMEPROPERTY_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QDialog>
#include <ui_RenameProperty.h>

namespace te
{
  namespace da
  {
    class DataSource;
  }

  namespace dt { class Property; }

  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT RenameProperty : public QDialog, public Ui_RenamePropertyDialog
      {
        Q_OBJECT

        public:
          RenameProperty(te::da::DataSource* ds, QWidget* parent = 0);

          ~RenameProperty();

          te::dt::Property* getProperty() const  { return m_property; }

          std::string getOldPropertyName();
          std::string getNewPropertyName();

        private slots:
          void dataSetComboBoxChanged(const QString& dataSet); 
          void okPushButtonClicked();
          void cancelPushButtonClicked();
          void helpPushButtonClicked();

        protected:
          void closeEvent(QCloseEvent* e);

        private:
          te::da::DataSource* m_ds;
          te::dt::Property* m_property;
      };
    }
  }
}

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_RENAMEPROPERTY_H
