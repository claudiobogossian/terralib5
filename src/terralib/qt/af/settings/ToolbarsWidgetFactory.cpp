#include "ToolbarsWidget.h"
#include "ToolbarsWidgetFactory.h"

te::qt::af::ToolbarsWidgetFactory* te::qt::af::ToolbarsWidgetFactory::sm_factory(0);

void te::qt::af::ToolbarsWidgetFactory::initialize()
{
  finalize();
  sm_factory = new ToolbarsWidgetFactory;
}

void te::qt::af::ToolbarsWidgetFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::qt::af::ToolbarsWidgetFactory::~ToolbarsWidgetFactory()
{

}

te::qt::af::AbstractSettingWidget* te::qt::af::ToolbarsWidgetFactory::build()
{
  return new ToolbarsWidget;
}

te::qt::af::ToolbarsWidgetFactory::ToolbarsWidgetFactory() :
te::qt::af::SettingsWidgetsFactory(QObject::tr("Tool bars").toStdString())
{

}

te::qt::af::AbstractSettingWidget* te::qt::af::ToolbarsWidgetFactory::create(QWidget* parent)
{
  return new ToolbarsWidget(parent);
}