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
  \file terralib/rp/Functions.cpp
  \brief Raster Processing functions.
*/

// TerraLib
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypePersistence.h"
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "../datatype/Enums.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/RasterFactory.h"
#include "../raster/RasterProperty.h"
#include "Exception.h"
#include "Functions.h"
#include "Macros.h"
#include "RasterHandler.h"

// Boost
#include <boost/filesystem.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/shared_ptr.hpp>


// STL
#include <cstring>
#include <limits>
#include <map>
#include <memory>

namespace te
{
  namespace rp
  {
    bool createNewRaster( const te::rst::Grid& rasterGrid,
      std::vector< te::rst::BandProperty* > bandsProperties,
      const std::string& outDataSetName,
      te::da::DataSource& outDataSource,
      RasterHandler& outRasterHandler )
    {
      // Defining the raster properties

      std::auto_ptr< te::rst::RasterProperty > rasterPropertyPtr(
        new te::rst::RasterProperty( new te::rst::Grid( rasterGrid ),
        bandsProperties, std::map< std::string, std::string >(),
        false, 0, 0 ) );

      // acquiring a transactor instance

      std::auto_ptr< te::da::DataSourceTransactor > transactorPtr(
        outDataSource.getTransactor() );

      if( transactorPtr.get() == 0 )
      {
        return false;
      }

      // acquiring a persistence instance

      std::auto_ptr< te::da::DataSetTypePersistence > persistencePtr(
        transactorPtr->getDataSetTypePersistence() );

      if( persistencePtr.get() == 0 )
      {
        return false;
      }

      // Creating a data set instance

      std::auto_ptr< te::da::DataSetType > dataSetTypePtr(
        new te::da::DataSetType( outDataSetName ) );
      dataSetTypePtr->add( rasterPropertyPtr.release() );

      persistencePtr->create( dataSetTypePtr.release() );

      std::auto_ptr< te::da::DataSet > dataSetPtr( transactorPtr->getDataSet(
        outDataSetName, te::common::FORWARDONLY, te::common::RWAccess) );

      if( dataSetPtr.get() == 0 )
      {
        return false;
      }

      // Creating a raster instance

      std::auto_ptr< te::rst::Raster > rasterPtr( dataSetPtr->getRaster() );

      if( rasterPtr.get() )
      {
        outRasterHandler.reset( transactorPtr.release(),
          persistencePtr.release(), dataSetPtr.release(),
          rasterPtr.release() );

        return true;
      }
      else
      {
        return false;
      }
    }

    bool createNewMemRaster( const te::rst::Grid& rasterGrid,
      std::vector< te::rst::BandProperty* > bandsProperties,
      RasterHandler& outRasterHandler )
    {
      // Creating a new memory datasource

      boost::shared_ptr< te::da::DataSource > dataSourcePtr(
        te::da::DataSourceFactory::make( "MEM" ) );
      TERP_TRUE_OR_THROW( dataSourcePtr.get(), "Data source creation error" );

      // Defining the raster properties

      std::auto_ptr< te::rst::RasterProperty > rasterPropertyPtr(
        new te::rst::RasterProperty( new te::rst::Grid( rasterGrid ),
        bandsProperties, std::map< std::string, std::string >(),
        false, 0, 0 ) );

      // acquiring a transactor instance

       boost::shared_ptr< te::da::DataSourceTransactor > transactorPtr(
        dataSourcePtr->getTransactor() );

      if( transactorPtr.get() == 0 )
      {
        return false;
      }

      // acquiring a persistence instance

      boost::shared_ptr< te::da::DataSetTypePersistence > persistencePtr(
        transactorPtr->getDataSetTypePersistence() );

      if( persistencePtr.get() == 0 )
      {
        return false;
      }

      // Creating a data set instance

      std::auto_ptr< te::da::DataSetType > dataSetTypePtr(
        new te::da::DataSetType( "createNewMemRaster" ) );
      dataSetTypePtr->add( rasterPropertyPtr.release() );

      persistencePtr->create( dataSetTypePtr.release() );

      boost::shared_ptr< te::da::DataSet > dataSetPtr( transactorPtr->getDataSet(
        "createNewMemRaster", te::common::FORWARDONLY, te::common::RWAccess) );

      if( dataSetPtr.get() == 0 )
      {
        return false;
      }

      // Creating a raster instance

      boost::shared_ptr< te::rst::Raster > rasterPtr( dataSetPtr->getRaster() );

      if( rasterPtr.get() )
      {
        outRasterHandler.reset( dataSourcePtr, transactorPtr,
          persistencePtr, dataSetPtr, rasterPtr );

        return true;
      }
      else
      {
        return false;
      }
    }

