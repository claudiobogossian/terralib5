/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file widgets.h

  \brief This file contains include headers for the TerraLib Qt widgets.
 */

#ifndef __TERRALIB_QT_INTERNAL_WIDGETS_H
#define __TERRALIB_QT_INTERNAL_WIDGETS_H

// TerraLib
#include "widgets/Config.h"
#include "widgets/canvas/Canvas.h"
#include "widgets/canvas/MapDisplay.h"
#include "widgets/datagrid/DataGridItemDelegate.h"
#include "widgets/datagrid/DataGridModel.h"
#include "widgets/datagrid/DataGridView.h"
#include "widgets/datagrid/HeaderView.h"
#include "widgets/layer/explorer/AbstractLayerTreeItem.h"
#include "widgets/layer/explorer/LayerExplorer.h"

namespace te
{
  namespace qt
  {
    /*!
      \brief Namespace for the TerraLib customized Qt Widgets.

      In this namespace you will find just Qt components. Some of them
      have a dialog window others are just cusmozied controls (like lists, buttons and menus).
    */
    namespace widgets
    {
    }
  } // end namespace qt
}   // end namespace te

#endif  // __TERRALIB_QT_INTERNAL_WIDGETS_H

