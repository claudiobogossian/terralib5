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
  \file terralib/se/Symbolizer.h
  
  \brief A Symbolizer describes how a Feature is to appear on a map.
*/

#ifndef __TERRALIB_SE_INTERNAL_SYMBOLIZER_H
#define __TERRALIB_SE_INTERNAL_SYMBOLIZER_H

// TerraLib
#include "../common/BaseVisitable.h"
#include "Config.h"
#include "Enums.h"
#include "Visitor.h"

// STL
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
// Forward declaration
  namespace common { class BaseUnitOfMeasure; }
  namespace xl { class SimpleLink; }

  namespace se
  {
// Forward declaration
    class Description;

    /*! 
      \class Symbolizer
      
      \brief A Symbolizer describes how a feature is to appear on a map.

      A Symbolizer is an abstract type for encoding
      the graphical properties used to portray geographic information.
      Concrete Symbolizers are derived from this base type.
      A Symbolizer describes how a feature is to appear
      on a map. The Symbolizer describes not just the shape
      that should appear but also such graphical properties
      as color and opacity. A Symbolizer is obtained by
      specifying one of a small number of different types
      of Symbolizers and then supplying parameters to override
      its default behaviour. Five types of Symbolizers are defined:
      <ol>
      <li>Line</li>
      <li>Polygon</li>
      <li>Point</li>
      <li>Text</li>
      <li>Raster</li>
      </ol>
      Concrete Symbolizer types are derived from this base type.

      \ingroup se

      \sa Rule, LineSymbolizer, PointSymbolizer, PolygonSymbolizer, TextSymbolizer, RasterSymbolizer, Description
    */
    class TESEEXPORT Symbolizer : public te::common::BaseVisitable<Visitor>, public boost::noncopyable
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Symbolizer. */
        Symbolizer();

        /*!
          \brief Copy constructor.

          \param rhs The other symbolizer.
        */
        Symbolizer(const Symbolizer& rhs);

        /*! \brief Virtual destructor. */
        virtual ~Symbolizer();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setName(const std::string& name);

        const std::string& getName() const;

        void setDescription(Description* d);

        const Description* getDescription() const;

        void setBaseSymbolizer(te::xl::SimpleLink* baseSymbolizer);

        const te::xl::SimpleLink* getBaseSymbolizer() const;

        void setVersion(const std::string& version);

        const std::string& getVersion() const;

        /*!
          \brief It sets the unit-of-measure.

          All Symbolizers include an optional gml:uom-attribute as
          used by GML (this is set inside the abstract SymbolizerType
          and therefore inherited by all Symbolizers). This applies
          to all elements included inside a Symbolizer such as
          stroke-width, size, font-size, Gap, InitialGap, Displacement
          and PerpendicularOffset. If no uom is set inside of Symbolizer,
          all units are measured in pixel, the behaviour used by SLD 1.0.0.
          The following uom definitions are recommended to be used:
          <ul>
          <li>http://www.opengeospatial.org/se/units/metre</li>
          <li>http://www.opengeospatial.org/se/units/foot</li>
          <li>http://www.opengeospatial.org/se/units/pixel</li>
          </ul>
        */
        void setUom(const te::common::BaseUnitOfMeasure* uom);

        const te::common::BaseUnitOfMeasure* getUom() const;

        /*!
          \brief It returns the symbolizer type.

          \return The symbolizer type.
        */
        virtual const std::string& getType() const = 0;

        //@}

         /*! \brief It creates a new copy of this object. */
        virtual Symbolizer* clone() const = 0;

      private:

        std::string m_name;                         //!< It allows the symbolizer to be referenced externally, which is needed in some methods of SE usage. (Optional)
        Description* m_description;                 //!< It gives the familiar short title for display lists and longer description for the symbolizer. (Optional)
        te::xl::SimpleLink* m_baseSymbolizer;       //!< A BaseSymbolizer (an OnlineResource) defines the default properties of a Symbolizer to be those of an external Symbolizer, which will frequently be inside of an OGC Symbolizer repository. The Symbolizer properties given in-line override the base-Symbolizer properties. (Optional)
        std::string m_version;                      //!< The version is an optional attribute on the FeatureType Style element that identifies the SE version number that the FeatureTypeStyle corresponds to. For now, the only allowed value is 1.1.0. (Mandatory)
        const te::common::BaseUnitOfMeasure* m_uom; //!< Unit of measurement. If no uom is set inside of Symbolizer, all units are measured in pixel. (Mandatory)
    };    

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_SYMBOLIZER_H

