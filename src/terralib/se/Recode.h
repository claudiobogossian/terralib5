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
  \file terralib/se/Recode.h

  \brief Transformation of discrete values to other values.
*/

#ifndef __TERRALIB_SE_INTERNAL_RECODE_H
#define __TERRALIB_SE_INTERNAL_RECODE_H

// TerraLib
#include "Function.h"

namespace te
{
  namespace se
  {
// Forward declaration
    class MapItem;
    class ParameterValue;

    /*!
      \class Recode
      
      \brief Transformation of discrete values to other values.

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
      It allows to replace a color in
      the ExternalGraphic, the color specified in
      the OriginalColor sub-element, by another
      color as a result of a recode function.

      \sa Function, ExternalGraphic, MapItem, ParameterValue
    */
    class TESEEXPORT Recode : public Function
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Recode. */
        Recode();

        /*! \brief Destructor. */
        ~Recode();

        /*!
        \brief Copy constructor.

        \param rhs The other Categorize.
        */
        Recode(const Recode& rhs);

        /*!
        \brief It returns a clone of this object.

        \return A clone of this object.
        */
        virtual Recode* clone() const;

        //@}

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        void setLookupValue(ParameterValue* v);
        void add(MapItem* m);

        std::vector<MapItem*> getMapItems();

        //@}

      private:

        ParameterValue* m_lookupValue;     //!< Mandatory.
        std::vector<MapItem*> m_mapItems;  //!< Mandatory.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_RECODE_H
