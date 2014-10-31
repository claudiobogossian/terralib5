-------------------------------------------------------
-- import TerraLib module and do some initialization --
-------------------------------------------------------
require ( "terralib_mod_binding_lua" )

---------------------------
-- using Data Access module --
---------------------------

-- Init TerraLib
local t = terralib_mod_binding_lua.TeSingleton.getInstance()
t:initialize()

-- Load plugins
local pmger = terralib_mod_binding_lua.te.plugin.PluginManager.getInstance()
pmger:loadAll()

-- Open a folder containing shape files
local ds = terralib_mod_binding_lua.te.da.DataSourceFactory.make("OGR")
local dsInfo = { URI="D:/funcate/data/brazil.shapefiles.edited" }
ds:setConnectionInfo(dsInfo) 
ds:open()

-- Get datasets names
dNames = { ds:getDataSetNames() }

-- Print datasets names
print("Datasets:\n---------\n\n")

for k, v in pairs(dNames) do
  print(v)
end

-- Destroy dataset
ds = nil
collectgarbage("collect")

-- Finalize plugins and TerraLib
pmger:shutdownAll()
pmger:unloadAll()
pmger:clear()

t:finalize()