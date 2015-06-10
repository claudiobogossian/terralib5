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
  \file terralib/qt/widgets/tools/ZoomClick.h

  \brief This class implements a concrete tool to geographic zoom operation using the mouse click.
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_VIEWZOOMCLICK_H
#define __TERRALIB_LAYOUT_INTERNAL_VIEWZOOMCLICK_H

// TerraLib
#include "ViewZoom.h"

namespace te
{
  namespace layout
  {
    // Forward declarations
    class View;

      /*!
        \class ViewZoomClick

        \brief This class implements a concrete tool to geographic zoom operation using the mouse click.

        \ingroup widgets
      */
      class TELAYOUTEXPORT ViewZoomClick : public ViewZoom
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a zoom click tool associated with the given map display.

            \param display The map display associated with the tool.
            \param cursor The tool cursor.
            \param zoomFactor The factor used to zoom. i.e. A factor value of 2.0 (default) will generate a new extent twice (%) bigger or smaller.
            \para zoomType The zoom operation type - ZoomIn | ZoomOut.
            \param parent The tool's parent.

            \note The tool will NOT take the ownership of the given pointers.
          */
          ViewZoomClick(View* view, const QCursor& cursor, const double& zoomFactor = 2.0, const ViewZoomType& type = TypeIn, QObject* parent = 0);

          /*! \brief Destructor. */
          ~ViewZoomClick();

          //@}

          /** @name AbstractTool Methods
           *  Methods related with tool behavior.
           */
          //@{

          bool mousePressEvent(QMouseEvent* e);

          //@}
      };
  }   // end namespace layout
}     // end namespace te

#endif  
