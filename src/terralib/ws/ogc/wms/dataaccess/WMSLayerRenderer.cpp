#include "WMSLayerRenderer.h"

#include "WMSLayer.h"

#include "../../../../core/translator/Translator.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../../maptools/Canvas.h"
#include "../../../../geometry/Envelope.h"
#include "../../../core/Exception.h"
#include "../../../core/Utils.h"
#include "../../../../srs/Config.h"

te::ws::ogc::wms::WMSLayerRenderer::WMSLayerRenderer()
{

}

te::ws::ogc::wms::WMSLayerRenderer::~WMSLayerRenderer()
{

}

void te::ws::ogc::wms::WMSLayerRenderer::draw(te::map::AbstractLayer *layer, te::map::Canvas *canvas, const te::gm::Envelope &bbox, int srid, const double &scale, bool *cancel)
{

  te::ws::ogc::wms::WMSLayer* wmsLayer = dynamic_cast<te::ws::ogc::wms::WMSLayer*>(layer);

  if(wmsLayer == nullptr)
  {
    throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("Wrong type render type for this layer!"));
  }

  te::gm::Envelope reprojectedBBOX(bbox);

  if((layer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS))
  {
    reprojectedBBOX.transform(srid, wmsLayer->getSRID());
  }
  else if(layer->getSRID() != srid)
  {
    throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The layer or map has no valid SRID!"));
  }

  if(!reprojectedBBOX.intersects(wmsLayer->getExtent()))
    return;

  //Preparing request acording to canvas and bbox.

  te::ws::ogc::wms::WMSGetMapRequest request = wmsLayer->getRequest();

  request.m_width = canvas->getWidth();
  request.m_height = canvas->getHeight();

  te::ws::ogc::wms::BoundingBox boundingBox;
  boundingBox.m_crs = request.m_boundingBox.m_crs;
  boundingBox.m_minX = reprojectedBBOX.getLowerLeftX();
  boundingBox.m_minY = reprojectedBBOX.getLowerLeftY();
  boundingBox.m_maxX = reprojectedBBOX.getUpperRightX();
  boundingBox.m_maxY = reprojectedBBOX.getUpperRightY();

  request.m_boundingBox = boundingBox;

  wmsLayer->setGetMapRequest(request);

  te::ws::ogc::wms::WMSGetMapResponse response = wmsLayer->getMap();

  te::map::ImageType imageType = te::ws::core::FormatToImageType(response.m_format);

  canvas->drawImage(const_cast<char*>(response.m_buffer.c_str()), (std::size_t) response.m_size, imageType);
}
