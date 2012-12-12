#include "CoreApplication.h"
#include "UserPlugins.h"
#include "ApplicationPlugins.h"
#include "SplashScreenManager.h"

//! TerraLib include files
#include <terralib/common/Exception.h>
#include <terralib/common/Translator.h>
#include <terralib/common/TerraLib.h>
#include <terralib/common/SystemApplicationSettings.h>
#include <terralib/common/UserApplicationSettings.h>
#include <terralib/common/Logger.h>
#include <terralib/plugin/PluginManager.h>
#include <terralib/plugin/PluginInfo.h>

#include <terralib/qt/af/events/NewToolBar.h>

//! Qt include files
#include <QDir>
#include <QApplication>
#include <QIcon>

//! Boost include files
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>


const te::qt::af::CoreApplication& sm_core = te::qt::af::Application::getInstance();

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

b_prop::ptree getPluginRef(const std::string& pluginName, const std::string pluginFileName)
{
  b_prop::ptree tree;

  tree.add("Name", pluginName);
  tree.add("Path.<xmlattr>.xlink:href", pluginFileName);

  return tree;
}

void configureFile(const std::vector< std::pair<std::string, std::string> >& configs, const std::string& fileName)
{
  std::vector< std::pair<std::string, std::string> >::const_iterator it;
  b_prop::ptree p;

  for(it=configs.begin(); it != configs.end(); ++it)
  {
    if(it->first.find("Plugins.Plugin.Name") != std::string::npos)
    {
      std::string plg_name = it->second;
      std::string plg_file_name = (++it)->second;
      p.add_child("Plugins.Plugin", getPluginRef(plg_name, plg_file_name));
    }
    else
      p.add(it->first, it->second);
  }

  boost::property_tree::xml_writer_settings<char> settings('\t', 1);
  boost::property_tree::write_xml(fileName, p, std::locale(), settings);
}

void saveEnabledPlugins(const std::vector<std::string>& enabled_plgs)
{
  std::string settingsFile = te::common::SystemApplicationSettings::getInstance().getValue("Application.UserSettingsFile.<xmlattr>.xlink:href");

  if(!boost::filesystem::is_regular_file(settingsFile))
    return;

  b_prop::ptree new_sett;
  b_prop::ptree plgs;
  b_prop::ptree old_plgs;
  b_prop::ptree only_plgs;
  b_prop::ptree::iterator it;
  std::vector<std::string>::const_iterator it2;

  b_prop::read_xml(settingsFile, new_sett);
  old_plgs = new_sett.get_child("UserSettings");

  //! Empty plugins file
  for(it = old_plgs.begin(); it != old_plgs.end(); ++it)
  {
    if(it->first != "EnabledPlugins")
      plgs.add_child(it->first, it->second);
  }

  for(it2=enabled_plgs.begin(); it2 != enabled_plgs.end(); ++it2)
    only_plgs.add("Plugin", *it2);

  plgs.insert(plgs.to_iterator(plgs.find("DataSourcesFile")), std::pair<std::string, b_prop::ptree>("EnabledPlugins", only_plgs));

  new_sett.put_child("UserSettings", plgs);

  boost::property_tree::xml_writer_settings<char> settings('\t', 1);
  boost::property_tree::write_xml(settingsFile, new_sett, std::locale(), settings);
}

