-- package.path = package.path .. ";/Users/gribeiro/MyDevel/dpi/terralib5/codebase-develop/lua/terralib/?.lua"

te = require ("terralib")

pt = te.Point(-45.5, -23.5, 4326)

print("srid:", pt:getSRID(), "longitude:", pt:getX(), "latitude:", pt:getY())

pt:transform(4674)

print("srid:", pt:getSRID(), "longitude:", pt:getX(), "latitude:", pt:getY())

