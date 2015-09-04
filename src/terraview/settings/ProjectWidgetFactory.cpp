#include "ProjectWidget.h"
#include "ProjectWidgetFactory.h"

ProjectWidgetFactory* ProjectWidgetFactory::sm_factory(0);


void ProjectWidgetFactory::initialize()
{
  finalize();
  sm_factory = new ProjectWidgetFactory;
}

void ProjectWidgetFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

ProjectWidgetFactory::~ProjectWidgetFactory()
{

}

te::qt::af::AbstractSettingWidget* ProjectWidgetFactory::build()
{
  return new ProjectWidget;
}

ProjectWidgetFactory::ProjectWidgetFactory() :
te::qt::af::SettingsWidgetsFactory(QObject::tr("Project").toStdString())
{

}

te::qt::af::AbstractSettingWidget* ProjectWidgetFactory::create(QWidget* parent)
{
  return new ProjectWidget(parent);
}