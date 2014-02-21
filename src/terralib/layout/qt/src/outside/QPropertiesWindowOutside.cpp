#include "QPropertiesWindowOutside.h"
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

te::layout::QPropertiesWindowOutside::QPropertiesWindowOutside( LayoutOutsideController* controller, LayoutOutsideModelObservable* o ) :
	QDockWidget("", 0, 0),
	LayoutOutsideObserver(controller, o)
{
	te::gm::Envelope box = _model->getBox();	
	setFixedSize(box.getWidth(), box.getHeight());
	setVisible(false);
	setWindowTitle("Layout - Propriedades");

  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  //
  _variantPropertyEditorManager = new QtVariantPropertyManager(this);
  connect(_variantPropertyEditorManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
    this, SLOT(propertyEditorValueChanged(QtProperty *, const QVariant &)));

  QtVariantEditorFactory* variantPropertyEditorFactory = new QtVariantEditorFactory(this);
  _propertyEditor = new QtTreePropertyBrowser(this);
  _propertyEditor->setFactoryForManager(_variantPropertyEditorManager, variantPropertyEditorFactory);
  
  setWidget(_propertyEditor);
}

te::layout::QPropertiesWindowOutside::~QPropertiesWindowOutside()
{

}

void te::layout::QPropertiesWindowOutside::updateObserver( ContextLayoutItem context )
{
	setVisible(context.getShow());
	if(context.getShow() == true)
		show();
	else
		hide();
}

void te::layout::QPropertiesWindowOutside::setPosition( const double& x, const double& y )
{
	move(x,y);
	refresh();
}

te::gm::Coord2D te::layout::QPropertiesWindowOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::QPropertiesWindowOutside::propertyEditorValueChanged( QtProperty *property, const QVariant &value )
{
 
}

void te::layout::QPropertiesWindowOutside::updateExpandState()
{
  QList<QtBrowserItem *> list = _propertyEditor->topLevelItems();
  QListIterator<QtBrowserItem *> it(list);
  while (it.hasNext()) {
    QtBrowserItem *item = it.next();
    QtProperty *prop = item->property();
    _idToExpanded[_propertyToId[prop]] = _propertyEditor->isExpanded(item);
  }
}

void te::layout::QPropertiesWindowOutside::itemsSelected(QList<QGraphicsItem*> graphicsItems)
{
  updateExpandState();

  QMap<QtProperty *, QString>::ConstIterator itProp = _propertyToId.constBegin();
  while (itProp != _propertyToId.constEnd()) {
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
          property = _variantPropertyEditorManager->addProperty(QVariant::String, tr(p.getName().c_str()));
          property->setValue(p.getLabel().c_str());
          addProperty(property, QLatin1String(p.getName().c_str()));

          //Show up a combobox for change values
          property = _variantPropertyEditorManager->addProperty(QVariant::Double, tr("Position X"));
          property->setAttribute(QLatin1String("minimum"), 0);
          property->setAttribute(QLatin1String("maximum"), 20);
          property->setValue(item->x());
          addProperty(property, QLatin1String("xpos"));
        }
      }
    }
  }

  update();
}

void te::layout::QPropertiesWindowOutside::addProperty(QtVariantProperty *property, const QString &id)
{
  _propertyToId[property] = id;
  _idToProperty[id] = property;
  QtBrowserItem *item = _propertyEditor->addProperty(property);
  if (_idToExpanded.contains(id))
    _propertyEditor->setExpanded(item, _idToExpanded[id]);
}
