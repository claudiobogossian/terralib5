#ifndef CONTEXTLAYOUTITEM_H
#define CONTEXTLAYOUTITEM_H

#include "LayoutParams.h"
#include "../../../color/RGBAColor.h"

namespace te
{
  namespace layout
  {
    class LayoutPixmap;

    class ContextLayoutItem 
    {
      public:

        ContextLayoutItem();
        ContextLayoutItem(LayoutParams params, double zoomFactor);
        virtual ~ContextLayoutItem();

        void setParams(LayoutParams params);
        LayoutParams getParams();

        void setZoomFactor(double zoomFactor);
        double getZoomFactor();

        void setPixmap(te::color::RGBAColor** pixmap);
        te::color::RGBAColor** getPixmap();

        void setShow( bool show );
        bool getShow();

        void setWait(bool wait);
        bool getWait();

        bool isResizeCanvas();
        void setResizeCanvas(bool clear);

      protected:
        LayoutParams            _params;
        double                  _zoomFactor;
        te::color::RGBAColor**  _pixmap;
        bool		                _show;
        bool		                _wait;
        bool                    _resizeCanvas;
    };
  }
}

#endif