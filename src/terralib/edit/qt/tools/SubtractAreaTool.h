#ifndef __TERRALIB_EDIT_QT_INTERNAL_SUBTRACTAREATOOL_H
#define __TERRALIB_EDIT_QT_INTERNAL_SUBTRACTAREATOOL_H

// TerraLib
#include "../../../geometry/Envelope.h"
#ifndef Q_MOC_RUN
#include "../../../maptools/AbstractLayer.h"
#endif
#include "../Config.h"

// Qt
#include <QPointF>

#ifndef Q_MOC_RUN
#include "CreateLineTool.h"
#endif

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

      void storeFeature();

      void pickFeature(const te::map::AbstractLayerPtr& layer, const QPointF& pos);

      te::gm::Envelope buildEnvelope(const QPointF& pos);

      te::gm::Geometry* buildPolygon();

      te::gm::Geometry* differenceGeometry(te::gm::Geometry* g1, te::gm::Geometry* g2);

      std::set<std::string> m_oidsSet;

    private slots:

      void onExtentChanged();

    protected:

      std::vector<Feature*> m_updateWatches;

    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_AGGREGATEAREATOOL_H
