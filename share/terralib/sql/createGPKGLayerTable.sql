CREATE TABLE IF NOT EXISTS tm_layer_form (
tm_conf_id INTEGER PRIMARY KEY AUTOINCREMENT,
gpkg_layer_identify TEXT NOT NULL,
tm_form TEXT,
CONSTRAINT fk_layer_identify_id FOREIGN KEY (gpkg_layer_identify) REFERENCES gpkg_contents(table_name)
);