    bool createNewGeotifRaster( const te::rst::Grid& rasterGrid,
      std::vector< te::rst::BandProperty* > bandsProperties,
      const std::string& fileName,
      RasterHandler& outRasterHandler )
    {
      boost::filesystem::path pathInfo( fileName );

      // Creating a new memory datasource

      boost::shared_ptr< te::da::DataSource > dataSourcePtr(
        te::da::DataSourceFactory::make( "GDAL" ) );
      if( dataSourcePtr.get() == 0 ) return false;

      std::map<std::string, std::string> outputRasterInfo;
      outputRasterInfo["URI"] = pathInfo.parent_path().string();

      dataSourcePtr->open(outputRasterInfo);
      if( ! dataSourcePtr->isOpened() ) return false;

      // Defining the raster properties

      std::auto_ptr< te::rst::RasterProperty > rasterPropertyPtr(
        new te::rst::RasterProperty( new te::rst::Grid( rasterGrid ),
        bandsProperties, std::map< std::string, std::string >(),
        false, 0, 0 ) );

      // acquiring a transactor instance

       boost::shared_ptr< te::da::DataSourceTransactor > transactorPtr(
        dataSourcePtr->getTransactor() );

      if( transactorPtr.get() == 0 )
      {
        return false;
      }

      // acquiring a persistence instance

      boost::shared_ptr< te::da::DataSetTypePersistence > persistencePtr(
        transactorPtr->getDataSetTypePersistence() );

      if( persistencePtr.get() == 0 )
      {
        return false;
      }

      // Creating a data set instance

      std::auto_ptr< te::da::DataSetType > dataSetTypePtr(
        new te::da::DataSetType( pathInfo.filename().string() ) );
      dataSetTypePtr->add( rasterPropertyPtr.release() );

      persistencePtr->create( dataSetTypePtr.release() );

      boost::shared_ptr< te::da::DataSet > dataSetPtr( transactorPtr->getDataSet(
        pathInfo.filename().string(), te::common::FORWARDONLY,
        te::common::RWAccess) );

      if( dataSetPtr.get() == 0 )
      {
        return false;
      }

      // Creating a raster instance

      boost::shared_ptr< te::rst::Raster > rasterPtr( dataSetPtr->getRaster() );

      if( rasterPtr.get() )
      {
        outRasterHandler.reset( dataSourcePtr, transactorPtr,
          persistencePtr, dataSetPtr, rasterPtr );

        return true;
      }
      else
      {
        return false;
      }
    }


    void getDataTypeRange( const int dataType, double& min, double& max )
    {
      switch( dataType )
      {
        case te::dt::BIT_TYPE :
          min = 0;
          max = 1;
          break;
        case te::dt::CHAR_TYPE :
          min = (double)std::numeric_limits<char>::min();
          max = (double)std::numeric_limits<char>::max();
          break;
        case te::dt::UCHAR_TYPE :
          min = (double)std::numeric_limits<unsigned char>::min();
          max = (double)std::numeric_limits<unsigned char>::max();
          break;
        case te::dt::INT16_TYPE :
        case te::dt::CINT16_TYPE :
          min = (double)std::numeric_limits<short int>::min();
          max = (double)std::numeric_limits<short int>::max();
          break;
        case te::dt::UINT16_TYPE :
          min = (double)std::numeric_limits<unsigned short int>::min();
          max = (double)std::numeric_limits<unsigned short int>::max();
          break;
        case te::dt::INT32_TYPE :
        case te::dt::CINT32_TYPE :
          min = (double)std::numeric_limits<int>::min();
          max = (double)std::numeric_limits<int>::max();
          break;
        case te::dt::UINT32_TYPE :
          min = (double)std::numeric_limits<unsigned int>::min();
          max = (double)std::numeric_limits<unsigned int>::max();
          break;
        case te::dt::INT64_TYPE :
          min = (double)std::numeric_limits<long int>::min();
          max = (double)std::numeric_limits<long int>::max();
          break;
        case te::dt::UINT64_TYPE :
          min = (double)std::numeric_limits<unsigned long int>::min();
          max = (double)std::numeric_limits<unsigned long int>::max();
          break;
        case te::dt::FLOAT_TYPE :
        case te::dt::CFLOAT_TYPE :
          min = (double)std::numeric_limits<float>::min();
          max = (double)std::numeric_limits<float>::max();
          break;
        case te::dt::DOUBLE_TYPE :
        case te::dt::CDOUBLE_TYPE :
          min = (double)std::numeric_limits<double>::min();
          max = (double)std::numeric_limits<double>::max();
          break;
        default :
          throw te::rp::Exception( "Invalid data type" );
          break;
      };
    }

