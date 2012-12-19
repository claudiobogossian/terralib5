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
  \file terralib/qt/widgets/se/SymbolInfoDialog.cpp

  \brief A dialog used to configure informations about a symbol.
*/

// TerraLib
#include "SymbolInfoDialog.h"
#include "ui_SymbolInfoDialogForm.h"

te::qt::widgets::SymbolInfoDialog::SymbolInfoDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::SymbolInfoDialogForm)
{
  m_ui->setupUi(this);
}

te::qt::widgets::SymbolInfoDialog::~SymbolInfoDialog()
{
}

te::qt::widgets::SymbolInfo te::qt::widgets::SymbolInfoDialog::getSymbolInfo() const
{
  return m_symbolInfo;
}

void te::qt::widgets::SymbolInfoDialog::setSymbolInfo(const te::qt::widgets::SymbolInfo& info)
{
  m_symbolInfo = info;
  updateUi();
}

void te::qt::widgets::SymbolInfoDialog::updateUi()
{
  m_ui->m_idLineEdit->setText(m_symbolInfo.m_id.c_str());
  m_ui->m_nameLineEdit->setText(m_symbolInfo.m_name.c_str());
  m_ui->m_authorLineEdit->setText(m_symbolInfo.m_author.c_str());
  m_ui->m_categoryLineEdit->setText(m_symbolInfo.m_category.c_str());
  m_ui->m_tagsLineEdit->setText(m_symbolInfo.m_tags.c_str());
  m_ui->m_descriptionTextEdit->setText(m_symbolInfo.m_description.c_str());
}
