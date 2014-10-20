-------------------------------------------------------
-- import TerraLib module and do some initialization --
-------------------------------------------------------
require ( "terralib_mod_binding_lua" )

---------------------------
-- using Data Access module --
---------------------------

local t = terralib_mod_binding_lua.TeSingleton.getInstance()
t:initialize()

local pmger = terralib_mod_binding_lua.PluginManager.getInstance()
pmger:loadAll()

local ds = terralib_mod_binding_lua.DataSourceFactory.make("OGR")

local dsInfo = { URI="D:/funcate/data/brazil.shapefiles.edited" }

ds:setConnectionInfo(dsInfo) 

ds = nil

collectgarbage("collect")

pmger:shutdownAll()
pmger:unloadAll()
pmger:clear()

t:finalize()