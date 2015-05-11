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
  \file terralib/se/Visitor.h

  \brief A visitor interface for the SymbologyEncoding hierarchy.
*/

#ifndef __TERRALIB_SE_INTERNAL_VISITOR_H
#define __TERRALIB_SE_INTERNAL_VISITOR_H

// TerraLib
#include "../se_fw.h"

namespace te
{
  namespace se
  {
    /*!
      \class Visitor
      
      \brief A visitor interface for the SymbologyEncoding hierarchy.
    */
    class Visitor
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief Default constructor. */
        Visitor() {}

        /*! \brief Virtual destructor. */
        virtual ~Visitor() {}

        //@}

        /** @name Select
         *  All concrete visitors must implement these methods.
         */
        //@{

        virtual void visit(const Style& visited) = 0;
        virtual void visit(const FeatureTypeStyle& visited) = 0;
        virtual void visit(const CoverageStyle& visited) = 0;
        virtual void visit(const Symbolizer& visited) = 0;
        virtual void visit(const PolygonSymbolizer& visited) = 0;
        virtual void visit(const LineSymbolizer& visited) = 0;
        virtual void visit(const PointSymbolizer& visited) = 0;
        virtual void visit(const TextSymbolizer& visited) = 0;
        virtual void visit(const RasterSymbolizer& visited) = 0;

        //@}
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_VISITOR_H
