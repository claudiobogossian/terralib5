/*
 * File: Maptools.i
 *
 * Map Tools module specific to Lua
 */
 
%extend te::map::DataSetLayer {
  static DataSetLayer* toDataSetLayer(AbstractLayer* layer)
  {
    return (te::map::DataSetLayer*)layer;
  }
};