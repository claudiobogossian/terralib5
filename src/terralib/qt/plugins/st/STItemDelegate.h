/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.

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


// TerraLib
#include "../../../common/Decorator.h"

// Qt
#include <QIcon>
#include <QStyledItemDelegate>

namespace te
{
  namespace map
  {
    // Forward declarations
    class AbstractLayer;
  }

  namespace qt
  {
    namespace plugins
    {
      namespace st
      {
        class STItemDelegate : public te::common::Decorator<QStyledItemDelegate>
        {

        public:
          STItemDelegate(QStyledItemDelegate* decorated, QObject* parent = 0);

          ~STItemDelegate();

          void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

        protected:

          QIcon getIcon(const te::map::AbstractLayer* l) const;

          QIcon m_obsIcon;
          QIcon m_tmsIcon;
          QIcon m_trjIcon;
        };
      }
    }
  }
}
