#ifndef QOBJECTINSPECTOR_WINDOWOUTSIDE_H
#define QOBJECTINSPECTOR_WINDOWOUTSIDE_H

#include <QDockWidget>
#include "LayoutOutsideObserver.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../../../third-party/qt/propertybrowser/qtpropertybrowser.h"

class QGraphicsItem;
class QWidget;
class QGraphicsProxyWidget;

class QtTreePropertyBrowser;
class QtVariantProperty;
class QtProperty;
class QtVariantPropertyManager;

class QtBrowserIndex;

namespace te
{
  namespace layout
  {
    class QObjectInspectorWindowOutside : public QDockWidget, public LayoutOutsideObserver
    {
	    Q_OBJECT //for slots/signals

    public:

	    QObjectInspectorWindowOutside(LayoutOutsideController* controller, LayoutOutsideModelObservable* o);
	    virtual ~QObjectInspectorWindowOutside();

	    virtual void updateObserver(ContextLayoutItem context);
	    virtual void setPosition(const double& x, const double& y);
	    virtual te::gm::Coord2D getPosition();

      virtual void itemsInspector(QList<QGraphicsItem*> graphicsItems);

      private slots:

        void propertyEditorValueChanged(QtProperty *property, const QVariant &value);
        
    protected:
      virtual void addProperty(QtVariantProperty *property, const QString &id);
      virtual void updateExpandState();

    protected:

      QtVariantPropertyManager* _variantInspectorObjectManager;
      QtTreePropertyBrowser* _objectInspector;
      
      QMap<QtProperty*, QString> _propertyToId;
      QMap<QString, QtVariantProperty*> _idToProperty;
      QMap<QString, bool> _idToExpanded;
      QList<QGraphicsItem*> _graphicsItems;
    };
  }
}


#endif
