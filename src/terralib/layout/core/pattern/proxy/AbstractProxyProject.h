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
  \file AbstractProxyProject.h
   
  \brief Abstract class to provide a surrogate or placeholder for te::qt::af::Project to control access to it. 
    A wrapper to access without complexity. This abstract proxy is required because module not must have dependence te::qt::af.
    Useful to access the Layers belonging to the project.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_PROXY_PROJECT_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_PROXY_PROJECT_H

// TerraLib
#include "../../Config.h"
#include "../../../../maptools/AbstractLayer.h"

#include <list>

namespace te
{
  namespace layout
  {
    /*!
    \brief Abstract class to provide a surrogate or placeholder for te::qt::af::Project to control access to it. 
      A wrapper to access without complexity. This abstract proxy is required because module not must have dependence te::qt::af.
      Useful to access the Layers belonging to the project.
    
    \ingroup layout
    */
    class TELAYOUTEXPORT AbstractProxyProject
    {
      public:
       
        /*!
          \brief Constructor
        */
        AbstractProxyProject();

        /*!
          \brief Destructor
        */
        virtual ~AbstractProxyProject();

        /*!
          \brief It gets all the layers (single and folder layers) of the project.

          \param invalid If including invalid layers.

          \return The list of all the layers (single and folder layers) of the project.
        */
        virtual std::list<te::map::AbstractLayerPtr> getAllLayers(bool invalid = true) = 0;

        /*!
          \brief It gets all the layers that are selected.

          \param invalid If including invalid layers.

          \return The list of all the layers that are selected.
        */
        virtual const std::list<te::map::AbstractLayerPtr> getSelectedLayers(bool invalid = true) const = 0;

        /*!
            \brief Checks whether the layer is in the project.

            \param name layer name to check

            \return pointer to the layer found or null
         */
        virtual te::map::AbstractLayerPtr contains(std::string name) = 0;
    };
  }
}

#endif
