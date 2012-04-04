#ifndef __MY_LAYER_H_
#define __MY_LAYER_H_

#include <terralib/maptools.h>

class MyLayer : public te::map::Layer
{
public:
  MyLayer(const std::string& id, const std::string& title, AbstractLayer* parent = 0);
  ~MyLayer();

  void setDataGridOperation(te::map::DataGridOperation* op);
  te::map::DataGridOperation* getDataGridOperation();
  void setTemporal(bool);
  bool isTemporal();

private:
  te::map::DataGridOperation* m_op;
  bool m_temporal;
};

#endif
