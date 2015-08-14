#include "AbstractItemModel.h"

#include "../../property/Property.h"

te::layout::AbstractItemModel::AbstractItemModel()
  : Subject()
  , m_properties("")
{
  te::gm::Envelope boundingBox(0., 0., 20., 20.);
  te::color::RGBAColor backgroundColor(0, 0, 0, 0);
  te::color::RGBAColor frameColor(0, 0, 0, 255);
  double rotation = 0;

  m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getObjectUnknown());

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  {
    Property property(0);
    property.setName("id");
    property.setLabel("Id");
    property.setValue(0, dataType->getDataTypeInt());
    property.setEditable(false);
    m_properties.addProperty(property);
  }

  {
    std::string name = "";
    Property property(0);
    property.setName("name");
    property.setLabel("Name");
    property.setValue(name, dataType->getDataTypeString());
    property.setEditable(false);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("x");
    property.setLabel("X");
    property.setValue(boundingBox.getLowerLeftX(), dataType->getDataTypeDouble());
    property.setEditable(false);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("y");
    property.setLabel("Y");
    property.setValue(boundingBox.getLowerLeftY(), dataType->getDataTypeDouble());
    property.setEditable(false);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("width");
    property.setLabel("Width");
    property.setValue(boundingBox.getWidth(), dataType->getDataTypeDouble());
    property.setEditable(false);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("height");
    property.setLabel("Height");
    property.setValue(boundingBox.getHeight(), dataType->getDataTypeDouble());
    property.setEditable(false);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("background_color");
    property.setLabel("Background Color");
    property.setValue(backgroundColor, dataType->getDataTypeColor());
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("frame_color");
    property.setLabel("Frame Color");
    property.setValue(frameColor, dataType->getDataTypeColor());
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("show_frame");
    property.setLabel("show_frame");
    property.setValue(false, dataType->getDataTypeBool());
    property.setMenu(true);
    this->m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("resizable");
    property.setLabel("Resizable");
    property.setValue(true, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }
  
  {
    Property property(0);
    property.setName("rotation");
    property.setLabel("Rotation");
    property.setValue(rotation, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }
}

te::layout::AbstractItemModel::~AbstractItemModel()
{ 
}

const te::layout::Property& te::layout::AbstractItemModel::getProperty(const std::string& propertyName) const
{
  return m_properties.getProperty(propertyName);
}

void te::layout::AbstractItemModel::setProperty(const Property& property)
{
  m_properties.updateProperty(property);
  notify();
}

const te::layout::Properties& te::layout::AbstractItemModel::getProperties() const
{
  return m_properties;
}

void te::layout::AbstractItemModel::setProperties(const Properties& properties)
{
  const std::vector<Property>& vecProperties = properties.getProperties();
  for(unsigned int i = 0; i < vecProperties.size(); ++i)
  {
    const Property& property = vecProperties[i];
    m_properties.updateProperty(property);
  }

  notify();
}
