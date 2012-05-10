#include "MyLayerRenderer.h"
#include "MyLayer.h"
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/maptools/PtMarker.h>
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
    te::color::RGBAColor cor;
    std::size_t gPos = dsType->getDefaultGeomPropertyPos();
    te::gm::GeometryProperty* gProp = dsType->getDefaultGeomProperty();
    int gtype = gProp->getGeometryType();

    canvas->setPolygonContourColor(op->getContourColor());
    canvas->setPolygonContourWidth(op->getContourWidth());
    //canvas->setPointWidth(op->getPtWidth());
    //canvas->setPointMarker(op->getPtMarkerType());
    te::color::RGBAColor** mark = te::map::CreateMarker(op->getPtMarkerType(), op->getPtWidth(), op->getDeselectedColor());
    canvas->setPointPattern(mark, op->getPtWidth(), op->getPtWidth());
    canvas->setLineWidth(op->getLineWidth());
    canvas->setPolygonFillColor(op->getDeselectedColor());
    canvas->setLineColor(op->getDeselectedColor());
    //canvas->setPointColor(op->getDeselectedColor());

    size_t size = changeds.size();

    int w = canvas->getWidth();
    int h = canvas->getHeight();
    QMatrix matrix = ((te::qt::widgets::Canvas*)canvas)->getMatrix();
    QRectF deviceRect(0, 0, w, h);
    QRectF worldRect = matrix.inverted().mapRect(deviceRect);

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
      }

      --size;
      te::gm::Geometry* g = dataSet->getGeometry(gPos);
      if(g == 0)
        continue;

      g->transform(srid);
      const te::gm::Envelope* env = g->getMBR();
      QRectF r(env->m_llx, env->m_lly, env->getWidth(), env->getHeight());

      if(gtype == te::gm::MultiPolygonType || gtype == te::gm::PolygonType || gtype == te::gm::MultiLineStringType || gtype == te::gm::LineStringType)
      {
        if(r.intersects(worldRect) == false)
        {
          delete g;
          continue;
        }
      }

      status = op->getDataSetSelectionStatus(pkv);
      if(!(status == te::map::DataGridOperation::POINTED || status == te::map::DataGridOperation::QUERIED))
        cor = op->getDeselectedColor();
      else if(status == te::map::DataGridOperation::POINTED)
        cor = op->getPointedColor();
      else if(status == te::map::DataGridOperation::QUERIED)
        cor = op->getQueriedColor();
      else if(status == te::map::DataGridOperation::POINTED_AND_QUERIED)
        cor = op->getPointedAndQueriedColor();

      switch( gProp->getGeometryType())
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
        {
          //canvas->setPointMarkerColor(cor);
          te::color::RGBAColor** mark = te::map::CreateMarker(op->getPtMarkerType(), op->getPtWidth(), cor);
          canvas->setPointPattern(mark, op->getPtWidth(), op->getPtWidth());
        }
      }

      canvas->draw(g);

      delete g;
    }
  }
  else
    te::map::LayerRenderer::draw(al, canvas, e, srid);
}
