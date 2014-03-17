#include "QObjectInspectorWindowOutside.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "QLayoutScene.h"
#include "LayoutOutsideModelObservable.h"
#include "LayoutItemObserver.h"
#include "LayoutOutsideObserver.h"
#include "LayoutOutsideController.h"

#include <QGraphicsWidget>

#include "../../../../geometry/Envelope.h"
#include "../../../../../../third-party/qt/propertybrowser/qtvariantproperty.h"
#include "../../../../../../third-party/qt/propertybrowser/qttreepropertybrowser.h"

te::layout::QObjectInspectorWindowOutside::QObjectInspectorWindowOutside( LayoutOutsideController* controller, LayoutOutsideModelObservable* o ) :
	QDockWidget("", 0, 0),
	LayoutOutsideObserver(controller, o)
{
	te::gm::Envelope box = _model->getBox();	
	setFixedSize(box.getWidth(), box.getHeight());
	setVisible(false);
	setWindowTitle("Layout - Inspetor de Objetos");

  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  //
   _variantInspectorObjectManager = new QtVariantPropertyManager(this);
  connect(_variantInspectorObjectManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
  this, SLOT(propertyEditorValueChanged(QtProperty *, const QVariant &)));

  QtVariantEditorFactory* variantObjectInspectorFactory = new QtVariantEditorFactory(this);
  _objectInspector = new QtTreePropertyBrowser(this);
  _objectInspector->setFactoryForManager(_variantInspectorObjectManager, variantObjectInspectorFactory);

  setWidget(_objectInspector);
}

te::layout::QObjectInspectorWindowOutside::~QObjectInspectorWindowOutside()
{

}

void te::layout::QObjectInspectorWindowOutside::updateObserver( ContextLayoutItem context )
{
	setVisible(context.getShow());
	if(context.getShow() == true)
		show();
	else
		hide();
}

void te::layout::QObjectInspectorWindowOutside::setPosition( const double& x, const double& y )
{
	move(x,y);
	refresh();
}

te::gm::Coord2D te::layout::QObjectInspectorWindowOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::QObjectInspectorWindowOutside::propertyEditorValueChanged( QtProperty *property, const QVariant &value )
{
 
}

void te::layout::QObjectInspectorWindowOutside::updateExpandState()
{
  QList<QtBrowserItem *> list = _objectInspector->topLevelItems();
  QListIterator<QtBrowserItem *> it(list);
  while (it.hasNext()) {
    QtBrowserItem *item = it.next();
    QtProperty *prop = item->property();
    _idToExpanded[_propertyToId[prop]] = _objectInspector->isExpanded(item);
  }
}

void te::layout::QObjectInspectorWindowOutside::itemsInspector(QList<QGraphicsItem*> graphicsItems)
{
  updateExpandState();

  QMap<QtProperty *, QString>::ConstIterator itProp = _propertyToId.constBegin();
  while (itProp != _propertyToId.constEnd()) 
  {
    delete itProp.key();
    itProp++;
  }
  _propertyToId.clear();
  _idToProperty.clear();
  _graphicsItems = graphicsItems;
    
  QtVariantProperty *property;
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {			
      QGraphicsWidget* outside = dynamic_cast<QGraphicsWidget*>(item);

      if(outside)
        continue;

      LayoutItemObserver* lItem = dynamic_cast<LayoutItemObserver*>(item);
      if(lItem)
      {
        LayoutProperties properties = lItem->toString();

        foreach(LayoutProperty p, properties.getProperties())
        {
          //item->metaObject()->className(); ->>> have to be QObject or QGraphicsObject too. 

          QGraphicsObject* itemObj = dynamic_cast<QGraphicsObject*>(item);         

          if(!itemObj)
            break;
          
          property = _variantInspectorObjectManager->addProperty(QVariant::String, tr(itemObj->metaObject()->className()));
          property->setValue(p.getLabel().c_str());
          addProperty(property, QLatin1String(p.getName().c_str()));

          property = _variantInspectorObjectManager->addProperty(QVariant::String, tr(itemObj->objectName().toStdString().c_str()));
          property->setValue(p.getLabel().c_str());
          addProperty(property, QLatin1String(p.getName().c_str()));
        }
      }
    }
  }

  update();
}

void te::layout::QObjectInspectorWindowOutside::addProperty(QtVariantProperty *property, const QString &id)
{
  _propertyToId[property] = id;
  _idToProperty[id] = property;
  QtBrowserItem *item = _objectInspector->addProperty(property);
  if (_idToExpanded.contains(id))
    _objectInspector->setExpanded(item, _idToExpanded[id]);
}
