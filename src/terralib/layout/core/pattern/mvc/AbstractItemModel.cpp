#include "AbstractItemModel.h"

#include "../../property/Property.h"

te::layout::AbstractItemModel::AbstractItemModel()
: Subject()
, m_properties("")
{
  te::gm::Envelope boundingBox(0., 0., 80., 80.);
  te::color::RGBAColor backgroundColor(255, 255, 255, 0);
  te::color::RGBAColor frameColor(0, 0, 0, 255);
  double rotation = 0;
  int zValue = 0;
  double frameThickness = 0.5; //in mm

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
    property.setName("frame_thickness");
    property.setLabel("Frame Thickness");
    property.setValue(frameThickness, dataType->getDataTypeDouble());
    property.setVisible(false);
    property.setEditable(false);
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
    property.setVisible(false);
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

  {
    Property property(0);
    property.setName("printable");
    property.setLabel("printable");
    property.setVisible(false);
    property.setValue(true, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("zValue");
    property.setLabel("zValue");
    property.setValue(zValue, dataType->getDataTypeInt());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("connect_item_position");
    property.setVisible(false);
    property.setValue(false, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("editable");
    property.setVisible(false);
    property.setValue(false, dataType->getDataTypeBool());
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

void te::layout::AbstractItemModel::setProperty(const te::layout::Property& property)
{
  m_properties.updateProperty(property);
  notify();
}

const te::layout::Properties& te::layout::AbstractItemModel::getProperties() const
{
  return m_properties;
}

void te::layout::AbstractItemModel::setProperties(const te::layout::Properties& properties)
{
  const std::vector<Property>& vecProperties = properties.getProperties();
  for (unsigned int i = 0; i < vecProperties.size(); ++i)
  {
    const Property& property = vecProperties[i];
    m_properties.updateProperty(property);
  }

  notify();
}

te::layout::EnumType* te::layout::AbstractItemModel::getType()
{
  return m_properties.getTypeObj();
}

std::string te::layout::AbstractItemModel::getName()
{
  std::string prop = "name";
  return m_properties.getProperty(prop).getValue().toString();
}

bool te::layout::AbstractItemModel::isPrintable()
{
  std::string propertyName = "printable";
  Property prop_printable = m_properties.getProperty(propertyName);
  return prop_printable.getValue().toBool();
}

te::gm::Envelope te::layout::AbstractItemModel::getBoundingRect()
{
  std::string propertyX = "x";
  std::string propertyY = "y";
  std::string propertyWidth = "width";
  std::string propertyHeight = "height";

  Property prop_x = m_properties.getProperty(propertyX);
  Property prop_y = m_properties.getProperty(propertyY);
  Property prop_width = m_properties.getProperty(propertyWidth);
  Property prop_height = m_properties.getProperty(propertyHeight);

  te::gm::Envelope box;
  box.m_llx = prop_x.getValue().toDouble();
  box.m_lly = prop_y.getValue().toDouble();
  box.m_urx = box.m_llx + prop_width.getValue().toDouble();
  box.m_ury = box.m_lly + prop_height.getValue().toDouble();

  return box;
}

bool te::layout::AbstractItemModel::contains(const te::gm::Coord2D &coord) const
{
  te::gm::Envelope env(coord.x, coord.y, coord.x, coord.y);

  std::string propertyX = "x";
  std::string propertyY = "y";
  std::string propertyWidth = "width";
  std::string propertyHeight = "height";

  Property prop_x = m_properties.getProperty(propertyX);
  Property prop_y = m_properties.getProperty(propertyY);
  Property prop_width = m_properties.getProperty(propertyWidth);
  Property prop_height = m_properties.getProperty(propertyHeight);

  te::gm::Envelope box;
  box.m_llx = prop_x.getValue().toDouble();
  box.m_lly = prop_y.getValue().toDouble();
  box.m_urx = box.m_llx + prop_width.getValue().toDouble();
  box.m_ury = box.m_lly + prop_height.getValue().toDouble();
  
  if (env.isValid())
    return box.contains(env);

  return false;
}

