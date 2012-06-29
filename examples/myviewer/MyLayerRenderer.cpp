#include "MyLayerRenderer.h"
#include "MyLayer.h"
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/maptools.h>
#include <terralib/qt/widgets.h>

#include <QMatrix>
#include <QRectF>
#include <set>

MyLayerRenderer::MyLayerRenderer(bool useChanged) :
  te::map::LayerRenderer(),
    m_useChanged(useChanged)
{
}

MyLayerRenderer::~MyLayerRenderer()
{
}


// Existe um erro nesta forma de desenho. Quando se tem um emaranhado de pontos ou linhas superpostas, deveriamos
// desenhar primeiro os nao selecionados e em seguida o resto. Desta forma garantimos que os
// selecionados serao visiveis (ficam por cima dos nao selecionados). Mas, como fazer isso de forma eficiente???
void MyLayerRenderer::draw(te::map::AbstractLayer* al, te::map::Canvas* canvas,
     const te::gm::Envelope& e, int srid)
{
  if(m_useChanged == false)
    canvas->clear();

  MyLayer* layer =  (MyLayer*)al;
  te::map::DataGridOperation* op = layer->getDataGridOperation();

  if(op)
  {
    const std::set<std::string>& changeds = op->getTheChanged();
    te::da::DataSet* dataSet = op->getDataSet();
    te::da::DataSetType* dsType = op->getDataSetType();
    te::da::PrimaryKey *pk = dsType->getPrimaryKey();
    const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
    std::string pkName = pkProps[0]->getName();
    int pkPos = dsType->getPropertyPosition(pkName);
    std::string pkv;
    int status = -1;
    te::color::RGBAColor defaultColor;
    te::color::RGBAColor cor;
    std::size_t gPos = dsType->getDefaultGeomPropertyPos();
    te::gm::GeometryProperty* gProp = dsType->getDefaultGeomProperty();
    int gtype = gProp->getGeometryType();
    int width = 0, ptMark = 0;

    if(gtype == te::gm::PointType || gtype == te::gm::PointZType || gtype == te::gm::PointMType || gtype == te::gm::PointZMType)
    {
      defaultColor = op->getPointColor();
      canvas->setPointColor(defaultColor);
      if(op->getPointMarkerType() == te::map::MarkerPattern && op->getPointIcon())
      {
        canvas->setPointPattern(op->getPointIcon(), op->getPointIconSize(), op->getPointIconImageType());
        canvas->setPointWidth(op->getPointWidth());
      }
      else
      {
        canvas->setPointWidth(op->getPointWidth());
        //canvas->setPointMarker(op->getPointMarkerType());
      }
    }
    else if(gtype == te::gm::LineStringType || gtype == te::gm::LineStringZType || gtype == te::gm::LineStringMType || gtype == te::gm::LineStringZMType ||
      gtype == te::gm::MultiLineStringType || gtype == te::gm::MultiLineStringZType || gtype == te::gm::MultiLineStringMType || gtype == te::gm::MultiLineStringZMType)
    {
      defaultColor = op->getLineColor();
      if(op->getLinePatternIcon())
        canvas->setLinePattern(op->getLinePatternIcon(), op->getLinePatternIconSize(), op->getLinePatternIconImageType());
      canvas->setLineWidth(op->getLineWidth());
      canvas->setLineColor(op->getLineColor());
      //((te::qt::widgets::Canvas*)canvas)->setLineStyle(Qt::SolidLine);
    }
    else
    {
      // polygon fill style
      defaultColor = op->getPolygonFillColor();
      canvas->setPolygonFillColor(defaultColor);
      if(op->getPolygonPatternIcon())
      {
        canvas->setPolygonFillPattern(op->getPolygonPatternIcon(), op->getPolygonPatternIconSize(), op->getPolygonPatternIconImageType());
        canvas->setPolygonPatternWidth(op->getPolygonPatternWidth());
      }
      else if(op->getPolygonMarkerType() != te::map::MarkerNone && op->getPolygonMarkerType() != te::map::MarkerPattern)
      {
        //canvas->setPolygonFillMarkerColor(op->getPolygonFillMarkerColor());
        //canvas->setPolygonFillMarker(op->getPolygonMarkerType());
      }
      else
        canvas->setPolygonFillPattern(0, 0, 0);

      // polygon contour style
      canvas->setPolygonContourColor(op->getPolygonContourColor());
      //((te::qt::widgets::Canvas*)canvas)->setPolygonContourStyle(Qt::SolidLine);
      canvas->setPolygonContourWidth(op->getPolygonContourWidth());
      if(op->getPolygonContourPatternIcon())
        canvas->setPolygonContourPattern(op->getPolygonContourPatternIcon(), op->getPolygonContourPatternIconSize(), op->getPolygonContourPatternIconImageType());
    }

    size_t size = changeds.size();

    int w = canvas->getWidth();
    int h = canvas->getHeight();
    QMatrix matrix = ((te::qt::widgets::Canvas*)canvas)->getMatrix();
    QRectF deviceRect(0, 0, w, h);
    QRectF worldRect = matrix.inverted().mapRect(deviceRect);

    // increase the rectangle to verify geometries of type point. 
    double delta = 50.; 
    QRectF pointDeviceRect(-delta, -delta, w + 2 * delta, h + 2 * delta);
    QRectF pointWorldRect = matrix.inverted().mapRect(pointDeviceRect);

    te::gm::Geometry* g;
    std::vector<te::gm::Geometry*> geoms, tgeoms;
    std::vector<te::color::RGBAColor> colors, tcolors;
    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      pkv = dataSet->getAsString(pkPos);

      if(m_useChanged)
      {
        if(size <= 0)
          break;
        if(changeds.find(pkv) == changeds.end())
          continue;

        --size;
      }

      g = dataSet->getGeometry(gPos);
      if(g == 0)
        continue;

      g->transform(srid);
      const te::gm::Envelope* env = g->getMBR();
      QRectF r(env->m_llx, env->m_lly, env->getWidth(), env->getHeight());

      if(env->getWidth() == 0 && env->getHeight() == 0)
      {
        if(pointWorldRect.contains(env->m_llx, env->m_lly) == false)
        {
          delete g;
          continue;
        }
      }
      else
      {
        if(r.intersects(worldRect) == false)
        {
          delete g;
          continue;
        }
      }

      status = op->getDataSetSelectionStatus(pkv);
      if(!(status == te::map::DataGridOperation::POINTED || status == te::map::DataGridOperation::QUERIED))
        cor = op->getDefaultColor();

      cor = defaultColor;
      if(status == te::map::DataGridOperation::POINTED)
        cor = op->getPointedColor();
      else if(status == te::map::DataGridOperation::QUERIED)
        cor = op->getQueriedColor();
      else if(status == te::map::DataGridOperation::POINTED_AND_QUERIED)
        cor = op->getPointedAndQueriedColor();

      if(m_useChanged == false)
      {
        draw(canvas, g, cor);
        delete g;
      }
      else
      {
        if(cor.getAlpha() == 255)
        {
          geoms.push_back(g);
          colors.push_back(cor);
        }
        else
        {
          tgeoms.push_back(g);
          tcolors.push_back(cor);
        }
      }
    }
    
    if(m_useChanged)
    {
      std::vector<te::gm::Geometry*>::iterator git;
      std::vector<te::color::RGBAColor>::iterator cit;
      
      // erase transparent geometries
      ((te::qt::widgets::Canvas*)(canvas))->setEraseMode();
      for(git = tgeoms.begin(), cit = tcolors.begin(); git != tgeoms.end(); ++git, ++cit)
      {
        g = *git;
        cor = *cit;
        draw(canvas, g, cor);
      }
      ((te::qt::widgets::Canvas*)(canvas))->setNormalMode();

      // draw opaque geometries
      for(git = geoms.begin(), cit = colors.begin(); git != geoms.end(); ++git, ++cit)
      {
        g = *git;
        cor = *cit;
        draw(canvas, g, cor);
        delete g;
      }

      // draw transparent geometries
      for(git = tgeoms.begin(), cit = tcolors.begin(); git != tgeoms.end(); ++git, ++cit)
      {
        g = *git;
        cor = *cit;
        draw(canvas, g, cor);
        delete g;
      }
    }
  }
  else
    te::map::LayerRenderer::draw(al, canvas, e, srid);
}

void MyLayerRenderer::draw(te::map::Canvas* canvas, const te::gm::Geometry* g, const te::color::RGBAColor& cor)
{
  int gtype = g->getGeomTypeId();
  switch(gtype)
  {
    case te::gm::MultiPolygonType:
    case te::gm::PolygonType:
    {
      canvas->setPolygonFillColor(cor);
    }
    break;
    
    case te::gm::MultiLineStringType:
    case te::gm::LineStringType:
    {
      canvas->setLineColor(cor);
    }
    break;
  
    default:
      canvas->setPointColor(cor);
  }
  canvas->draw(g);
}
