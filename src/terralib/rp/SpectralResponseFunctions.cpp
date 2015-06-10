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
  \file terralib/rp/SpectralResponseFunctions.cpp
  \brief Spectral Response Functions.
*/

#include "SpectralResponseFunctions.h"
  
namespace te
{
  namespace rp
  {
    namespace srf
    {
      double interpolateSRF( const std::map< double, double >& sRFs, 
        const double& frequency )
      {
        std::map< double, double >::const_iterator it2 = sRFs.find( frequency );
        
        if( it2 == sRFs.end() )
        {
          it2 = sRFs.upper_bound( frequency );
          
          if( ( it2 == sRFs.begin() ) || ( it2 == sRFs.end() ) )
          {
            return 0.0;
          }
          else
          {
            std::map< double, double >::const_iterator it1 = it2;
            --it1;
            
            double dist1 = frequency - it1->first;
            assert( dist1 > 0.0 );          
            double dist2 = it2->first - frequency;
            assert( dist2 > 0.0 );
            
            return ( 
                      ( it1->second * dist2 ) 
                      + 
                      ( it2->second * dist1 )
                   ) 
                   / 
                   ( dist1  + dist2 );
          }
        }
        else
        {
          return it2->second;
        }
      }
      
      double getSRFArea( const std::map< double, double >& sRFs )
      {
        double returnValue = 0.0;
        
        std::map< double, double >::const_iterator it = sRFs.begin();
        std::map< double, double >::const_iterator itPrev;
        const std::map< double, double >::const_iterator itEnd = sRFs.end();      
        
        while( it != itEnd )
        {
          if( it == sRFs.begin() )
          {
            itPrev = it;
          }
          else
          {
            returnValue += std::abs( it->second - itPrev->second ) * 
              ( it->first - itPrev->first ) / 2.0;
            returnValue += ( it->first - itPrev->first ) *
              std::min( it->second, itPrev->second );
            
            ++itPrev;
          }
          
          ++it;
        }
        
        return returnValue;
      }
      
      void getIntersectionSRF( const std::map< double, double >& sRF1, 
        const std::map< double, double >& sRF2,
        std::map< double, double >& intersectionSRF )
      {
        getUnionSRF( sRF1, sRF2, intersectionSRF );
        
        std::map< double, double >::const_iterator it = intersectionSRF.begin();
        const std::map< double, double >::const_iterator itEnd = intersectionSRF.end();
        
        while( it != itEnd )
        {
          intersectionSRF[ it->first ] =
            std::min(
              interpolateSRF( sRF1, it->first ),
              interpolateSRF( sRF2, it->first )
            );
            
          ++it;
        }
      }
      
      void getUnionSRF( const std::map< double, double >& sRF1, 
        const std::map< double, double >& sRF2,
        std::map< double, double >& unionSRF )
      {
        unionSRF = sRF2;
        
        std::map< double, double >::const_iterator it1 = sRF1.begin();
        const std::map< double, double >::const_iterator it1End = sRF1.end();
        
        while( it1 != it1End )
        {
          unionSRF[ it1->first ] = 
            std::max( 
              interpolateSRF( unionSRF, it1->first ),
              it1->second
            );
            
          ++it1;
        }
      }
      
    }
  }
}   // end namespace te

