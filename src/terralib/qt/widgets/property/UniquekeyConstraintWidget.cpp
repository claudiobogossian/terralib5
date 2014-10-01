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
  \file terralib/qt/widgets/property/UniqueKeyConstraintWidget.cpp

  \brief This file has the UniqueKeyConstraintWidget class.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/UniqueKey.h"
#include "../utils/DoubleListWidget.h"
#include "UniquekeyConstraintWidget.h"
#include "ui_ConstraintWidgetForm.h"
#include "ui_DoubleListWidgetForm.h"


//Qt 
#include <QWidget>


te::qt::widgets::UniqueKeyConstraintWidget::UniqueKeyConstraintWidget(te::da::DataSetType* dsType, QWidget* parent, Qt::WindowFlags f)
  : ConstraintWidget(dsType, parent, f)
{
}

te::qt::widgets::UniqueKeyConstraintWidget::~UniqueKeyConstraintWidget()
{
}

te::da::Constraint* te::qt::widgets::UniqueKeyConstraintWidget::getConstraint()
{
//get index name
  if(m_ui->m_nameLineEdit->text().isEmpty())
  {
    return 0;
  }

  std::string constraintName = m_ui->m_nameLineEdit->text().toStdString();

  //get properties
  std::vector<std::string> vec = m_doubleListWidget->getOutputValues();

  if(vec.empty())
  {
    return 0;
  }

  //create constraint
  te::da::UniqueKey* uk = new te::da::UniqueKey(m_dsType);

  uk->setName(constraintName);

  for(size_t t = 0; t < vec.size(); ++t)
  {
    te::dt::Property* p = m_dsType->getProperty(vec[t])->clone();

    uk->add(p);
  }

  return uk;
}

void te::qt::widgets::UniqueKeyConstraintWidget::setConstraint(te::da::Constraint* constraint)
{
  if(!constraint)
    return;

  te::da::UniqueKey* uk = dynamic_cast<te::da::UniqueKey*>(constraint);

  if(!uk)
    return;

  m_ui->m_nameLineEdit->setText(uk->getName().c_str());

  std::vector<te::dt::Property*> ukProps = uk->getProperties();
  std::vector<std::string> ukPropsStr;
  for(std::size_t i = 0; i < ukProps.size(); ++i)
  {
    ukPropsStr.push_back(ukProps[i]->getName());
  }

  std::vector<te::dt::Property*> dtProps = m_dsType->getProperties();
  std::vector<std::string> dtPropsStr;
  for(std::size_t i = 0; i < dtProps.size(); ++i)
  {
    std::string propStr = dtProps[i]->getName();

    if(std::find(ukPropsStr.begin(), ukPropsStr.end(), propStr) != ukPropsStr.end())
      continue;

    dtPropsStr.push_back(propStr);
  }

  m_doubleListWidget->setInputValues(dtPropsStr);
  m_doubleListWidget->setOutputValues(ukPropsStr);

}