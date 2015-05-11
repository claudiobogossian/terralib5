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
  \file terralib/se/Interpolate.h

  \brief The transformation of continuous values to a number of values (Interpolate function).
*/

#ifndef __TERRALIB_SE_INTERNAL_INTERPOLATE_H
#define __TERRALIB_SE_INTERNAL_INTERPOLATE_H

// TerraLib
#include "Function.h"

// STL
#include <vector>

namespace te
{
  namespace se
  {
// Forward declarations
    class InterpolationPoint;
    class ParameterValue;

    /*!
      \class Interpolate
      
      \brief The transformation of continuous values to a number of values (Interpolate function).

      The Symbology encoding specification defines
      three pre-defined functions for transforming raw data:
      <ul>
      <li><b>Categorization:</b> The transformation of continuous
                                 values to distinct values. This is
                                 for example needed to generate choropleth
                                 maps from continuous attributes. Another
                                 example would be the stepwise selection of
                                 different text heights or line widths in
                                 dependence from such an attribute;</li>
      <li><b>Interpolation:</b> Transformation of continuous values by a
                                function defined on a number of nodes. This
                                is used to adjust the value distribution of an
                                attribute to the desired distribution of a
                                continuous symbolization control variable
                                (like size, width, color, etc);</li>
      <li><b>Recoding:</b> Transformation of discrete values to any
                           other values. This is needed when integers
                           have to be translated into text or, reversely,
                           text contents into other texts or numeric values
                           or colors.</li>
      </ul>

      The Interpolation points have to be specified in
      ascending order of Data. They define a graph of
      points. LookupValues less than the Data value of the
      first InterpolationPoint are mapped to its
      corresponding Value. Accordingly, LookupValues greater than the
      Data value of the last InterpolationPoint are mapped
      to the Value of this one. LookupValues between two
      InterpolationPoints are interpolated between the
      corresponding Values.
      <br>
      Only numeric quantities are allowed for
      LookupValue and Data. Values are usually numeric
      as well. The interpolation of color-values requires
      the attribute mode="color" at the Interpolate element.

      \sa Function, ParameterValue, InterpolationPoint
    */
    class TESEEXPORT Interpolate : public Function
    {
      public:

        /*!
          \enum ModeType

          \brief It controls the ...
        */
        enum ModeType
        {
          LINEAR, /*!<  */
          COSINE, /*!<  */
          CUBIC   /*!<  */
        };

        /*!
          \enum MethodType

          \brief It controls the ...
        */
        enum MethodType
        {
          NUMERIC, /*!<  */
          COLOR    /*!<  */
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Interpolate. */
        Interpolate();

        /*!
          \brief Copy constructor.

          \param rhs The other Interpolate.
        */
        Interpolate(const Interpolate& rhs);

        /*! \brief Destructor. */
        ~Interpolate();

        /*!
          \brief It returns a clone of this object.
          
          \return A clone of this object.
        */
        virtual Interpolate* clone() const;

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setLookupValue(ParameterValue* v);

        ParameterValue* getLookupValue() const { return m_lookupValue; }

        void add(InterpolationPoint* i);

        void setModeType(ModeType t);

        const ModeType& getModeType() const { return m_mode; }

        void setMethodType(MethodType t);

        const MethodType& geMethodType() const { return m_method; }

        const std::vector<InterpolationPoint*>& getInterpolationPoints() const;

        //@}

      private:

        ParameterValue* m_lookupValue;                           //!< Mandatory.
        std::vector<InterpolationPoint*> m_interpolationPoints;  //!< Mandatory.
        ModeType m_mode;                                         //!< Mandatory.
        MethodType m_method;                                     //!< Mandatory.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_INTERPOLATE_H
