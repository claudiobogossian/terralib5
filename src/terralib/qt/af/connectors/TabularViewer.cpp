#include "TabularViewer.h"

#include <terralib/qt/widgets/dataview/TabularViewer.h>
#include <terralib/qt/widgets/dataview/HLDelegateDecorator.h>

#include <terralib/qt/af/events/LayerEvents.h>
#include <terralib/dataaccess/dataset/DataSet.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceTransactor.h>
#include <terralib/qt/af/ApplicationController.h>
#include <terralib/qt/af/events/PointGeometries.h>
#include <terralib/qt/af/events/StyleChanged.h>

te::da::DataSet* GetDataSet(te::da::DataSource* src, te::da::DataSourceTransactor*& trans, const std::string& dsetName)
{
  if(src != 0 && !dsetName.empty())
  {
  // get a transactor to interact to the data source
    trans = src->getTransactor();

  // retrieve the dataset by its name
    te::da::DataSet* dataset = trans->getDataSet(dsetName);

    return dataset;
  }

  return 0;
}

namespace te
{
  namespace qt
  {
    namespace af
    {
      TabularViewer::TabularViewer(te::qt::widgets::TabularViewer* viewer) :
      QObject(viewer),
      m_viewer(viewer),
      m_dset(0),
      m_trans(0),
      m_layer(0)
      {
        connect(m_viewer, SIGNAL(pointObjects(const std::map<std::string, te::gm::Geometry*>&)), SLOT(pointedObjects(const std::map<std::string, te::gm::Geometry*>&)));
        connect(m_viewer, SIGNAL(groupColorChanged(const int&, const QColor&)), SLOT(colorChanged(const int&, const QColor&)));
      }

      TabularViewer::~TabularViewer()
      {
        delete m_dset;
        delete m_trans;
      }

      void TabularViewer::onApplicationTriggered(te::qt::af::evt::Event* evt)
      {
        if(!m_viewer->isVisible())
          return;

        switch(evt->m_id)
        {
          case te::qt::af::evt::LAYER_SELECTED:
            {
              te::qt::af::evt::LayerSelected* e = static_cast<te::qt::af::evt::LayerSelected*>(evt);
              te::map::AbstractLayer* abs = e->m_layer;

              // * Under revision *
              /*if(abs->getType() == "LAYER")
              {
                te::map::Layer* lay = static_cast<te::map::Layer*>(abs);
                te::da::DataSourceTransactor* trans;

                te::da::DataSet* dset = getDataSet(lay->getDataSource(), trans, lay->getDataSetName());

                if (dset != 0)
                  m_viewer->showData(dset);

                delete m_dset;
                delete m_trans;

                m_dset = dset;
                m_trans = trans;
                m_layer = lay;

                updateColors(m_layer);
              }*/
            }
          break;

          default:
          break;
        }
      }

      void TabularViewer::pointedObjects(const std::map<std::string, te::gm::Geometry*>& geoms)
      {
        te::qt::af::evt::PointGeometries evt(m_layer, &geoms);
        ApplicationController::getInstance().broadcast(&evt);
      }

      void TabularViewer::colorChanged(const int& g, const QColor& c)
      {
        std::vector<QColor>* cs = getColorVector(m_layer);

        if(m_layer == 0 || cs == 0)
          return;

        (*cs)[g] = c;

        te::qt::af::evt::StyleChanged evt(m_layer, g, &c);
        ApplicationController::getInstance().broadcast(&evt);
      }

      void TabularViewer::updateColors(te::map::Layer* layer)
      {
        te::qt::widgets::HLDelegateDecorator* del = dynamic_cast<te::qt::widgets::HLDelegateDecorator*>(m_viewer->itemDelegate());

        if(del == 0)
          return;

        std::vector<QColor>* cs = getColorVector(layer);

        if(cs == 0)
        {
          size_t clss = del->getNumberOfClasses();
          std::vector<QColor> nCs;

          for(size_t i=0; i<clss; i++)
            nCs.push_back(del->getDecorated(i)->getHighlightColor());

          if(!nCs.empty())
            m_colors_map[layer] = nCs;
        }
        else
          for(size_t i=0; i<cs->size(); i++)
            m_viewer->setHighlightColor((int)i, (*cs)[i], false);
      }

      std::vector<QColor>* TabularViewer::getColorVector(te::map::Layer* layer)
      {
        std::map< te::map::Layer*, std::vector<QColor> >::iterator it = m_colors_map.find(layer);

        if(it != m_colors_map.end())
          return &it->second;

        return 0;
      }
    }
  }
}