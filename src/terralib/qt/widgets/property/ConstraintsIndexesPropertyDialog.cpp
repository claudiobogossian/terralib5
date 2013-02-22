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
  \file terralib/qt/widgets/property/ConstraintsIndexesPropertyDialog.cpp

  \brief A class used to define a constraint or index property creator.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "ConstraintsIndexesPropertyDialog.h"
#include "IndexWidget.h"
#include "PrimaryKeyConstraintWidget.h"
#include "UniqueKeyConstraintWidget.h"
#include "ui_ConstraintsIndexesPropertyDialogForm.h"


te::qt::widgets::ConstraintsIndexesPropertyDialog::ConstraintsIndexesPropertyDialog(const te::da::DataSetTypePtr& dsType, QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ConstraintsIndexesPropertyDialogForm),
    m_pkConstraintWidget(0),
    m_ukConstraintWidget(0),
    m_indexWidget(0)
{
  m_ui->setupUi(this);

  m_dsType = dsType;

  //set layout
  m_widgetLayout = new QGridLayout(m_ui->m_widget);

  //connects
  connect(m_ui->m_pkRadioButton, SIGNAL(toggled(bool)), this, SLOT(on_pkRadioButtonToggled(bool)));
  connect(m_ui->m_ukRadioButton, SIGNAL(toggled(bool)), this, SLOT(on_ukRadioButtonToggled(bool)));
  connect(m_ui->m_idxRadioButton, SIGNAL(toggled(bool)), this, SLOT(on_idxRadioButtonToggled(bool)));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(on_okPushButtonClicked()));
}

te::qt::widgets::ConstraintsIndexesPropertyDialog::~ConstraintsIndexesPropertyDialog()
{
}

te::da::Index* te::qt::widgets::ConstraintsIndexesPropertyDialog::getIndex()
{
  if(m_indexWidget)
    return m_indexWidget->getIndex();

  return 0;
}

te::da::Constraint* te::qt::widgets::ConstraintsIndexesPropertyDialog::getConstraint()
{
  if(m_pkConstraintWidget)
    return m_pkConstraintWidget->getConstraint();

  if(m_ukConstraintWidget)
    return m_ukConstraintWidget->getConstraint();

  return 0;
}

void te::qt::widgets::ConstraintsIndexesPropertyDialog::on_pkRadioButtonToggled(bool status)
{
  if(status == false)
    return;

  freeWidget();

  m_pkConstraintWidget = new te::qt::widgets::PrimaryKeyConstraintWidget(m_dsType, m_ui->m_widget);

  m_widgetLayout->addWidget(m_pkConstraintWidget);
}

void te::qt::widgets::ConstraintsIndexesPropertyDialog::on_ukRadioButtonToggled(bool status)
{
  if(status == false)
    return;

  freeWidget();

  m_ukConstraintWidget = new te::qt::widgets::UniqueKeyConstraintWidget(m_dsType, m_ui->m_widget);

  m_widgetLayout->addWidget(m_ukConstraintWidget);
}

void te::qt::widgets::ConstraintsIndexesPropertyDialog::on_idxRadioButtonToggled(bool status)
{
  if(status == false)
    return;

  freeWidget();

  m_indexWidget = new te::qt::widgets::IndexWidget(m_dsType, m_ui->m_widget);

  m_widgetLayout->addWidget(m_indexWidget);
}

void te::qt::widgets::ConstraintsIndexesPropertyDialog::on_okPushButtonClicked()
{
  if(m_ui->m_pkRadioButton->isChecked())
  {
    if(m_pkConstraintWidget->checkParameters() == false)
      return;
  }
  else if(m_ui->m_ukRadioButton->isChecked())
  {
    if(m_ukConstraintWidget->checkParameters() == false)
      return;
  }
  else if(m_ui->m_idxRadioButton->isChecked())
  {
    if(m_indexWidget->checkParameters() == false)
      return;
  }
  else
  {
    return;
  }

  accept();
}

void te::qt::widgets::ConstraintsIndexesPropertyDialog::freeWidget()
{
  if(m_indexWidget)
  {
    m_widgetLayout->removeWidget(m_indexWidget);
    delete m_indexWidget;
    m_indexWidget = 0;
  }

  if(m_pkConstraintWidget)
  {
    m_widgetLayout->removeWidget(m_pkConstraintWidget);
    delete m_pkConstraintWidget;
    m_pkConstraintWidget = 0;
  }

  if(m_ukConstraintWidget)
  {
    m_widgetLayout->removeWidget(m_ukConstraintWidget);
    delete m_ukConstraintWidget;
    m_ukConstraintWidget = 0;
  }
}
