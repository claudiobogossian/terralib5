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
  \file terralib/se/PointSymbolizer.h
  
  \brief A PointSymbolizer specifies the rendering of a graphic Symbolizer at a point.
*/

#ifndef __TERRALIB_SE_INTERNAL_POINTSYMBOLIZER_H
#define __TERRALIB_SE_INTERNAL_POINTSYMBOLIZER_H

// TerraLib
#include "Symbolizer.h"

namespace te
{
  namespace fe { class PropertyName; }

  namespace se
  {
    /*!
      \class PointSymbolizer
      
      \brief A PointSymbolizer specifies the rendering of a graphic Symbolizer at a point.

      In the PointSymbolizer case, if a line, polygon, or raster geometry
      is used, then the semantic is to
      use the centroid of the geometry, or any similar
      representative point.

      \ingroup se

      \sa Symbolizer, Graphic, te::fe::PropertyName
     */
    class TESEEXPORT PointSymbolizer : public Symbolizer
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new PointSymbolizer. */
        PointSymbolizer();

        /*!
          \brief Copy constructor.

          \param rhs The other point symbolizer.
        */
        PointSymbolizer(const PointSymbolizer& rhs);

        /*! \brief Destructor */
        ~PointSymbolizer();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setGeometry(te::fe::PropertyName* geometry);

        const te::fe::PropertyName* getGeometry() const;

        void setGraphic(Graphic* graphic);

        const Graphic* getGraphic() const;

        const std::string& getType() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        Symbolizer* clone() const;

      private:

        te::fe::PropertyName* m_geometry;  //!< A Geometry gives reference to a (the) geometry property of a feature to be used for rendering. (Optional)
        Graphic* m_graphic;                //!< A Graphic specifies or refers to a graphic Symbolizer with inherent shape, size, and coloring. (Optional)

        static const std::string sm_type;  //!< A static data member used in the implementation of getType method.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_POINTSYMBOLIZER_H

