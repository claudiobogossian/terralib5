/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*! 
  \file terralib/qt/widgets/InterfaceController.h

  \brief A virtual class for non modal interfaces
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_INTERFACECONTROLLER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_INTERFACECONTROLLER_H

// Terralib
#include "../../../maptools/AbstractLayer.h"
#include "Config.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class InterfaceController

        \brief A virtual class for non modal interfaces

      */
      class TEQTWIDGETSEXPORT InterfaceController
      {
        public:

          /*!
            \brief Constructor.

          */
          InterfaceController();

          /*! \brief Destructor. */
          ~InterfaceController();

        public:

          /*!
            \brief This method is used to set current layer
            
          */
          virtual void layerSelected(te::map::AbstractLayerPtr layer) = 0;

          /*!
            \brief This method is used to add a new layer
            
          */
          virtual void layerAdded(te::map::AbstractLayerPtr layer) = 0;

          /*!
            \brief This method is used to remove a layer
            
          */
          virtual void layerRemoved(te::map::AbstractLayerPtr layer) = 0;

      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif // __TERRALIB_QT_WIDGETS_INTERNAL_INTERFACECONTROLLER_H

