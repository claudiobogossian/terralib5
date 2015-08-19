#ifndef __TERRALIB_EDIT_QT_INTERNAL_AGGREGATEAREATOOL_H
#define __TERRALIB_EDIT_QT_INTERNAL_AGGREGATEAREATOOL_H

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// Qt
#include <QPointF>

#include "CreateLineTool.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class Canvas;
      class MapDisplay;
    }
  }

  namespace edit
  {

    class TEEDITQTEXPORT AggregateAreaTool : public CreateLineTool
    {
    Q_OBJECT

    public:
      AggregateAreaTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent = 0);

      ~AggregateAreaTool();

      //@}

      /** @name AbstractTool Methods
      *  Methods related with tool behavior.
      */
      //@{

      bool mousePressEvent(QMouseEvent* e);

      bool mouseMoveEvent(QMouseEvent* e);

      bool mouseReleaseEvent(QMouseEvent* e);

      bool mouseDoubleClickEvent(QMouseEvent* e);

      //@}

    private:
      void drawPolygon();
      void draw(bool drawline);
      void reset();
      void storeEditedFeature();
      void storeUndoCommand();

      void pickFeature(const te::map::AbstractLayerPtr& layer);

      te::gm::Envelope buildEnvelope(const QPointF& pos);

      te::gm::Geometry* buildPolygon();

      te::gm::Geometry* Union(te::gm::Geometry* g1, Feature* feature_g2);

    private slots:

      void onExtentChanged();

    protected:

      Feature* m_feature;
      //std::map<te::da::ObjectId*, std::vector<te::gm::Geometry*> > m_updateWatches;

    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_AGGREGATEAREATOOL_H
