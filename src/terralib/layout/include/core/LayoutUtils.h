#ifndef __TERRALIB_LAYOUT_INTERNAL_UTILS_H
#define __TERRALIB_LAYOUT_INTERNAL_UTILS_H

#include "../../../geometry/Envelope.h"
#include "../../../geometry/LinearRing.h"
#include "../../../color/RGBAColor.h"
#include "../../../maptools/Canvas.h"
#include "../../../maptools/WorldDeviceTransformer.h"

namespace te
{
  namespace layout
  {
    class LayoutUtils
    {
      public:

        LayoutUtils();
        virtual ~LayoutUtils();
        virtual void drawRectW(te::map::Canvas* canvas, te::gm::Envelope* box);
        virtual void drawLineW(te::map::Canvas* canvas, te::gm::LinearRing* line);
        virtual te::color::RGBAColor** getImageW(te::map::Canvas* canvas, te::gm::Envelope* box);
        virtual int mm2pixel(te::map::Canvas* canvas, double mm);
        virtual void configCanvas(te::map::Canvas* canvas, te::gm::Envelope* box);
        virtual te::gm::Envelope* viewportBox(te::map::Canvas* canvas, te::gm::Envelope* box);
        virtual te::gm::LinearRing* createSimpleLine(te::gm::Envelope* box);

      protected:

        te::map::WorldDeviceTransformer    m_transformer; // World Device Transformer.
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_UTILS_H