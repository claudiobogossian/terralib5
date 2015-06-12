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
  \file GeometryOperands.h
  
  \brief Geometry operands.  
 */

#ifndef __TERRALIB_FE_INTERNAL_GEOMETRYOPERANDS_H
#define __TERRALIB_FE_INTERNAL_GEOMETRYOPERANDS_H

// TerraLib
#include "../common/Comparators.h"
#include "Config.h"

// STL
#include <vector>
#include <set>

namespace te
{
  namespace fe
  {
    /*!
      \class GeometryOperands
      
      \brief Geometry operands.

      The geometry operands can be:
      <ul>
      <li>gml:Envelope</li>
      <li>gml:Point</li>
      <li>gml:LineString</li>
      <li>gml:Polygon</li>
      <li>gml:ArcByCenterPoint</li>
      <li>gml:CircleByCenterPoint</li>
      <li>gml:Arc</li>
      <li>gml:Circle</li>
      <li>gml:ArcByBulge</li>
      <li>gml:Bezier</li>
      <li>gml:Clothoid</li>
      <li>gml:CubicSpline</li>
      <li>gml:Geodesic</li>
      <li>gml:OffsetCurve</li>
      <li>gml:Triangle</li>
      <li>gml:PolyhedralSurface</li>
      <li>gml:TriangulatedSurface</li>
      <li>gml:Tin</li>
      <li>gml:Solid</li>
      </ul>

      \sa SpatialCapabilities, SpatialOperator
     */
    class TEFEEXPORT GeometryOperands
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new GeometryOperands. */
        GeometryOperands();

        /*! \brief Destructor. */
        ~GeometryOperands();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the number of available geometry operands in the object list.

          \return The number of available geometry operands in the object list.

          \warning This is not the same as the all available in the system! This way you can limit in your service the geometry operands.
         */
        size_t size() const;

        /*!
          \brief It adds the geometry operand to the supported list of operands.
         
          \param g The geometry operand to be added to the supported list of operands.
         */
        void push_back(const char* g);        

        /*!
          \brief It returns a specified geometry operand.

          \param i The index of desired geometry operand.

          \return A specified geometry operand.

          \note You must not clear the returned geometry operand.

          \note The method will not check the index range.
         */
        const char* operator[](size_t i) const;

        /*! \brief It checks if operator 'o' is a valid operator. It returns a pointer
                   to the found operator or NULL otherwise.

             \param o The operator name we are looking for.
         */
        static const char* findOperand(const char* o);

        /*! \brief It loads the valid geometry operands list. */
        static void loadValidGeometryOperandsList();

        /*! \brief It clears the valid geometry operands list. */
        static void clearValidGeometryOperandsList();

        //@}

      private:

        std::vector<const char*> m_operands; //!< A vector of const pointer to geometry operand type name. Mandatory at least one operand type.

        static std::set<const char*, te::common::LessCmp<const char*> >  sm_validGeometryOperands; //!< The list of valid bynary geometry operands.
    };    

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_GEOMETRYOPERANDS_H

