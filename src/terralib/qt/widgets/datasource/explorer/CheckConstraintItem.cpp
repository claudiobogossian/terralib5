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
  \file terralib/qt/widgets/datasource/explorer/CheckConstraintItem.cpp

  \brief A class that represents a check-constraint in a TreeModel.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/CheckConstraint.h"
#include "../../Exception.h"
#include "CheckConstraintItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::CheckConstraintItem::CheckConstraintItem(te::da::CheckConstraint* cc, AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent),
    m_cc(cc)
{
}

te::qt::widgets::CheckConstraintItem::~CheckConstraintItem()
{
  //if((m_cc != 0) && (m_cc->getDataSetType() != 0))
  //  delete m_cc;
}

int te::qt::widgets::CheckConstraintItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::CheckConstraintItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("check"));

  if(role == Qt::DisplayRole)
    return QVariant(m_cc->getName().c_str());

  return QVariant();
}

QMenu* te::qt::widgets::CheckConstraintItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenCheckConstraint = m->addAction(tr("&Open check-constraint"));

  connect(aOpenCheckConstraint, SIGNAL(triggered()), this, SLOT(openCheckConstraint()));

  return m;
}

bool te::qt::widgets::CheckConstraintItem::canFetchMore() const
{
  return false;
}

Qt::ItemFlags te::qt::widgets::CheckConstraintItem::flags() const
{
  return Qt::NoItemFlags;
}

void te::qt::widgets::CheckConstraintItem::fetchMore()
{
}

bool te::qt::widgets::CheckConstraintItem::hasChildren() const
{
  return false;
}

bool te::qt::widgets::CheckConstraintItem::setData(const QVariant& /*value*/, int /*role*/)
{
  return false;
}

te::da::CheckConstraint* te::qt::widgets::CheckConstraintItem::getCheckConstraint() const
{
  return m_cc;
}

