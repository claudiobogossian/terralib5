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
  \file terralib/qt/af/events/Enums.h

  \brief Enumerations for the TerraLib Application Framework.
*/

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_ENUMS_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_ENUMS_H

// TerraLib
#include "Event.h"

namespace te
{
  namespace qt
  {
    namespace af
    {
      namespace evt
      {
        enum
        {
          APP_CLOSED,
          TOOLBAR_ADDED,
          DISPLAY_RESIZED,
          LAYER_ADDED,
          LAYER_REMOVED,
          ITEM_OF_LAYER_REMOVED,
          LAYER_SELECTED,
          LAYER_VISIBILITY_CHANGED,
          LAYER_SELECTED_OBJECTS_CHANGED,
          HIGHLIGHT_LAYER_OBJECTS,
          LAYER_STYLE_SELECTED,
          LAYER_POPUP_ADD_ACTION,
          LAYER_POPUP_REMOVE_ACTION,
          GET_LAYER_SELECTED,
          TOOL_CHANGED,
          COORDINATE_TRACKED,
          POINT_GEOMETRIES,
          VISIBLE_BBOX_CHANGED,
          STYLE_CHANGED,
          PROJECT_ADDED,
          PROJECT_UNSAVED,
          MAP_SRID_CHANGED,
          MAP_COLOR_CHANGED,
          TABLE_ALTERNATE_COLORS_CHANGED,
          DRAW_BUTTON_CLICKED,
          ZOOM_IN_BUTTON_TOGGLED,
          ZOOM_OUT_BUTTON_TOGGLED,
          PAN_BUTTON_TOGGLED,
          SELECTION_BUTTON_TOGGLED,
          GET_MAPDISPLAY,
          DRAWING_FINISHED,
          NEW_ACTIONS_AVAILABLE,
          GET_AVAILABLE_LAYERS,
          CUSTOM = 1024
        };

      } // end namespace evt
    }   // end namespace af
  }     // end namesopace qt
}       // end namespace te

#endif  // __TERRALIB_QT_AF_EVENTS_INTERNAL_ENUMS_H

