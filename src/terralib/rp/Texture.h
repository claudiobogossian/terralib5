/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/rp/Texture.h
 
  \brief A structure to hold the set of GLCM metrics.
*/

#ifndef __TERRALIB_RP_INTERNAL_TEXTURE_H
#define __TERRALIB_RP_INTERNAL_TEXTURE_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/numeric/ublas/matrix.hpp>

namespace te
{
  namespace rp
  {
    /*!
      \struct Texture

      \brief A structure to hold the set of GLCM metrics.
    */
    struct TERPEXPORT Texture
    {
      public:

        /*! \brief Constructor. */
        Texture();

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        Texture(const Texture& rhs);

        /*! \brief Destructor. */
        ~Texture();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        Texture& operator=(const Texture& rhs);

        /*! \brief Clear the structure. */
        void clear();

      public:

        double m_contrast;        //!< GLCM metric Contrast (also called Sum of Squares Variance) $\sum_{i,j=0}^N-1P_{i,j}(i-j)^2$.
        double m_dissimilarity;   //!< GLCM metric Dissimilarity $\sum_{i,j=0}^{N-1}P_{i,j}|i-j|$.
        double m_energy;          //!< GLCM metric Energy (the square root of Angular Second Moment) $\sqrt{\sum_{i,j=0}^{N-1}P_{i,j}^2}$.
        double m_entropy;         //!< GLCM metric Entropy $\sum_{i,j=0}^{N-1}P_{i,j}(-\ln{P_{i,j}})$.
        double m_homogeneity;     //!< GLCM metric Homogeneity (also called Inverse Difference Moment) $\sum_{i,j=0}^{N-1}\frac{P_{i,j}}{1+(i-j)^2}$.
    };

  } // end namespace rp
} // end namespace te

#endif // __TERRALIB_RP_INTERNAL_TEXTURE_H
