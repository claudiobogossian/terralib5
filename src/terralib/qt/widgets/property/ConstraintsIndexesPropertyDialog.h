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
  \file terralib/qt/widgets/property/ConstraintsIndexesPropertyDialog.h

  \brief A class used to define a constraint or index property creator.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_CONSTRAINTSINDEXESPROPERTYDIALOG_H
#define __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_CONSTRAINTSINDEXESPROPERTYDIALOG_H

// TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../Config.h"


// Qt
#include <QDialog>
#include <QGridLayout>

namespace Ui { class ConstraintsIndexesPropertyDialogForm; }

namespace te
{
  namespace da 
  { 
    class Constraint;
    class Index;
  }

  namespace qt
  {
    namespace widgets
    {
      class PrimaryKeyConstraintWidget;
      class UniqueKeyConstraintWidget;
      class IndexWidget;

      /*!
        \class ConstraintsIndexesPropertyDialog

        \brief A dialog for creating a constraint or a index property.
      */
      class TEQTWIDGETSEXPORT ConstraintsIndexesPropertyDialog : public QDialog
      {
        Q_OBJECT

        public:

          ConstraintsIndexesPropertyDialog(te::da::DataSetType* dsType, QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~ConstraintsIndexesPropertyDialog();

          void setConstraint(te::da::Constraint* constraint);

          void setIndex(te::da::Index* index);

        public slots:

          void onpkRadioButtonToggled(bool status);
          void onukRadioButtonToggled(bool status);
          void onidxRadioButtonToggled(bool status);
          void onokPushButtonClicked();

        protected:

          void freeWidget();

        private:

          std::auto_ptr<Ui::ConstraintsIndexesPropertyDialogForm> m_ui;
          te::da::DataSetType* m_dsType;
          te::qt::widgets::PrimaryKeyConstraintWidget* m_pkConstraintWidget;
          te::qt::widgets::UniqueKeyConstraintWidget* m_ukConstraintWidget;
          te::qt::widgets::IndexWidget* m_indexWidget;

          QGridLayout* m_widgetLayout;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_CONSTRAINTSINDEXESPROPERTYDIALOG_H

