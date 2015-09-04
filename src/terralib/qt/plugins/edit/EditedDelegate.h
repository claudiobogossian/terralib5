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
 * \file
 *
 * \brief
 */
#ifndef __TERRALIB_PLUGINEDIT_INTERNAL_EDITDELEGATE_H
#define __TERRALIB_PLUGINEDIT_INTERNAL_EDITDELEGATE_H

// TerraLib
#include "../../../common/Decorator.h"

// Qt
#include <QIcon>
#include <QStyledItemDelegate>

// STL
#include <set>

class EditDelegate: public te::common::Decorator<QStyledItemDelegate>
{
  Q_OBJECT

public:

  EditDelegate(QStyledItemDelegate* decorated, QObject* parent = 0);

  ~EditDelegate();

  void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

  void addStashed(const std::string& lName);

  void removeStashed(const std::string& lName);

  void addEdited(const std::string& lName);

  void removeEdited(const std::string& lName);

  bool isStached(const std::string& lName);

protected:

  std::set<std::string> m_stashed;
  std::set<std::string> m_edited;

  QIcon m_stash;
  QIcon m_memory;
};

#endif //__TERRALIB_PLUGINEDIT_INTERNAL_EDITDELEGATE_H