void savePluginsFiles()
{
  b_prop::ptree& new_sett = te::qt::af::ApplicationPlugins::getInstance().getAllSettings();

  b_prop::ptree::iterator it;
  b_prop::ptree plgs;
  b_prop::ptree old_plgs = new_sett.get_child("Plugins");
  std::vector<std::string> enabled_plgs;

  //! Empty plugins file
  for(it = old_plgs.begin(); it != old_plgs.end(); ++it)
  {
    if(it->first != "Plugin")
      plgs.add_child(it->first, it->second);
  }

  //! Now adds the used plug-ins
  std::map<std::string, std::string> plg_files = te::qt::af::ApplicationPlugins::getInstance().getPluginsFiles();
  std::map<std::string, std::string>::iterator it2;

  for(it2=plg_files.begin(); it2!=plg_files.end(); ++it2)
  {
    plgs.add_child("Plugin", getPluginRef(it2->first, it2->second));
    if(te::plugin::PluginManager::getInstance().isLoaded(it2->first))
      enabled_plgs.push_back(it2->first);
  }

  new_sett.put_child("Plugins", plgs);

  boost::property_tree::xml_writer_settings<char> settings('\t', 1);
  boost::property_tree::write_xml(te::common::SystemApplicationSettings::getInstance().getValue("Application.PluginsFile.<xmlattr>.xlink:href"), new_sett, std::locale(), settings);

  saveEnabledPlugins(enabled_plgs);
}

