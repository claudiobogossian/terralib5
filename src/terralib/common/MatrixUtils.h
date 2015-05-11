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
  \file terralib/common/MatrixUtils.h

  \brief Matrix manipulation utils.

  \ingroup common
*/

#ifndef __TERRALIB_COMMON_INTERNAL_MATRIXUTILS_H
#define __TERRALIB_COMMON_INTERNAL_MATRIXUTILS_H

// TerraLib
#include "Config.h"

// STL
#include <cassert>

// Boost
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>

namespace te
{
  namespace common
  {
    /*!
      \brief Get the Matrix determinant value.

      \param inputMatrix  Input matrix.

      \return true if ok, false on errors.
    */
    template<class T>
    bool GetDeterminant(const boost::numeric::ublas::matrix<T>& inputMatrix,
      double& determinant)
    {
      if( ( inputMatrix.size1() == 0 ) || ( inputMatrix.size2() == 0 ) )
      {
        determinant = 0.0;
        return true;
      }
      
      // create a working copy of the input
      boost::numeric::ublas::matrix<T> A( inputMatrix );      
      
      const unsigned int size1 = A.size1();
      
      boost::numeric::ublas::permutation_matrix<std::size_t> pm( size1 );
      
      if( boost::numeric::ublas::lu_factorize( A, pm ) != 0.0 ) 
      {
        return false;
      } 
      else 
      {
        double pmSign = 1.0;
        for ( unsigned int pmi = 0; pmi < size1; ++pmi)
          if ( pmi != pm( pmi ) )
              pmSign *= -1.0; // swap_rows would swap a pair of rows here, so we change sign        
        
        determinant = 1.0;
        
        for( unsigned int i = 0 ; i < size1 ; i++ ) 
          determinant *= A(i,i); // multiply by elements on diagonal
          
        determinant = determinant * pmSign;
        
        return true;
      }
    }
    
    /*!
      \brief Matrix inversion.

      \param inputMatrix  Input matrix.
      \param outputMatrix Output matrix.

      \return true if ok, false on errors.
    */
    template<class T>
    bool GetInverseMatrix(const boost::numeric::ublas::matrix<T>& inputMatrix,
                          boost::numeric::ublas::matrix<T>& outputMatrix)
    {
      assert( inputMatrix.size1() == inputMatrix.size2() );

      // create a working copy of the input
      boost::numeric::ublas::matrix<T> A( inputMatrix );

      // create a permutation matrix for the LU-factorization
      boost::numeric::ublas::permutation_matrix<std::size_t> pm( A.size1() );

      // perform LU-factorization
      if( boost::numeric::ublas::lu_factorize( A, pm ) != 0 )
      {
        return false;
      }
      else
      {
        // create identity matrix of "inverse"
        outputMatrix = boost::numeric::ublas::identity_matrix<T>( A.size1() );

        // backsubstitute to get the inverse
        try
        {
          boost::numeric::ublas::lu_substitute( A, pm, outputMatrix );
        }
        catch(...)
        {
          return false;
        }

        return true;
      }
    }

