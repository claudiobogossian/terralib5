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
  \file terralib/qt/widgets/query/WhereClauseWizardPage.cpp

  \brief This file has the WhereClauseWizardPage class.
*/

// TerraLib
#include "ui_WhereClauseWidgetForm.h"
#include "WhereClauseWidget.h"
#include "WhereClauseWizardPage.h"

// Qt
#include <QGridLayout>


te::qt::widgets::WhereClauseWizardPage::WhereClauseWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
  m_widget.reset(new WhereClauseWidget(this));

  QGridLayout* layout = new QGridLayout(this);

  layout->addWidget(m_widget.get());

  //configure page
  this->setTitle(tr("Where Clause Definition"));
  this->setSubTitle(tr("Define the restriction clauses to search."));
}

te::qt::widgets::WhereClauseWizardPage::~WhereClauseWizardPage()
{
}

bool te::qt::widgets::WhereClauseWizardPage::isComplete() const
{
  return true;
}

te::qt::widgets::WhereClauseWidget* te::qt::widgets::WhereClauseWizardPage::getWidget() const
{
  return m_widget.get();
}
