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
  \file terralib/qt/widgets/query/GroupByWizardPage.cpp

  \brief This file has the GroupByWizardPage class.
*/

// TerraLib
#include "../../../dataaccess/query/GroupByItem.h"
#include "../../../dataaccess/query/PropertyName.h"
#include "../utils/DoubleListWidget.h"
#include "GroupByWizardPage.h"
#include "ui_DoubleListWidgetForm.h"

// Qt
#include <QGridLayout>


te::qt::widgets::GroupByWizardPage::GroupByWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
  m_widget.reset(new DoubleListWidget(this));

  QGridLayout* layout = new QGridLayout(this);

  layout->addWidget(m_widget.get());

  //configure page
  this->setTitle(tr("Group By Definition"));
  this->setSubTitle(tr("Choose the attributes to be grouped."));

  m_widget->getForm()->m_leftItemsLabel->setText(tr("Available Properties"));
  m_widget->getForm()->m_rightItemsLabel->setText(tr("Used Properties"));
}

te::qt::widgets::GroupByWizardPage::~GroupByWizardPage()
{
}

bool te::qt::widgets::GroupByWizardPage::isComplete() const
{
  return true;
}

te::qt::widgets::DoubleListWidget* te::qt::widgets::GroupByWizardPage::getWidget() const
{
  return m_widget.get();
}

te::da::GroupBy* te::qt::widgets::GroupByWizardPage::getGroupBy()
{
  std::vector<std::string> values = m_widget->getOutputValues();

  if(values.empty())
    return 0;
  

  te::da::GroupBy* groupBy = new te::da::GroupBy;

  for(size_t t = 0; t < values.size();  ++t)
  {
    te::da::GroupByItem* gi = new te::da::GroupByItem(new te::da::PropertyName(values[t]));

    groupBy->push_back(gi);
  }

  return groupBy;
}
