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
  \file terralib/se/LinePlacement.h

  \brief A LinePlacement specifies how a text label should be rendered relative to a linear geometry.
*/

#ifndef __TERRALIB_SE_INTERNAL_LINEPLACEMENT_H
#define __TERRALIB_SE_INTERNAL_LINEPLACEMENT_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace se
  {
// Forward declaration
    class ParameterValue;

    /*!
      \class LinePlacement
      
      \brief A LinePlacement specifies how a text label should be rendered relative to a linear geometry.

      For a LinePlacement, a perpendicular offset can be specified,
      to allow the line itself to be plotted also. This might be
      useful for labelling a road or a river, for example. The default
      behaviour of LinePlacement is to draw of the label along the line.
      If IsRepeated is "true", the label will be repeatedly drawn along
      the line with InitialGap and Gap defining the spaces at the
      beginning and between labels. GeneralizeLine allows the actual
      geometry, be it a linestring or polygon to be generalized for
      label placement. This is e.g. useful for labelling polygons inside
      their interior when there is need for the label to resemble the
      shape of the polygon.

      \sa LabelPlacement, ParameterValue
    */
    class TESEEXPORT LinePlacement : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new LinePlacement. */
        LinePlacement();

        /*! \brief Destructor. */
        ~LinePlacement();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setPerpendicularOffset(ParameterValue* p);

        const ParameterValue* getPerpendicularOffset() const { return m_perpendicularOffset; }

        void setIsRepeated(bool i);

        bool isRepeated() const { return m_isRepeated; }

        void setInitialGap(ParameterValue* p);

        const ParameterValue* getInitialGap() const { return m_gap; }

        void setGap(ParameterValue* p);

        const ParameterValue* getGap() const { return m_gap; }

        void setIsAligned(bool i);

        bool isAligned() const { return m_isAligned; }

        void setGeneralizeLine(bool g);

        bool getGeneralizeLine() const { return m_generalizeLine; }

        //@}

        /*! \brief It creates a new copy of this object. */
        LinePlacement* clone() const;

      private:

        ParameterValue* m_perpendicularOffset; //!< A perpendicular offset gives the perpendicular distance away from a line to draw a label. (Optional)
        bool m_isRepeated;                     //!< If IsRepeated is "true", the label will be repeatedly drawn along the line with m_initialGap and m_gap defining the spaces at the beginning and between labels. (Optional)
        ParameterValue* m_initialGap;          //!< Initial gap defines the initial empty space, before the first Graphic or Label should be rendered. (Optional)
        ParameterValue* m_gap;                 //!< Gap defines the empty space between two Graphics or Labels. (Optional)
        bool m_isAligned;                      //!< Labels can either be aligned to the line geometry if m_isAligned is "true" (the default) or are drawn horizontally. (Optional)
        bool m_generalizeLine;                 //!< It allows the actual geometry, be it a linestring or polygon, to be generalized for label placement. This is e.g. useful for labelling polygons inside their interior when there is need for the label to resemble the shape of the polygon. (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_LINEPLACEMENT_H