void updatePluginsFiles()
{
  b_prop::ptree& sett = te::qt::af::ApplicationPlugins::getInstance().getAllSettings().get_child("Plugins");
  b_prop::ptree::iterator it;

  for(it = sett.begin(); it != sett.end(); ++it)
  {
    if(it->first == "Plugin")
      te::qt::af::ApplicationPlugins::getInstance().addPlugin(it->second.get_child("Name").data(), it->second.get_child("Path.<xmlattr>.xlink:href").data());
  }
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
        m_initialized = false;
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
        broadcast(&evt);
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
        if(m_initialized)
          return;

        SplashScreenManager::getInstance().showMessage(tr("Initializing TerraLib modules..."));

        std::string exc_msg;

        try
        {
          TerraLib::getInstance().initialize();

          TE_LOG_TRACE(TR_QT_AF("TerraLib Application Framework module initialized!"));

          //! Application configurations file generation
          QString fName = QDir::toNativeSeparators(qApp->applicationDirPath() + "/config.xml");
          fs::path p(fName.toStdString());

          if(!fs::exists(p) || !fs::is_regular_file(p))
          {
            SplashScreenManager::getInstance().showMessage(tr("Creating configuration file for application..."));

            if(!fs::exists(p.branch_path()))
              fs::create_directories(p.branch_path());

            const std::vector< std::pair<std::string, std::string> >* af_configs = te::qt::af::Application::getInstance().getApplicationInfo();

            if(af_configs->empty())
            {
              std::vector< std::pair<std::string, std::string> > configs;
              getDefaultConfigurations(configs);
              configureFile(configs, p.generic_string());
            }
            else
              configureFile(*af_configs, p.generic_string());
          }

          SplashScreenManager::getInstance().showMessage(tr("Loading configuration file..."));
          te::common::SystemApplicationSettings::getInstance().load(p.generic_string());
        
          //! Setting SplashScreenManager
          QFileInfo logo(QDir::toNativeSeparators(te::common::SystemApplicationSettings::getInstance().getValue("Application.TerraLibLogo.<xmlattr>.xlink:href").c_str()));

          if(logo.exists())
            SplashScreenManager::getInstance().setLogo(QPixmap(logo.filePath(), logo.suffix().toLatin1().data()));

          //! Application plug-ins configurations file generation
          fName = QDir::toNativeSeparators(te::common::SystemApplicationSettings::getInstance().getValue("Application.PluginsFile.<xmlattr>.xlink:href").c_str());
          p = fs::path(fName.toStdString());

          if(!fs::exists(p) || !fs::is_regular_file(p))
          {
            SplashScreenManager::getInstance().showMessage(tr("Creating application plug-ins configuration file..."));
            if(!fs::exists(p.branch_path()))
              fs::create_directories(p.branch_path());

            const std::vector< std::pair<std::string, std::string> >* af_configs = te::qt::af::Application::getInstance().getApplicationPlgInfo();

            if(!af_configs->empty())
              configureFile(*af_configs, p.generic_string());
          }

          //! User settings configurations file generation
          fName = QDir::toNativeSeparators(te::common::SystemApplicationSettings::getInstance().getValue("Application.UserSettingsFile.<xmlattr>.xlink:href").c_str());
          p = fs::path(fName.toStdString());

          if(!fs::exists(p) || !fs::is_regular_file(p))
          {
            SplashScreenManager::getInstance().showMessage(tr("Creating user settings configuration file..."));

            if(!fs::exists(p.branch_path()))
              fs::create_directories(p.branch_path());

            const std::vector< std::pair<std::string, std::string> >* af_configs = te::qt::af::Application::getInstance().getUserInfo();

            if(!af_configs->empty())
              configureFile(*af_configs, p.generic_string());
          }

          SplashScreenManager::getInstance().showMessage(tr("Loading user configuration file..."));
          te::common::UserApplicationSettings::getInstance().load(te::common::SystemApplicationSettings::getInstance().getValue("Application.UserSettingsFile.<xmlattr>.xlink:href"));

          SplashScreenManager::getInstance().showMessage(tr("Loading application plug-ins..."));
          ApplicationPlugins::getInstance().load(te::common::SystemApplicationSettings::getInstance().getValue("Application.PluginsFile.<xmlattr>.xlink:href"));

          SplashScreenManager::getInstance().showMessage(tr("Loading user plug-ins..."));
          UserPlugins::getInstance().load();
        }
        catch(te::common::Exception& e)
        {
          SplashScreenManager::getInstance().close();
          exc_msg = QString(tr("Could not completely initialize application framework: \n") + e.what()).toStdString();
        }

        m_app_info.clear();
        m_app_plg_info.clear();
        m_user_info.clear();

        //! Setting application configurations.

        //! Icon theme path
        std::string ithemedir = te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.BaseDirectory.<xmlattr>.xlink:href");
        if(!ithemedir.empty())
        {
          QStringList ithemes = QIcon::themeSearchPaths();
          ithemes.push_back(ithemedir.c_str());
          QIcon::setThemeSearchPaths(ithemes);
        }
        //! Icon theme
        std::string iconTheme = te::common::UserApplicationSettings::getInstance().getValue("UserSettings.SelectedIconTheme");
        if(iconTheme.empty())
          iconTheme = te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.DefaultTheme");
        if(!iconTheme.empty())
          QIcon::setThemeName(iconTheme.c_str());

        //! Icon size
        std::string iconSize = te::common::UserApplicationSettings::getInstance().getValue("UserSettings.ToolBarIconSize");
        if(iconSize.empty())
          iconSize = te::common::SystemApplicationSettings::getInstance().getValue("Application.ToolBarDefaultIconSize");
        if(!iconSize.empty())
        {
          QString sh = QString("QToolBar { qproperty-iconSize: ") + iconSize.c_str() + "px " + iconSize.c_str() + "px; }";
          qApp->setStyleSheet(sh);
        }

        //! Application name
        std::string app_name = te::common::SystemApplicationSettings::getInstance().getValue("Application.Name");
        qApp->setApplicationName(app_name.c_str());

        //! Application organization
        std::string app_org = te::common::SystemApplicationSettings::getInstance().getValue("Application.Organization");
        qApp->setOrganizationName(app_org.c_str());

        m_initialized = true;

        updatePluginsFiles();

        if(!exc_msg.empty())
          throw te::common::Exception(exc_msg);
      }

      void CoreApplication::finalize()
      {
        if(!m_initialized)
          return;

        savePluginsFiles();
        te::plugin::PluginManager::getInstance().shutdownAll();
        te::plugin::PluginManager::getInstance().unloadAll();
        TerraLib::getInstance().finalize();

        m_initialized = false;
      }

      void CoreApplication::broadcast(te::qt::af::Event* evt)
      {
        //! Need to check event send to prevent loops
        //! -----------------------------------------

        emit triggered(evt);
      }
    }
  }
}