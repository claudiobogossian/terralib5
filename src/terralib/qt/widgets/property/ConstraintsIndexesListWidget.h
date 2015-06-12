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
  \file terralib/qt/widgets/property/ConstraintsIndexesListWidget.h

  \brief This file has the ConstraintsIndexesListWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_CONSTRAINTSINDEXESLISTWIDGET_H
#define __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_CONSTRAINTSINDEXESLISTWIDGET_H

// TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../Config.h"

// Qt
#include <QWidget>

namespace Ui { class ConstraintsIndexesListWidgetForm; }

namespace te
{
  namespace da
  {
    class Constraint;
    class Index;
  }

  namespace dt { class Property; }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ConstraintsIndexesListWidget

        \brief This class is used to list the constraints and indexes of a datasetype.
      */
      class TEQTWIDGETSEXPORT ConstraintsIndexesListWidget : public QWidget
      {
        Q_OBJECT

        public:

          ConstraintsIndexesListWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~ConstraintsIndexesListWidget();

          Ui::ConstraintsIndexesListWidgetForm* getForm() const;


        public:

          void setDataSetType(te::da::DataSetType* dsType);


        public slots:

          void onAddToolButtonClicked();

          void onRemoveToolButtonClicked();

          void onEditToolButtonClicked();

          void onCellClicked(int row, int col);


        protected:

          void listDataSetProperties();

          void addConstraint(te::da::Constraint* c);

          void addIndex(te::da::Index* i);

          void removePrimaryKey(const std::string& name);

          void removeUniqueKey(const std::string& name);

          void removeIndex(const std::string& name);

          void addTableItem(std::string name, std::string type, std::string properties);

          std::string getPropertiesStr(std::vector<te::dt::Property*> vec);

        private:

         std::auto_ptr<Ui::ConstraintsIndexesListWidgetForm> m_ui;
         te::da::DataSetType* m_dsType;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_CONSTRAINTSINDEXESLISTWIDGET_H

