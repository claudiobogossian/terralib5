#ifndef __TERRALIB_LAYOUT_INTERNAL_GROUPRULER_LAYOUTMODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_GROUPRULER_LAYOUTMODEL_H

#include "LayoutItemModel.h"
#include "ContextLayoutItem.h"
#include "LayoutUtils.h"

#include "../../../qt/widgets/canvas/Canvas.h"
#include "../../../geometry/Envelope.h"

#include <vector>
#include <string>
#include "LayoutMode.h"
#include "AbstractRulerLayoutModel.h"

class AbstractCanvas;

namespace te
{
  namespace layout
  {
    class GroupRulerLayoutModel : public LayoutItemModel
    {
      public:

        GroupRulerLayoutModel();
        GroupRulerLayoutModel(AbstractRulerLayoutModel* verticalRuler, AbstractRulerLayoutModel* horizontalRuler);
        virtual ~GroupRulerLayoutModel();

        virtual void updateObserver(ContextLayoutItem context);

        virtual void draw( ContextLayoutItem context );
        virtual void setPosition(const double& x, const double& y);
        
        virtual void setPaperBox(te::gm::Envelope*	 box);
        virtual te::gm::Envelope*	 getPaperBox();
                
        virtual void setVisibleHorizontalRuler(bool visible);
        virtual bool isVisibleHorizontalRuler();

        virtual void invertedLines(bool invert);

      protected:

        te::gm::Envelope*	 _paperBox;
        double	  _longLine;
        double	  _mediumLine;
        double    _smallLine;
        double    _spacing;
        double	  _lineMargin;
        int		  _horizontalBlockMarks;
        int		  _blockSize;
        int		  _middleBlockSize;
        double	  _borderLineSize;
        bool	  _visibleHorizontalRuler;
        double	  _posCount;
        std::vector<std::string> _horizontalTexts;
        bool      _invertedLines;
        AbstractRulerLayoutModel* _verticalRuler;
        AbstractRulerLayoutModel* _horizontalRuler;
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_GROUPRULER_LAYOUTMODEL_H