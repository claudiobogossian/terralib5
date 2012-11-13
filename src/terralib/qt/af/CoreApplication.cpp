#include "CoreApplication.h"
#include "UserPlugins.h"
#include "ApplicationPlugins.h"

//! TerraLib include files
#include <terralib/common/Exception.h>
#include <terralib/common/Translator.h>
#include <terralib/common/TerraLib.h>
#include <terralib/common/SystemApplicationSettings.h>
#include <terralib/common/UserApplicationSettings.h>
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
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xsd:schemaLocation", "http://www.terralib.org/schemas/af " + std::string(TERRALIB_SCHEMA_LOCATION)));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.version", "5.0.0"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.release", "2011-01-01"));
  configs.push_back(std::pair<std::string, std::string>("Application.Organization", "INPE"));
  configs.push_back(std::pair<std::string, std::string>("Application.Name", "TerraLib"));
}

void configureFile(const std::vector< std::pair<std::string, std::string> >& configs, const std::string& fileName)
{
  std::vector< std::pair<std::string, std::string> >::const_iterator it;
  b_prop::ptree p;

  for(it=configs.begin(); it != configs.end(); ++it)
  {
    if(it->first.find("Plugins.Plugin.Name") != std::string::npos)
    {
      b_prop::ptree sub_p;
      sub_p.add("Name", it->second);
      it++;
      sub_p.add("Path.<xmlattr>.xlink:href", it->second);
      p.add_child("Plugins.Plugin", sub_p);
    }
    else
      p.add(it->first, it->second);
  }

  boost::property_tree::xml_writer_settings<char> settings('\t', 1);
  boost::property_tree::write_xml(fileName, p, std::locale(), settings);
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

      void CoreApplication::setApplicationPlgInfo(const std::vector< std::pair<std::string, std::string> >& info)
      {
        m_app_plg_info = info;
      }

      const std::vector< std::pair<std::string, std::string> >* CoreApplication::getApplicationPlgInfo() const
      {
        return &m_app_plg_info;
      }

      void CoreApplication::setUserInfo(const std::vector< std::pair<std::string, std::string> >& info)
      {
        m_user_info = info;
      }

      const std::vector< std::pair<std::string, std::string> >* CoreApplication::getUserInfo() const
      {
        return &m_user_info;
      }

      void CoreApplication::initialize()
      {
        TerraLib::getInstance().initialize();

        TE_LOG_TRACE(TR_QT_AF("TerraLib Application Framework module initialized!"));

        //! Application configurations file generation
        QString fName = QDir::toNativeSeparators(qApp->applicationDirPath() + "/config.xml");
        fs::path p(fName.toStdString());

        if(!fs::exists(p) || !fs::is_regular_file(p))
        {
          if(!fs::exists(p.branch_path()))
            fs::create_directories(p.branch_path());

          const std::vector< std::pair<std::string, std::string> >* af_configs = te::qt::af::teApp::getInstance().getApplicationInfo();

          if(af_configs->empty())
          {
            std::vector< std::pair<std::string, std::string> > configs;
            getDefaultConfigurations(configs);
            configureFile(configs, p.generic_string());
          }
          else
            configureFile(*af_configs, p.generic_string());
        }

        te::common::SystemApplicationSettings::getInstance().load(p.generic_string());

        //! Application plug-ins configurations file generation
        fName = QDir::toNativeSeparators(te::common::SystemApplicationSettings::getInstance().getValue("Application.PluginsFile.<xmlattr>.xlink:href").c_str());
        p = fs::path(fName.toStdString());

        if(!fs::exists(p) || !fs::is_regular_file(p))
        {
          if(!fs::exists(p.branch_path()))
            fs::create_directories(p.branch_path());

          const std::vector< std::pair<std::string, std::string> >* af_configs = te::qt::af::teApp::getInstance().getApplicationPlgInfo();

          if(!af_configs->empty())
            configureFile(*af_configs, p.generic_string());
        }

        //! Application plug-ins configurations file generation
        fName = QDir::toNativeSeparators(te::common::SystemApplicationSettings::getInstance().getValue("Application.UserSettingsFile.<xmlattr>.xlink:href").c_str());
        p = fs::path(fName.toStdString());

        if(!fs::exists(p) || !fs::is_regular_file(p))
        {
          if(!fs::exists(p.branch_path()))
            fs::create_directories(p.branch_path());

          const std::vector< std::pair<std::string, std::string> >* af_configs = te::qt::af::teApp::getInstance().getUserInfo();

          if(!af_configs->empty())
            configureFile(*af_configs, p.generic_string());
        }

        te::common::UserApplicationSettings::getInstance().load(te::common::SystemApplicationSettings::getInstance().getValue("Application.UserSettingsFile.<xmlattr>.xlink:href"));

        ApplicationPlugins::getInstance().load(te::common::SystemApplicationSettings::getInstance().getValue("Application.PluginsFile.<xmlattr>.xlink:href"));

        UserPlugins::getInstance().load();

        m_app_info.clear();
        m_app_plg_info.clear();
        m_user_info.clear();
      }

      void CoreApplication::finalize()
      {
        te::plugin::PluginManager::getInstance().shutdownAll();
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