    /*!
      \brief Pseudo matrix inversion.

      \param inputMatrix  Input matrix.
      \param outputMatrix Output matrix.

      \return true if ok, false on errors.

      \note PinvX = Inv( Xt * X ) * Xt, where Inv=Inverse and Xt is the transposed matrix of X.
      \note A* = inv(trasnp(A) * A) * transp(A)    (i>j)
      \note A* = transp(A) * inv(A * trasnp(A))    (i<j)
    */
    template<class T>
    bool GetPseudoInverseMatrix(const boost::numeric::ublas::matrix<T>& inputMatrix,
                                boost::numeric::ublas::matrix<T>& outputMatrix)
    {
      if( inputMatrix.size1() > inputMatrix.size2() )
      {
        boost::numeric::ublas::matrix<T> trans( boost::numeric::ublas::trans(
          inputMatrix ) );

        boost::numeric::ublas::matrix<T> aux1( boost::numeric::ublas::prod( trans,
          inputMatrix ) );

        boost::numeric::ublas::matrix<T> aux1Inv;

        if( GetInverseMatrix( aux1, aux1Inv ) )
        {
          outputMatrix = boost::numeric::ublas::prod( aux1Inv, trans );
          return true;
        }
        else
        {
          return false;
        }
      }
      else if( inputMatrix.size1() < inputMatrix.size2() )
      {
        boost::numeric::ublas::matrix<T> trans( boost::numeric::ublas::trans(
          inputMatrix ) );

        boost::numeric::ublas::matrix<T> aux1( boost::numeric::ublas::prod(
          inputMatrix, trans ) );

        boost::numeric::ublas::matrix<T> aux1Inv;

        if( GetInverseMatrix( aux1, aux1Inv ) )
        {
          outputMatrix = boost::numeric::ublas::prod( trans, aux1Inv );
          return true;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return GetInverseMatrix( inputMatrix, outputMatrix );
      }
    }

    /*!
      \brief Computes the eigenvectors of a given matrix.

      \param inputMatrix          The input matrix.
      \param eigenVectorsMatrix   The matrix where the eigenvectors will be stored.
      \param eigenValuesMatrix    The matrix where the eigenvalues will be stored.

      \return true if ok, false on errors.
    */
    template<class T>
    bool EigenVectors(const boost::numeric::ublas::matrix<T>& inputMatrix, boost::numeric::ublas::matrix<T> &eigenVectorsMatrix, boost::numeric::ublas::matrix<T> &eigenValuesMatrix)
    {
// this source code is based on TerraLib 4
      double *cov = NULL,   // covariance matrix
             *e_val = NULL, // eigenvalue matrix
             *e_vec = NULL; // eigenvector matrix

      int dim = inputMatrix.size1();

      int i, j, ij, k,
          ia, ind, l, m,
          mq, lm, ll, mm,
          ilq, imq, im, iq,
          il, lq, ilr, imr,
          jq;

      double range, fdim, anorm, anrmx,
             thr, x, y, z,
             sinx, cosx, sinx2, cosx2,
             sincs;

      if (dim <= 0)
        return false; // print invalid matrix

      eigenVectorsMatrix.resize(dim, dim);
      eigenVectorsMatrix.clear();
      for (i = 0; i < dim; i++)
        eigenVectorsMatrix(i, i) = 1.0;

      int fat = (dim * dim + dim) / 2;
      range = 1.0e-6;
      fdim = dim;

      cov = new double[fat];
      e_vec = new double[dim * dim];
      e_val = new double[fat];

      if( cov == NULL || e_vec == NULL || e_val == NULL )
        return false; // print out of memory

      k = 0;
      for (i = 0; i < dim; i++)
        for (j = 0; j <= i; j++)
          cov[k++] = inputMatrix(i, j);

      for (i = 0; i < ((dim * dim + dim) / 2); i++)
        e_val[i] = cov[i];

      iq = -dim;
      for (i = 0; i < dim; i++)
      {
        iq = iq + dim;
        for (j = 0; j < dim; j++)
        {
          ij = iq + j;
          if (i == j)
            e_vec[ij] = 1.0;
          else
            e_vec[ij] = 0.0;
        }
      }

      anorm = 0.0;

      for (j = 0; j < dim; j++)
      {
        for (i = 0; i <= j; i++)
          if (i != j)
          {
            ia = i + (j * j + j) / 2;
            anorm = anorm + e_val[ia] * e_val[ia];
          }
      }

      if (anorm > 0)
      {
        anorm = 1.414 * sqrt((double)anorm);
        anrmx = anorm * range / fdim;
        ind   = 0;
        thr   = anorm;

// all this whiles were inserted to avoid the use of "goto" statements
        bool l1 = true;
        while(l1)
        {
          thr = thr / fdim;

          bool l2 = true;
          while(l2)
          {
            l = 0;

            bool l3 = true;
            while(l3)
            {
              m = l + 1;

              bool l4 = true;
              while(l4)
              {
                mq = (m * m + m) / 2;
                lq = (l * l + l) / 2;
                lm = l + mq;

                if (fabs((double)(e_val[lm])) >= thr)
                {
                  ind = 1;
                  ll = l + lq;
                  mm = m + mq;

                  x = 0.5 * (e_val[ll] - e_val[mm]);
                  z = e_val[lm] * e_val[lm] + x * x;
                  y = - e_val[lm] / sqrt((double)(z));

                  if (x < 0)
                    y = -y;

                  z = sqrt( (double)(1.0 - y * y) );
                  sinx = y / sqrt( (double)(2.0 * (1.0 + z)) );
                  sinx2 = sinx * sinx;

                  cosx = sqrt( (double)(1.0 - sinx2) );
                  cosx2 = cosx * cosx;

                  sincs = sinx * cosx;

                  ilq = dim * l;
                  imq = dim * m;

                  for (i = 0; i < dim; i++)
                  {
                    iq = (i * i + i) / 2;
                    if ( i != l )
                    {
                      if (i != m)
                      {
                        if (i > m)
                          im = m + iq;
                        else
                          im = i + mq;

                        if (i < l)
                          il = i + lq;
                        else
                          il = l + iq;

                        x = e_val[il] * cosx - e_val[im] * sinx;
                        e_val[im] = e_val[il] * sinx + e_val[im] * cosx;
                        e_val[il] = x;
                      }
                    }

// calculate eigenvectors
                    ilr = ilq + i;
                    imr = imq + i;
                    x = e_vec[ilr] * cosx - e_vec[imr] * sinx;
                    e_vec[imr] = e_vec[ilr] * sinx + e_vec[imr] * cosx;
                    e_vec[ilr] = x;
                  }

                  x = 2.0 * e_val[lm] * sincs;
                  y = e_val[ll] * cosx2 + e_val[mm] * sinx2 - x;
                  x = e_val[ll] * sinx2 + e_val[mm] * cosx2 + x;

                  e_val[lm] = (e_val[ll] - e_val[mm]) * sincs + e_val[lm] * (cosx2 - sinx2);
                  e_val[ll] = y;
                  e_val[mm] = x;
                }

                if (m != (dim - 1))
                  m++;
                else
                  l4 = false;
              }

              if (l != (dim - 2))
                l++;
              else
                l3 = false;
            }

            if (ind == 1)
              ind = 0;
            else
              l2 = false;
          }

          if (thr <= anrmx)
            l1 = false;
        }
      }

      iq = -dim;

      for (i = 0; i < dim; i++)
      {
        iq = iq + dim;
        ll = i + (i * i + i) / 2;
        jq = dim * (i - 1);

        for (j = i; j < dim; j++)
        {
          jq = jq + dim;
          mm = j + (j * j + j) / 2;

          if (e_val[ll] < e_val[mm])
          {
            x = e_val[ll];
            e_val[ll] = e_val[mm];
            e_val[mm] = x;

            for (k = 0; k < dim; k++)
            {
              ilr = iq + k;
              imr = jq + k;
              x = e_vec[ilr];
              e_vec[ilr] = e_vec[imr];
              e_vec[imr] = x;
            }
          }
        }
      }

// writing output matrices
      eigenValuesMatrix.resize(fat, 1);
      eigenValuesMatrix.clear();
      k = 0;
      for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
          eigenVectorsMatrix(j, i) = e_vec[k++];
      for (i = 0; i < fat; i++)
        eigenValuesMatrix(i, 0) = e_val[i];

      delete []cov;
      delete []e_vec;
      delete []e_val;

      return true;
    }

  }     // end namespace common
}       // end namespace te

#endif  //__TERRALIB_COMMON_INTERNAL_MATRIXUTILS_H
