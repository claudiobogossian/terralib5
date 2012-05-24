#ifndef __MY_LAYER_RENDERER_H_
#define __MY_LAYER_RENDERER_H_

#include <terralib/maptools.h>
#include <QRect>
#include <QPixmap>

class MyLayerRenderer : public te::map::LayerRenderer
{
public:
  MyLayerRenderer(bool useChanged = true);
  ~MyLayerRenderer();

  virtual void draw(te::map::AbstractLayer*, te::map::Canvas*, const te::gm::Envelope&, int srid);
  void draw(te::map::Canvas* canvas, const te::gm::Geometry*, const te::color::RGBAColor&);

private:
  bool m_useChanged;
};

#endif
