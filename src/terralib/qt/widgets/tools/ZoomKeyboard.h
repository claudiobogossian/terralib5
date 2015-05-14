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
  \file terralib/qt/widgets/tools/ZoomKeyboard.h

  \brief This class implements a concrete tool to geographic zoom operation using the keyboard.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_ZOOMKEYBOARD_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_ZOOMKEYBOARD_H

// TerraLib
#include "../Config.h"
#include "Zoom.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declarations
    class MapDisplay;

      /*!
        \class ZoomKeyboard

        \brief This class implements a concrete tool to geographic zoom operation using the keyboard.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT ZoomKeyboard : public Zoom
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a zoom keyboard tool associated with the given map display.

            \param display The map display associated with the tool.
            \param zoomFactor The factor used to zoom. i.e. A factor value of 2.0 (default) will generate a new extent twice (%) bigger or smaller.
            \param parent The tool's parent.

            \note The tool will NOT take the ownership of the given pointers.
          */
          ZoomKeyboard(MapDisplay* display, const double& zoomFactor = 2.0, QObject* parent = 0);

          /*! \brief Destructor. */
          ~ZoomKeyboard();

          //@}

          /** @name AbstractTool Methods
           *  Methods related with tool behavior.
           */
          //@{

          bool eventFilter(QObject* watched, QEvent* e);

          //@}
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_ZOOMKEYBOARD_H
