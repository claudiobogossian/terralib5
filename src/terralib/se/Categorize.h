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
  \file terralib/se/Categorize.h

  \brief The transformation of continuous values to distinct values (Categorize function).
*/

#ifndef __TERRALIB_SE_INTERNAL_CATEGORIZE_H
#define __TERRALIB_SE_INTERNAL_CATEGORIZE_H

// TerraLib
#include "Function.h"

// STL
#include <vector>

namespace te
{
  namespace se
  {
// Forward declaration
    class ParameterValue;

    /*!
      \class Categorize
      
      \brief The transformation of continuous values to distinct values (Categorize function).

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
      <br>
      The Thresholds have to be specified in ascending order
      and (like the LookupValue) have to be of a uniform and
      orderable type. The value of the function is determined
      by looking up into which interval between two thresholds
      the LookupValue falls. The first interval ranges from
      -Infinity to the first given threshold and the last one
      accordingly from the last threshold to +Infinity.
      In case the Categorize (or Interpolate) function is used
      inside a RasterSymbolizer as a ColorMap, the LookupValue
      is set to the fixed value "Rasterdata".
      The Values can be of any type, dependent on which symbolization
      context the function is employed. Color values (like #00ffff)
      or numeric values are typical.
      Whether the Threshold values themselves belong to the preceding
      or the succeeding interval can be controlled by the attribute
      thresholdsBelongTo= with the possible values "preceding" and
      "succeeding" the latter being the default.

      \sa Function, ParameterValue
    */
    class TESEEXPORT Categorize : public Function
    {
      public:

        /*!
          \enum ThresholdsBelongToType

          \brief It controls the interval order.
         */
        enum ThresholdsBelongToType
        {
          NONE,       /*!<  */
          SUCCEEDING, /*!<  */
          PRECEDING   /*!<  */
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Categorize. */
        Categorize();

        /*!
          \brief Copy constructor.

          \param rhs The other Categorize.
        */
        Categorize(const Categorize& rhs);
        
        /*! \brief Destructor. */
        ~Categorize();

        /*!
          \brief It returns a clone of this object.
          
          \return A clone of this object.
        */
        virtual Categorize* clone() const;

        //@}

        void setLookupValue(ParameterValue* v);

        ParameterValue* getLookupValue() const { return m_lookupValue; }

        void setValue(ParameterValue* v);

        ParameterValue* getValue() const { return m_value; }

        void addThreshold(ParameterValue* v);

        void addValue(ParameterValue* v);

        void setThresholdsBelongTo(ThresholdsBelongToType t);

        const ThresholdsBelongToType& getThresholdsBelongTo() const { return m_threshholdsBelongTo; }

        const std::vector<ParameterValue*>& getThresholds() const;

        const std::vector<ParameterValue*>& getThresholdValues() const;

      private:

        ParameterValue* m_lookupValue;                  //!< Mandatory.
        ParameterValue* m_value;                        //!< Mandatory.
        std::vector<ParameterValue*> m_thresholds;      //!< A vector of threshold. (Optional)
        std::vector<ParameterValue*> m_thresholdValues; //!< A vector of threshold values. (Optional)
        ThresholdsBelongToType m_threshholdsBelongTo;   //!< (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_CATEGORIZE_H
