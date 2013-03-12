#include "LayerInfoItem.h"
#include "../../widgets/propertybrowser/AbstractPropertyManager.h"

#include "../../../../maptools/AbstractLayer.h"

te::qt::widgets::LayerInfoItem::LayerInfoItem(QtTreePropertyBrowser* pb, te::map::AbstractLayer* layer) :
  te::qt::widgets::AbstractPropertyItem(pb),
  m_layer(layer)
{
  QtProperty* layerInfo_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Layer properties"));

  // Attributes
  QtProperty* id_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Id"));
  layerInfo_prop->addSubProperty(id_prop);

  QtProperty* title_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Title"));
  layerInfo_prop->addSubProperty(title_prop);

  QtProperty* srid_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->addProperty(tr("SRID"));
  layerInfo_prop->addSubProperty(srid_prop);

  /// Bounding Box
  QtProperty* bbox_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Bounding box"));

  QtProperty* llx_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->addProperty(tr("Lower left x"));
  QtProperty* lly_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->addProperty(tr("Lower left y"));
  QtProperty* urx_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->addProperty(tr("Upper right x"));
  QtProperty* ury_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->addProperty(tr("Upper right y"));

  llx_prop->setEnabled(false);
  lly_prop->setEnabled(false);
  urx_prop->setEnabled(false);
  ury_prop->setEnabled(false);

  bbox_prop->addSubProperty(llx_prop);
  bbox_prop->addSubProperty(lly_prop);
  bbox_prop->addSubProperty(urx_prop);
  bbox_prop->addSubProperty(ury_prop);

  layerInfo_prop->addSubProperty(bbox_prop);

  // Visibility
  QtProperty* vis_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->addProperty(tr("Visibility"));
  QStringList visEnum;
  visEnum << tr("Not visible") << tr("visible") << tr("Partially visible");
  te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setEnumNames(vis_prop, visEnum);

  layerInfo_prop->addSubProperty(vis_prop);

  //setting values
  te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(id_prop, m_layer->getId().c_str());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(title_prop, m_layer->getTitle().c_str());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setValue(id_prop, m_layer->getSRID());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(llx_prop, m_layer->getExtent().getLowerLeftX());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(lly_prop, m_layer->getExtent().getLowerLeftY());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(urx_prop, m_layer->getExtent().getUpperRightX());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(ury_prop, m_layer->getExtent().getUpperRightY());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setValue(vis_prop, (int) m_layer->getVisibility());
  
  addProperty(layerInfo_prop, tr("Layer properties"), Qt::gray);
}

te::qt::widgets::LayerInfoItem::~LayerInfoItem()
{
}

void te::qt::widgets::LayerInfoItem::valueChanged(QtProperty* p, int value)
{
}
