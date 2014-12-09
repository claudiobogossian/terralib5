-------------------------------------------------------
-- import TerraLib module and do some initialization --
-------------------------------------------------------
require ( "terralib_mod_binding_lua" )

---------------------------
-- using Data Access module --
---------------------------

-- Function for print the data set header
function PrintDataSetHeader(dSet)
  nP = dSet:getNumProperties()

  for prop=0, nP-1 do
    print("|" .. dSet:getPropertyName(prop) .. "|")
  end

  print("\n")
end

-- Function for print data set
function PrintDataSet(dSet)
  nP = dSet:getNumProperties()

  PrintDataSetHeader(dSet)

  dSet:moveBeforeFirst()

  while dSet:moveNext() do
    for prop=0, nP-1 do
	  if not dSet:isNull(prop) then
	    print("|" .. dSet:getAsString(prop) .. "|")
	  end
	end

	print("\n")
  end
end

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

-- Opening first data set of the list
dsNames = { ds:getDataSetNames() } 
dsName = dsNames[1]

local dset = ds:getDataSet(dsName)

-- Print data set
PrintDataSet(dset)

-- Destroy dataset
ds = nil
dset = nil

collectgarbage("collect")

-- Finalize plugins and TerraLib
pmger:shutdownAll()
pmger:unloadAll()
pmger:clear()

t:finalize()