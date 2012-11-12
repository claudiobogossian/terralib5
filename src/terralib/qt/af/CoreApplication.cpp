#include "CoreApplication.h"
#include "UserPlugins.h"
#include "ApplicationPlugins.h"

//! TerraLib include files
#include <terralib/common/Exception.h>
#include <terralib/common/Translator.h>
#include <terralib/common/TerraLib.h>
#include <terralib/common/SystemApplicationSettings.h>
#include <terralib/common/Logger.h>
#include <terralib/plugin/PluginManager.h>

#include <terralib/qt/af/events/NewToolBar.h>

//! Qt include files
#include <QDir>
#include <QApplication>

//! Boost include files
#include <boost/filesystem.hpp>

const te::qt::af::CoreApplication& sm_core = te::qt::af::teApp::getInstance();

namespace fs = boost::filesystem;
namespace b_prop = boost::property_tree;

void getDefaultConfigurations(std::vector< std::pair<std::string, std::string> >& configs)
{
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xmlns", "http://www.terralib.org/schemas/af"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xsd:schemaLocation", "http://www.terralib.org/schemas/af ../myschemas/terralib/af/af.xsd"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.version", "5.0.0"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.release", "2011-01-01"));
  configs.push_back(std::pair<std::string, std::string>("Application.Organization", "INPE"));
  configs.push_back(std::pair<std::string, std::string>("Application.Name", "TerraLib"));
}

void configureSystem(const std::vector< std::pair<std::string, std::string> >& configs)
{
  std::vector< std::pair<std::string, std::string> >::const_iterator it;
  b_prop::ptree* p = &te::common::SystemApplicationSettings::getInstance().getAllSettings();

  for(it=configs.begin(); it != configs.end(); ++it)
    p->add(it->first, it->second);

  te::common::SystemApplicationSettings::getInstance().changed();
}

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
        finalize();
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

      void CoreApplication::setApplicationInfo(const std::vector< std::pair<std::string, std::string> >& info)
      {
        m_app_info = info;
      }

      const std::vector< std::pair<std::string, std::string> >* CoreApplication::getApplicationInfo() const
      {
        return &m_app_info;
      }

      void CoreApplication::initialize()
      {
        TerraLib::getInstance().initialize();

        TE_LOG_TRACE(TR_QT_AF("TerraLib Application Framework module initialized!"));

        QString fName = QDir::toNativeSeparators(qApp->applicationDirPath() + "/config.xml");
        fs::path p(fName.toStdString());

        if(fs::exists(p) && fs::is_regular_file(p))
          te::common::SystemApplicationSettings::getInstance().load(p.generic_string());
        else
        {
          if(!fs::exists(p.branch_path()))
            fs::create_directories(p.branch_path());

          te::common::SystemApplicationSettings::getInstance().load(p.generic_string());

          const std::vector< std::pair<std::string, std::string> >* af_configs = te::qt::af::teApp::getInstance().getApplicationInfo();

          if(af_configs->empty())
          {
            std::vector< std::pair<std::string, std::string> > configs;
            getDefaultConfigurations(configs);
            configureSystem(configs);
          }
          else
            configureSystem(*af_configs);

          te::common::SystemApplicationSettings::getInstance().update();

//          te::common::UserApplicationSettings::getInstance().load();
        //  SplashScreenManager::getInstance().showMessage(tr("User application settings loaded!"));
        }

        ApplicationPlugins::getInstance().load(te::common::SystemApplicationSettings::getInstance().getValue("Application.PluginsFile.<xmlattr>.xlink:href"));
//        SplashScreenManager::getInstance().showMessage(tr("Application plugin list loaded!"));

//        UserPlugins::getInstance();
        UserPlugins::getInstance().load();
      }

      void CoreApplication::finalize()
      {
        te::plugin::PluginManager::getInstance().unloadAll();
        TerraLib::getInstance().finalize();
      }

      void CoreApplication::broadCast(te::qt::af::Event* evt)
      {
        //! Need to check event send to prevent loops
        //! -----------------------------------------

        emit triggered(evt);
      }
    }
  }
}