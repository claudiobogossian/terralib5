// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../common/Logger.h"
#include "../../../../plugin/Plugin.h"
#include "../../../af/ApplicationController.h"
 
// Qt
#include <QAction>
#include <QObject>
#include <QMenu>
#include <QMenuBar>
 
class Plugin : public QObject, public te::plugin::Plugin
{
  Q_OBJECT
 
  public:
 
    Plugin(const te::plugin::PluginInfo& pluginInfo);
 
    ~Plugin();
 
    void startup();
 
    void shutdown();
 
  protected slots:
 
    void showWindow();
 
  protected:
 
    QAction* m_showWindow;
 
};
 
Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : QObject(), te::plugin::Plugin(pluginInfo), m_showWindow(0)
{
}
 
Plugin::~Plugin() 
{
}
 
void Plugin::startup()
{
  if(m_initialized)
    return;
 
  m_initialized = true;
}
 
void Plugin::shutdown()
{
  if(!m_initialized)
    return;
 
  m_initialized = false;
}
 
void Plugin::showWindow()
{
}
 
PLUGIN_CALL_BACK_DECLARATION(TEQTPLUGINMNTEXPORT);
PLUGIN_CALL_BACK_IMPL(Plugin)