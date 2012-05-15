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
  \file terralib/common/MatrixUtils.h

  \brief Matrix manipulation utils.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_MATRIXUTILS_H
#define __TERRALIB_COMMON_INTERNAL_MATRIXUTILS_H

// TerraLib
#include "Config.h"

// STL
#include <cassert>

// Boost
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace te
{
  namespace common
  {
    /*!
      \brief Matrix inversion.
      
      \param inputMatrix Input matrix.
      
      \param outputMatrix Output matrix.

      \return true if ok, false on errors.
    */
    template<class T>
    bool getInverseMatrix(const boost::numeric::ublas::matrix<T>& inputMatrix,
                          boost::numeric::ublas::matrix<T>& outputMatrix)
    {
      assert( inputMatrix.size1() == inputMatrix.size2() );

      // create a working copy of the input
      boost::numeric::ublas::matrix<T> A( inputMatrix );
      
      // create a permutation matrix for the LU-factorization
      boost::numeric::ublas::permutation_matrix<std::size_t> pm( A.size1() );
      
      // perform LU-factorization
      if( boost::numeric::ublas::lu_factorize( A, pm ) )
      {
        return false;
      }
      else
      {
        // create identity matrix of "inverse"
        outputMatrix = boost::numeric::ublas::identity_matrix<T>( A.size1() );

        // backsubstitute to get the inverse
        boost::numeric::ublas::lu_substitute(A, pm, outputMatrix );
      
        return true;
      }
    }


  }     // end namespace common
}       // end namespace te

#endif  //__TERRALIB_COMMON_INTERNAL_MATRIXUTILS_H
