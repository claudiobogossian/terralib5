#include "SegmenterSegmentsBlock.h"

#include <climits>

namespace te
{
  namespace rp
  { 
    
    SegmenterSegmentsBlock::SegmenterSegmentsBlock()
    {
      clear();
    }
    
    SegmenterSegmentsBlock::SegmenterSegmentsBlock( const
      SegmenterSegmentsBlock& )
    {
      clear();
    }    


    SegmenterSegmentsBlock::~SegmenterSegmentsBlock()
    {
      clear();
    }

    void SegmenterSegmentsBlock::clear()
    {
      m_startX = m_startY = UINT_MAX;
      m_status = BlockNotProcessed;
      m_width = m_height = 0;
      m_segmentsMatrixXIndex = 0;
      m_segmentsMatrixYIndex = 0;      
      m_topCutOffProfile.clear();
      m_bottomCutOffProfile.clear();
      m_leftCutOffProfile.clear();
      m_rightCutOffProfile.clear();
    }
    
    const SegmenterSegmentsBlock& SegmenterSegmentsBlock::operator=(
      const SegmenterSegmentsBlock& )
    {
      return *this;
    }

  } // namespace rp
} // namespace te
