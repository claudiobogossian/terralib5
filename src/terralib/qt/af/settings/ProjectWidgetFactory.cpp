#include "ProjectWidget.h"
#include "ProjectWidgetFactory.h"

te::qt::af::ProjectWidgetFactory* te::qt::af::ProjectWidgetFactory::sm_factory(0);


void te::qt::af::ProjectWidgetFactory::initialize()
{
  finalize();
  sm_factory = new ProjectWidgetFactory;
}

void te::qt::af::ProjectWidgetFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::qt::af::ProjectWidgetFactory::~ProjectWidgetFactory()
{

}

te::qt::af::AbstractSettingWidget* te::qt::af::ProjectWidgetFactory::build()
{
  return new ProjectWidget;
}

te::qt::af::ProjectWidgetFactory::ProjectWidgetFactory() :
te::qt::af::SettingsWidgetsFactory(QObject::tr("Project").toStdString())
{

}

te::qt::af::AbstractSettingWidget* te::qt::af::ProjectWidgetFactory::create(QWidget* parent)
{
  return new ProjectWidget(parent);
}