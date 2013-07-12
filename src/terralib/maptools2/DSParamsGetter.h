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
  \file DSParamsGetter.h
  \brief Interface for components that can return informations for open data sources.
  \details There are rules for the composition of the connection string of the some data source, more details in te::da::DataSourceFactory documentation. 
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_DSPARAMSGGETTER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_DSPARAMSGGETTER_H

//! STL include files
#include <map>
#include <string>

namespace te
{
  namespace map
  {
    /*!
      \class DSParamsGetter
      \copydoc DSParamsGetter.h
    */
    class DSParamsGetter
    {
    public:

      /*!
        \brief Returns a map with the informations for connecting a data source.
        \return A map with informations for connecting some data source.
      */
      virtual std::map<std::string, std::string> getParams() const = 0;
    };
  }
}

#endif //! __TERRALIB_MAPTOOLS_INTERNAL_DSPARAMSGGETTER_H
