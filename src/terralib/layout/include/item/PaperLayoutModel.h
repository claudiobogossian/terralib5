#ifndef __TERRALIB_LAYOUT_INTERNAL_PAPERLAYOUTMODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_PAPERLAYOUTMODEL_H

#include "LayoutItemModelObservable.h"
#include "ContextLayoutItem.h"

namespace te
{
  namespace layout
  {
    class PaperLayoutModel : public LayoutItemModelObservable
    {
      public:

        PaperLayoutModel();
        virtual ~PaperLayoutModel();

        virtual void draw( ContextLayoutItem context );

        virtual te::color::RGBAColor getShadowColor();
        virtual void setShadowColor(te::color::RGBAColor color);

    protected:

      te::gm::Envelope _boxPaper;
      te::gm::Envelope _boxShadow;
      te::color::RGBAColor _shadowColor;
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_RECTANGLELAYOUTMODEL_H