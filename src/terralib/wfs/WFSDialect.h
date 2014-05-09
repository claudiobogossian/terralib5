te::da::SQLDialect* dialect = new te::da::SQLDialect;

dialect->insert("+", new te::da::BinaryOpEncoder("+"));
dialect->insert("-", new te::da::BinaryOpEncoder("-"));
dialect->insert("*", new te::da::BinaryOpEncoder("*"));
dialect->insert("/", new te::da::BinaryOpEncoder("/"));
dialect->insert("=", new te::da::BinaryOpEncoder("="));
dialect->insert("<>", new te::da::BinaryOpEncoder("<>"));
dialect->insert(">", new te::da::BinaryOpEncoder(">"));  
dialect->insert("<", new te::da::BinaryOpEncoder("<"));
dialect->insert(">=", new te::da::BinaryOpEncoder(">="));
dialect->insert("<=", new te::da::BinaryOpEncoder("<="));
dialect->insert("and", new te::da::BinaryOpEncoder("AND"));
dialect->insert("or", new te::da::BinaryOpEncoder("OR"));
dialect->insert("not", new te::da::UnaryOpEncoder("NOT"));
dialect->insert("st_envelopeintersects", new te::da::FunctionEncoder("Intersection"));

DataSource::setDialect(dialect);