    void convert2DoublesVector( void* inputVector, const int inputVectorDataType,
      unsigned int inputVectorSize, double* outputVector )
    {
      switch( inputVectorDataType )
      {
        case te::dt::CHAR_TYPE :
        {
          char* vPtr = (char*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::BIT_TYPE :
        case te::dt::UCHAR_TYPE :
        {
          unsigned char* vPtr = (unsigned char*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::INT16_TYPE :
        {
          short int* vPtr = (short int*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::CINT16_TYPE :
        {
          std::complex< short int >* vPtr = (std::complex< short int >*)
            inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ].real();
          break;
        }
        case te::dt::UINT16_TYPE :
        {
          unsigned short int* vPtr = (unsigned short int*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::INT32_TYPE :
        {
          int* vPtr = (int*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::CINT32_TYPE :
        {
          std::complex< int >* vPtr = (std::complex< int >*)
            inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ].real();
          break;
        }
        case te::dt::UINT32_TYPE :
        {
          unsigned int* vPtr = (unsigned int*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::INT64_TYPE :
        {
          long int* vPtr = (long int*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::UINT64_TYPE :
        {
          unsigned long int* vPtr = (unsigned long int*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::FLOAT_TYPE :
        {
          float* vPtr = (float*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::CFLOAT_TYPE :
        {
          std::complex< float >* vPtr = (std::complex< float >*)
            inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ].real();
          break;
        }
        case te::dt::DOUBLE_TYPE :
        {
          memcpy( outputVector, inputVector, inputVectorSize * sizeof( double ) );
          break;
        }
        case te::dt::CDOUBLE_TYPE :
        {
          std::complex< double >* vPtr = (std::complex< double >*)
            inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ].real();
          break;
        }
        default :
          throw te::rp::Exception( "Invalid data type" );
          break;
      };
    }

    void convertDoublesVector( double* inputVector,
      unsigned int inputVectorSize, const int outputVectorDataType,
      void* outputVector )
    {
      switch( outputVectorDataType )
      {
        case te::dt::CHAR_TYPE :
        {
          char* vPtr = (char*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (char)inputVector[ idx ];
          break;
        }
        case te::dt::BIT_TYPE :
        case te::dt::UCHAR_TYPE :
        {
          unsigned char* vPtr = (unsigned char*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (unsigned char)inputVector[ idx ];
          break;
        }
        case te::dt::INT16_TYPE :
        {
          short int* vPtr = (short int*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (short int)inputVector[ idx ];
          break;
        }
        case te::dt::CINT16_TYPE :
        {
          std::complex< short int >* vPtr = (std::complex< short int >*)
            outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ]= ( (short int)inputVector[ idx ] );
          break;
        }
        case te::dt::UINT16_TYPE :
        {
          unsigned short int* vPtr = (unsigned short int*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (unsigned short int)inputVector[ idx ];
          break;
        }
        case te::dt::INT32_TYPE :
        {
          int* vPtr = (int*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (int)inputVector[ idx ];
          break;
        }
        case te::dt::CINT32_TYPE :
        {
          std::complex< int >* vPtr = (std::complex< int >*)
            outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = ( (int)inputVector[ idx ] );
          break;
        }
        case te::dt::UINT32_TYPE :
        {
          unsigned int* vPtr = (unsigned int*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (unsigned int)inputVector[ idx ];
          break;
        }
        case te::dt::INT64_TYPE :
        {
          long int* vPtr = (long int*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (long int)inputVector[ idx ];
          break;
        }
        case te::dt::UINT64_TYPE :
        {
          unsigned long int* vPtr = (unsigned long int*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (unsigned long int)inputVector[ idx ];
          break;
        }
        case te::dt::FLOAT_TYPE :
        {
          float* vPtr = (float*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (float)inputVector[ idx ];
          break;
        }
        case te::dt::CFLOAT_TYPE :
        {
          std::complex< float >* vPtr = (std::complex< float >*)
            outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = ( (float)inputVector[ idx ] );
          break;
        }
        case te::dt::DOUBLE_TYPE :
        {
          memcpy( outputVector, outputVector, inputVectorSize * sizeof( double ) );
          break;
        }
        case te::dt::CDOUBLE_TYPE :
        {
          std::complex< double >* vPtr = (std::complex< double >*)
            outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = ( (double)inputVector[ idx ] );
          break;
        }
        default :
          throw te::rp::Exception( "Invalid data type" );
          break;
      };
    }

    bool InvertMatrix(const boost::numeric::ublas::matrix<double>& input,
                      boost::numeric::ublas::matrix<double>& inverse)
    {
      boost::numeric::ublas::matrix<double> A(input);
      boost::numeric::ublas::permutation_matrix<std::size_t> pm(A.size1());

      if (lu_factorize(A, pm) != 0)
        return false;

      inverse.assign(boost::numeric::ublas::identity_matrix<double> (A.size1()));
      lu_substitute(A, pm, inverse);

      return true;
    }

    bool EigenVectors(boost::numeric::ublas::matrix<double> inputMatrix, boost::numeric::ublas::matrix<double> &eigenVectorsMatrix, boost::numeric::ublas::matrix<double> &eigenValuesMatrix)
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

  } // end namespace rp
}   // end namespace te

