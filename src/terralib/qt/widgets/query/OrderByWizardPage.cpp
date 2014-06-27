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
  \file terralib/qt/widgets/query/OrderByWizardPage.cpp

  \brief This file has the OrderByWizardPage class.
*/

// TerraLib
#include "ui_OrderByWidgetForm.h"
#include "OrderByWidget.h"
#include "OrderByWizardPage.h"

// Qt
#include <QGridLayout>


te::qt::widgets::OrderByWizardPage::OrderByWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
  m_widget.reset(new OrderByWidget(this));

  QGridLayout* layout = new QGridLayout(this);

  layout->addWidget(m_widget.get());

  //configure page
  this->setTitle(tr("Order By Definition"));
  this->setSubTitle(tr("Choose the attributes to sort the search result."));
}

te::qt::widgets::OrderByWizardPage::~OrderByWizardPage()
{
}

bool te::qt::widgets::OrderByWizardPage::isComplete() const
{
  return true;
}

te::qt::widgets::OrderByWidget* te::qt::widgets::OrderByWizardPage::getWidget() const
{
  return m_widget.get();
}
