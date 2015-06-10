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
  \file terralib/geometry/GTParameters.h

  \brief 2D Geometric transformation parameters.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GTPARAMETERS_H
#define __TERRALIB_GEOMETRY_INTERNAL_GTPARAMETERS_H

// TerraLib
#include "../common/AbstractParameters.h"
#include "Config.h"
#include "Coord2D.h"
#include "GTModelParameters.h"

// STL
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace te
{
  namespace gm
  {
    /*!
      \class GTParameters

      \brief 2D Geometric transformation parameters.
    */
    class TEGEOMEXPORT GTParameters : public te::common::AbstractParameters
    {
      public:

        /*!
          \typedef std::pair< Coord2D, Coord2D > TiePoint

          \brief Tie point type definition.
        */
        typedef std::pair< Coord2D, Coord2D > TiePoint;

        /*! \brief Constructor. */
        GTParameters();

        /*! \brief Copy constructor. */
        GTParameters( const GTParameters& rhs );

        /*! \brief Destructor. */
        ~GTParameters();

        te::common::AbstractParameters* clone() const;

        void reset() throw( te::common::Exception );

        GTParameters& operator=( const GTParameters& rhs );

        /*!
          \brief Returns a pointer the the internal specific model parameters or null if they do not exist.

          \return A pointer the the internal specific model parameters or null if they do not exist.
        */
        const GTModelParameters* getModelParameters() const
        {
          return m_modelParameters.get();
        };
        
        /*!
          \brief Set the the internal specific model parameters.

          \param modelParamsPtr The new model specific parameters.
        */
        void setModelParameters( const GTModelParameters* modelParamsPtr )
        {
          m_modelParameters.reset( modelParamsPtr ?
                                  static_cast<GTModelParameters*>(modelParamsPtr->clone()) : 0 );
        };

      public:

        std::vector< TiePoint > m_tiePoints;        //!< Tie points.
        std::vector< double > m_directParameters;   //!< Transformation numeric direct parameters.
        std::vector< double > m_inverseParameters;  //!< Transformation numeric inverse parameters.

      protected:

        std::auto_ptr< GTModelParameters > m_modelParameters; //!< Transformation model parameters.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GTPARAMETERS_H

