#ifndef __TERRALIB_LAYOUT_INTERNAL_VERTICALRULER_LAYOUTMODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_VERTICALRULER_LAYOUTMODEL_H

#include "ContextLayoutItem.h"
#include "LayoutMode.h"
#include "LayoutUtils.h"

#include "../../../qt/widgets/canvas/Canvas.h"

#include <vector>
#include <string>
#include "AbstractRulerLayoutModel.h"

class AbstractCanvas;

namespace te
{
  namespace layout
  {
    class VerticalRulerLayoutModel : public AbstractRulerLayoutModel
    {
      public:

        VerticalRulerLayoutModel();
        ~VerticalRulerLayoutModel();

        virtual void updateObserver(ContextLayoutItem context);

        virtual void draw( ContextLayoutItem context );
        virtual void setPosition(const double& x, const double& y);
        
        virtual void setPaperBox(te::gm::Envelope*	 box);
        virtual te::gm::Envelope*	 getPaperBox();

        virtual void setBox(te::gm::Envelope* box);
        
        virtual void setVisibleVerticalRuler(bool visible);
        virtual bool isVisibleVerticalRuler();

        virtual void invertedLines(bool invert);

      protected:

        virtual void drawVerticalRuler(te::map::Canvas* canvas, LayoutUtils* utils);
 
        virtual void drawRuler(te::map::Canvas* canvas, LayoutUtils* utils);

        virtual void drawMarks(te::map::Canvas* canvas, LayoutUtils* utils, int marks);
        
        virtual void updateVerticalListText();

      protected:

        te::gm::Envelope*	 _paperBox;
        double	  _longLine;
        double	  _mediumLine;
        double    _smallLine;
        double    _spacing;
        double	  _lineMargin;
        int		  _verticalBlockMarks;
        int		  _blockSize;
        int		  _middleBlockSize;
        double	  _borderLineSize;
        bool	  _visibleVerticalRuler;
        double	  _posCount;
        std::vector<std::string> _verticalTexts;
        bool      _invertedLines;
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_RULERLAYOUTMODEL_H