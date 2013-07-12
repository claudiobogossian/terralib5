#include "TableWidget.h"
#include "TableWidgetFactory.h"

te::qt::af::TableWidgetFactory* te::qt::af::TableWidgetFactory::sm_factory(0);


void te::qt::af::TableWidgetFactory::initialize()
{
  finalize();
  sm_factory = new TableWidgetFactory;
}

void te::qt::af::TableWidgetFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::qt::af::TableWidgetFactory::~TableWidgetFactory()
{

}

te::qt::af::AbstractSettingWidget* te::qt::af::TableWidgetFactory::build()
{
  return new TableWidget;
}

te::qt::af::TableWidgetFactory::TableWidgetFactory() :
te::qt::af::SettingsWidgetsFactory(QObject::tr("Table").toStdString())
{

}

te::qt::af::AbstractSettingWidget* te::qt::af::TableWidgetFactory::create(QWidget* parent)
{
  return new TableWidget(parent);
}