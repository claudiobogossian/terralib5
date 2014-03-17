#ifndef __TERRALIB_LAYOUT_INTERNAL_HORIZONTALRULER_LAYOUTMODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_HORIZONTALRULER_LAYOUTMODEL_H

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
    class HorizontalRulerLayoutModel : public AbstractRulerLayoutModel
    {
      public:

        HorizontalRulerLayoutModel();
        ~HorizontalRulerLayoutModel();

        virtual void updateObserver(ContextLayoutItem context);

        virtual void draw( ContextLayoutItem context );
        
        virtual void setPaperBox(te::gm::Envelope	 box);
        virtual te::gm::Envelope	 getPaperBox();

        virtual void setBox(te::gm::Envelope box);
                
        virtual void setVisibleHorizontalRuler(bool visible);
        virtual bool isVisibleHorizontalRuler();

        virtual void invertedLines(bool invert);
        
      protected:

        virtual void drawHorizontalRuler(te::map::Canvas* canvas, LayoutUtils* utils);
        virtual void drawRuler(te::map::Canvas* canvas, LayoutUtils* utils);
        virtual void drawMarks(te::map::Canvas* canvas, LayoutUtils* utils, int marks);

        virtual void updateHorizontalListText();

      protected:

        te::gm::Envelope	 _paperBox;
        te::gm::Envelope	 _backEndBox;
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
        double _backEndSpacing;
        double _backEndMargin;
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_HORIZONTALRULER_LAYOUTMODEL_H