#ifndef __TERRALIB_EDIT_QT_INTERNAL_EDITINFOTOOL_H
#define __TERRALIB_EDIT_QT_INTERNAL_EDITINFOTOOL_H

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"
#include "GeometriesUpdateTool.h"

#include "../Config.h"

// Qt
#include <QPointF>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDialog>

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

    class TEEDITQTEXPORT EditInfoTool : public GeometriesUpdateTool
    {
      Q_OBJECT

    public:
      EditInfoTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent = 0);

      ~EditInfoTool();

      //@}

      /** @name AbstractTool Methods
      *  Methods related with tool behavior.
      */
      //@{

      //bool mouseReleaseEvent(QMouseEvent* e);
      bool mousePressEvent(QMouseEvent* e);

      //@}

    private:

      void draw();

      void reset();

      void updateCursor();

      void pickFeature(const te::map::AbstractLayerPtr& layer, const QPointF& pos);

      te::gm::Envelope buildEnvelope(const QPointF& pos);

      void getInfo(const te::gm::Envelope& e);

      std::auto_ptr<te::dt::AbstractData> getValue(int type, QString value) const;

      te::da::DataSet* m_dataset;                           //!< The attributes dataset type.
      std::map<std::size_t, te::dt::AbstractData*> m_data;
      std::vector<std::size_t> m_restrictivePropertyPos;
      QDialog* m_dialog;
      QTreeWidget* m_infoWidget;                            //!< Widget used to show the informations.
      bool m_forceSearch;

    private slots:

      void onCancelPushButtonPressed();

      void onOkPushButtonPressed();

      void onAttributesTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column);

    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_EDITINFOTOOL_H
