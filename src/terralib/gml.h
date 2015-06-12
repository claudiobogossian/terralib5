/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file gml.h
  
  \brief This file contains include headers for TerraLib GML module.
 */
#ifndef __TERRALIB_INTERNAL_GML_H
#define __TERRALIB_INTERNAL_GML_H

// TerraLib
#include "gml/Config.h"
#include "gml/Envelope.h"

namespace te
{
  /*!
	  \brief This is the namespace for the GML module.

    The OpenGIS Geography Markup Language Encoding Standard (GML
    is an XML grammar for expressing geographical features. GML serves
    as a modeling language for geographic systems as well as an open
    interchange format for geographic transactions on the Internet.
    <br>
    As with most XML based grammars, there are two parts to the
    grammar – the schema that describes the document and the instance
    document that contains the actual data.
    <br>
    A GML document is described using a GML Schema.
    This allows users and developers to describe generic geographic
    data sets that contain points, lines and polygons. However, the
    developers of GML envision communities working to define
    community-specific application schemas [en.wikipedia.org/wiki/GML_Application_Schemas]
    that are specialized extensions of GML. Using application schemas,
    users can refer to roads, highways, and bridges instead of points,
    lines and polygons. If everyone in a community agrees to use the same
    schemas they can exchange data easily and be sure that a road is still a road when they view it.
    <br>
    Clients and servers with interfaces that implement
    the OpenGIS Web Feature Service Interface
    Standard [http://www.opengeospatial.org/standards/wfs] read and write GML data.
    GML is also an ISO standard (ISO 19136:2007)
    [www.iso.org/iso/iso_catalogue/catalogue_tc/catalogue_detail.htm?csnumber=32554 ].
    <br>
    Most of the documentation used in GML classes were extracted
    from the specificaton available at: http://www.opengeospatial.org/standards/gml.
  
    \note See the te namespace comments for any detail on using namespaces.
   */
  namespace gml
  {
  } // end namespace gml
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_GML_H
