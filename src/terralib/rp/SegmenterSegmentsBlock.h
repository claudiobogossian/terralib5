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
  \file terralib/rp/SegmenterSegmentsBlock.h
  \brief Segments block.
 */

#ifndef __TERRALIB_RP_INTERNAL_SEGMENTERSEGMENTSBLOCK_H
#define __TERRALIB_RP_INTERNAL_SEGMENTERSEGMENTSBLOCK_H

  #include "Config.h"
  
  #include <vector>
  
  namespace te
  {
    namespace rp
    {  

      /*!
          \class SegmenterSegmentsBlock
          \brief Segmenter segments block description class.
      */
      class TERPEXPORT SegmenterSegmentsBlock
      {
        public :
          
          /*! Block status flag. */
          enum BlockStatus
          {
            /*! This block was not processed yet */
            BlockNotProcessed = 0,
            /*! This block is under segmentation */
            BlockUnderSegmentation = 1,        
            /*! This block already was segmented */
            BlockSegmented = 2
          };    
          
          /*! Segment ID data type definition - (zero: means invalid ID) */
          typedef unsigned int SegmentIdDataType;           
          
          /*! Block status - default:BlockNotProcessed*/
          BlockStatus m_status;      

          /*! Block upper left X over input full image - default:UINT_MAX*/
          unsigned int m_startX;
        
          /*! Block upper left Y over input full image - default:UINT_MAX*/
          unsigned int m_startY;
          
          /*! Block width - default:0*/
          unsigned int m_width;
        
          /*! Block height - default:0*/
          unsigned int m_height;      
          
          /*! Block X index inside the segments matrix - default:0*/
          unsigned int m_segmentsMatrixXIndex;          
          
          /*! Block Y index inside the segments matrix - default:0*/
          unsigned int m_segmentsMatrixYIndex;          

          /*! Top cut off profile - a vector of block line numbers (all pixels
          above this line must be ignored - the size of this vector is m_width - default:empty vector*/
          std::vector< unsigned int > m_topCutOffProfile;
          
          /*! Bottom cut off profile - a vector of block line number (all pixels
          below this line must be ignored - the size of this vector is m_width - default:empty vector*/
          std::vector< unsigned int > m_bottomCutOffProfile;
          
          /*! Left cut off profile - a vector of block column numbers (all pixels
          to the left of this line must be ignored - the size of this vector is m_height - default:empty vector*/
          std::vector< unsigned int > m_leftCutOffProfile;          
          
          /*! Right cut off profile - a vector of block column numbers (all pixels
          to the right of this line must be ignored - the size of this vector is m_height - default:empty vector*/
          std::vector< unsigned int > m_rightCutOffProfile;          
          
          SegmenterSegmentsBlock();

          ~SegmenterSegmentsBlock();   
            
          /*! Clear all internal data */   
          void clear();
          
        protected :
          
          SegmenterSegmentsBlock( const SegmenterSegmentsBlock& );
          
          const SegmenterSegmentsBlock& operator= ( 
            const SegmenterSegmentsBlock& );
      };
    } // namespace rp
  } // namespace te

#endif
