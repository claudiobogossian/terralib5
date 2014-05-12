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
  \file terralib/se/SymbolizerColorFinder.h
  
  \brief A Symbology Enconding visitor that finds a color given a symbolizer.
*/

#ifndef __TERRALIB_SE_INTERNAL_SYMBOLIZERCOLORFINDER_H
#define __TERRALIB_SE_INTERNAL_SYMBOLIZERCOLORFINDER_H

// TerraLib
#include "Config.h"
#include "Visitor.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace color { class RGBAColor; }
  namespace se
  {
    class Fill;
    class Graphic;
    class GraphicStroke;
    class Mark;
    class Stroke;

    /*!
      \class SymbolizerColorFinder
      
      \brief A Symbology Enconding visitor that finds a color given a symbolizer element.
             If you want to use this visitor, use a command like:
             <code>
             te::se::SymbolizerColorFinder scf;
             scf.find(symbolizer);
             te::color::RGBAColor color = scf.getColor();
             </code>

      \sa te::se::Visitor
    */
    class TESEEXPORT SymbolizerColorFinder : public te::se::Visitor, public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         */
        //@{

        /*! \brief Constructor. */
        SymbolizerColorFinder();

        /*! \brief Destructor. */
        ~SymbolizerColorFinder();

        //@}

        /** @name Find Method
         *  Method that can be used to find a color based on Symbology Enconding symbolizer elements.
         */
        //@{

        /*!
          \brief It find the color based on given symbolizer.

          \param symbolizer The symbolizer that will be used searched.
        */
        void find(const te::se::Symbolizer* symbolizer);

         /*!
          \brief Get the color.

          \return  Color found given a symbolizer
        */
        te::color::RGBAColor getColor() { return m_color; }

        //@}

        /** @name Visitor Methods
         *  All concrete visitors must implement these methods.
         */
        //@{

        void visit(const te::se::Style& visited);
        void visit(const te::se::FeatureTypeStyle& visited);
        void visit(const te::se::CoverageStyle& visited);
        void visit(const te::se::Symbolizer& visited);
        void visit(const te::se::PolygonSymbolizer& visited);
        void visit(const te::se::LineSymbolizer& visited);
        void visit(const te::se::PointSymbolizer& visited);
        void visit(const te::se::TextSymbolizer& visited);
        void visit(const te::se::RasterSymbolizer& visited);

        //@}

      private:

        /*!
          \brief Internal method that finds the color based on given Stroke element. 
          
          \param stroke  The Stroke element that will be used to find the color.
        */
        void find(const te::se::Stroke* stroke);

        /*!
          \brief Internal method that finds the color based on given Fill element. 
          
          \param fill The Fill element that will be used to find the color.
        */
        void find(const te::se::Fill* fill);

        /*!
          \brief Internal method that finds the color based on given Mark element. 
          
          \param mark The mark element that will be used to find the color.
        */
        void find(const te::se::Mark* mark);

    protected:

      te::color::RGBAColor m_color;   //!< Color found given a symbolizer

    };

  }   // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_SYMBOLIZERCOLORFINDER_H