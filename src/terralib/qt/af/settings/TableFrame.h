/*  Copyright (C) 2011-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/af/settings/TableFrame.h

  \brief A frame for setting Table options.
*/

#ifndef __TERRALIB_QT_AF_TABLEFRAME_H
#define __TERRALIB_QT_AF_TABLEFRAME_H

// TerraLib
#include "../Config.h"
#include "ComponentWidget.h"

namespace Ui { class TableFrameForm; }

namespace te
{
  namespace qt
  {
    namespace af
    {
      /*!
        \class TableFrame

        \brief A frame for setting Table options.
      */
      class TEQTAFEXPORT TableFrame : public ComponentWidget
      {
        Q_OBJECT

        public:

          TableFrame(QWidget* parent = 0);

          ~TableFrame();

        private:

          std::auto_ptr<Ui::TableFrameForm> m_ui;

      };

    }   // end namespace af
  }     // end namespace qt
}       // end namespace te

#endif // __TERRALIB_QT_AF_TABLEFRAME_H

