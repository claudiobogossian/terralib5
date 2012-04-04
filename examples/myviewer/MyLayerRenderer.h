#ifndef __MY_LAYER_RENDERER_H_
#define __MY_LAYER_RENDERER_H_

#include <terralib/maptools.h>
#include <qrect.h>

class MyLayerRenderer : public te::map::LayerRenderer
{
public:
  MyLayerRenderer(bool useChanged = true);
  ~MyLayerRenderer();

  virtual void draw(te::map::AbstractLayer*, te::map::Canvas*, const te::gm::Envelope&, int);

private:
  bool m_useChanged;
};

#endif
