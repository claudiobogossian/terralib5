#ifndef __TERRALIB_EDIT_QT_INTERNAL_SUBTRACTAREATOOL_H
#define __TERRALIB_EDIT_QT_INTERNAL_SUBTRACTAREATOOL_H

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

    class TEEDITQTEXPORT SubtractAreaTool : public CreateLineTool
    {
      Q_OBJECT

    public:
      SubtractAreaTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent = 0);

      ~SubtractAreaTool();

      //@}

      /** @name AbstractTool Methods
      *  Methods related with tool behavior.
      */
      //@{

      bool mousePressEvent(QMouseEvent* e);

      bool mouseMoveEvent(QMouseEvent* e);

      bool mouseDoubleClickEvent(QMouseEvent* e);

      //@}

    private:

      void drawPolygon();

      void draw();

      void reset();

      void storeEditedFeature();

      void storeUndoCommand();

      void pickFeature(const te::map::AbstractLayerPtr& layer);

      te::gm::Envelope buildEnvelope(const QPointF& pos);

      te::gm::Geometry* buildPolygon();

      te::gm::Geometry* Difference(te::gm::Geometry* g1, te::gm::Geometry* g2);

    private slots:

      void onExtentChanged();

    protected:

      std::vector<Feature*> m_updateWatches;

    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_AGGREGATEAREATOOL_H
