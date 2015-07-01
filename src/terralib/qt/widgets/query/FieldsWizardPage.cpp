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
  \file terralib/qt/widgets/query/FieldsWizardPage.cpp

  \brief This file has the FieldsWizardPage class.
*/

// TerraLib
#include "../../../dataaccess/query/Field.h"
#include "../../../dataaccess/query/PropertyName.h"
#include "../utils/DoubleListWidget.h"
#include "FieldsWizardPage.h"
#include "ui_DoubleListWidgetForm.h"

// Qt
#include <QGridLayout>


te::qt::widgets::FieldsWizardPage::FieldsWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
  m_widget.reset(new DoubleListWidget(this));

  QGridLayout* layout = new QGridLayout(this);

  layout->addWidget(m_widget.get());

  //configure page
  this->setTitle(tr("Attribute Selection"));
  this->setSubTitle(tr("Select the attributes to be listed."));

  m_widget->getForm()->m_leftItemsLabel->setText(tr("Available Properties"));
  m_widget->getForm()->m_rightItemsLabel->setText(tr("Used Properties"));

  //connect slots
  connect(m_widget.get(), SIGNAL(itemChanged()), this, SLOT(onItemChanged()));
}

te::qt::widgets::FieldsWizardPage::~FieldsWizardPage()
{
}

bool te::qt::widgets::FieldsWizardPage::isComplete() const
{
  if(m_widget->getOutputValues().empty())
    return false;

  return true;
}

te::qt::widgets::DoubleListWidget* te::qt::widgets::FieldsWizardPage::getWidget() const
{
  return m_widget.get();
}

te::da::Fields* te::qt::widgets::FieldsWizardPage::getFields()
{
  te::da::Fields* fields = new te::da::Fields;

  std::vector<std::string> values = m_widget->getOutputValues();

  for(size_t t = 0; t < values.size();  ++t)
  {
    te::da::Field* f = new te::da::Field(new te::da::PropertyName(values[t]));

    fields->push_back(f);
  }

  return fields;
}

void te::qt::widgets::FieldsWizardPage::setFields(const te::da::Fields* fields)
{
  std::vector<std::string> values;

  for(std::size_t i = 0; i < fields->size(); ++i)
  {
    te::da::Field f = fields->at(i);

    te::da::PropertyName* p = dynamic_cast<te::da::PropertyName*>(f.getExpression());

    values.push_back(p->getName());
  }

  m_widget->setOutputValues(values);
}

void te::qt::widgets::FieldsWizardPage::onItemChanged()
{
  emit completeChanged();
}
