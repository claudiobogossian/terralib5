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
  \file terralib/qt/widgets/property/PrimaryKeyConstraintWidget.h

  \brief This file has the PrimaryKeyConstraintWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_PKCONSTRAINTWIDGET_H
#define __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_PKCONSTRAINTWIDGET_H

// TerraLib
#include "../Config.h"
#include "ConstraintWidget.h"


namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class PrimaryKeyConstraintWidget

        \brief This class is used to define a  Primare Key Constraint DataSet class object.
      */
      class TEQTWIDGETSEXPORT PrimaryKeyConstraintWidget : public ConstraintWidget
      {
        Q_OBJECT

        public:

          PrimaryKeyConstraintWidget(te::da::DataSetType* dsType, QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~PrimaryKeyConstraintWidget();

        public:

          virtual te::da::Constraint* getConstraint();

          virtual void setConstraint(te::da::Constraint* constraint);
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_PKCONSTRAINTWIDGET_H

