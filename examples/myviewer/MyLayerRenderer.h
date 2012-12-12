#ifndef __MY_LAYER_RENDERER_H_
#define __MY_LAYER_RENDERER_H_

#include <terralib/maptools.h>
#include <terralib/geometry.h>
#include <QRect>
#include <QPixmap>
#include <QThread>

class MyDisplay;

//class MyLayerRenderer : public te::map::LayerRenderer
class MyLayerRenderer : public te::map::LayerRenderer, QThread
{
public:
  MyLayerRenderer(bool useChanged = true);
  ~MyLayerRenderer();

  virtual void draw(te::map::AbstractLayer*, te::map::Canvas*, const te::gm::Envelope&, int srid);
  void run();
  te::color::RGBAColor getLegendColor(const std::vector<te::map::LegendItem*>& legends, const std::string& gValue, bool useDouble);
  void draw(te::map::Canvas* canvas, const te::gm::Geometry*, const te::color::RGBAColor&);
  bool transform(te::gm::Envelope& e, int oldsrid, int newsrid);
  void abort();
  te::map::Canvas* getCanvas();
  void setDisplay(MyDisplay*);
  MyDisplay* getDisplay();

private:
  bool m_useChanged;
  te::map::AbstractLayer* m_al;
  te::map::Canvas* m_canvas;
  MyDisplay* m_display;
  te::gm::Envelope m_e;
  int m_srid;
  bool m_abort;
};

#endif
