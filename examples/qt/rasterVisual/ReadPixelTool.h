/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file ReadPixelTool.h

  \brief An example of MapDisplay Tool. The only purpose of this tool is to show how you can implement a new tool. Do not consider it as a final application.
*/

#ifndef __TERRALIB_EXAMPLES_QT_TOOLS_INTERNAL_TOOLS_READPIXEL_TOOL_H
#define __TERRALIB_EXAMPLES_QT_TOOLS_INTERNAL_TOOLS_READPIXEL_TOOL_H

// TerraLib
#include <terralib/qt/widgets/tools/AbstractTool.h>


//STL
#include <vector>

// Forward declarations
namespace te
{
  namespace rst
  {
    class Raster;
  }

  namespace map
  {
    class RasterLayer;
  }

  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;
    }
  }
}

class ReadPixelTool : public te::qt::widgets::AbstractTool
{
  Q_OBJECT

  public:

    // * Under revision *
    ReadPixelTool(te::qt::widgets::MapDisplay* display, /*te::map::RasterLayer* layer,*/ QObject* parent = 0);

    ~ReadPixelTool();

    /** @name AbstractTool Methods
      *  Methods related with tool behavior.
      */
    //@{

    bool mouseReleaseEvent(QMouseEvent* e);

    //@}

  private:

    //te::map::RasterLayer* m_layer; // * Under revision *
};

#endif  // __TERRALIB_EXAMPLES_QT_TOOLS_INTERNAL_TOOLS_SELECTION_TOOL_H
