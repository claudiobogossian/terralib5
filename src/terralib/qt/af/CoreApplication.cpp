#include "CoreApplication.h"

//! TerraLib include files
#include <terralib/common/Exception.h>

#include <terralib/qt/af/events/NewToolBar.h>

const te::qt::af::CoreApplication& sm_core = te::qt::af::teApp::getInstance();

namespace te
{
  namespace qt
  {
    namespace af
    {
      CoreApplication::CoreApplication(QObject* parent) :
      QObject(parent)
      {
      }

      CoreApplication::~CoreApplication()
      {
      }

      void CoreApplication::addToolBar(const QString& barId, QToolBar* bar)
      {
        registerToolBar(barId, bar);

        //! Send event of tool bar added.
        te::qt::af::NewToolBar evt(bar);
        broadCast(&evt);
      }

      void CoreApplication::registerToolBar(const QString& barId, QToolBar* bar)
      {
        QToolBar* b = getToolBar(barId);

        if(b != 0)
          throw te::common::Exception(tr("There is a tool bar with the same name registered.").toLatin1().data());

        m_toolbars[barId] = bar;
      }

      QToolBar* CoreApplication::getToolBar(const QString& barId)
      {
        std::map<QString, QToolBar*>::iterator it = m_toolbars.find(barId);
        return (it != m_toolbars.end()) ? it->second : 0;
      }

      void CoreApplication::addListener(QObject* obj)
      {
        std::set<QObject*>::iterator it = m_reg_coms.find(obj);

        //! If not found on the set, insert it and connect the slot.
        if(it == m_reg_coms.end())
        {
          m_reg_coms.insert(obj);
          obj->connect(this, SIGNAL(triggered(te::qt::af::Event*)), SLOT(onApplicationTriggered(te::qt::af::Event*)));
        }
      }

      void CoreApplication::removeListener(QObject* obj)
      {
        std::set<QObject*>::iterator it = m_reg_coms.find(obj);

        //! If found on the set, desconnect it.
        if(it != m_reg_coms.end())
        {
          m_reg_coms.erase(it);
          disconnect(SIGNAL(triggered(te::qt::af::Event*)), obj, SLOT(onApplicationTriggered(te::qt::af::Event*)));
        }
      }

      //void CoreApplication::setCurrentTool(te::qt::widgets::AbstractTool* tool)
      //{
      //}

      void CoreApplication::broadCast(te::qt::af::Event* evt)
      {
        //! Need to check event send to prevent loops
        //! -----------------------------------------

        emit triggered(evt);
      }
    }
  }
}