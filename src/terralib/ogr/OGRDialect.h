te::da::SQLDialect* mydialect = new te::da::SQLDialect;

mydialect->insert("+", new te::da::BinaryOpEncoder("+"));
mydialect->insert("-", new te::da::BinaryOpEncoder("-"));
mydialect->insert("*", new te::da::BinaryOpEncoder("*"));
mydialect->insert("/", new te::da::BinaryOpEncoder("/"));
mydialect->insert("=", new te::da::BinaryOpEncoder("="));
mydialect->insert("<>", new te::da::BinaryOpEncoder("<>"));
mydialect->insert(">", new te::da::BinaryOpEncoder(">"));  
mydialect->insert("<", new te::da::BinaryOpEncoder("<"));
mydialect->insert(">=", new te::da::BinaryOpEncoder(">="));
mydialect->insert("<=", new te::da::BinaryOpEncoder("<="));
mydialect->insert("and", new te::da::BinaryOpEncoder("AND"));
mydialect->insert("or", new te::da::BinaryOpEncoder("OR"));
mydialect->insert("not", new te::da::UnaryOpEncoder("NOT"));  

mydialect->insert("st_intersects", new te::da::FunctionEncoder("ST_Intersecs"));

DataSource::setDialect(mydialect);
