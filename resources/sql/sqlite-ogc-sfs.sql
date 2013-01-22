CREATE TABLE geometry_columns
(
  f_table_name      VARCHAR,
  f_geometry_column VARCHAR,
  geometry_type     INTEGER,
  coord_dimension   INTEGER,
  srid              INTEGER,
  geometry_format   VARCHAR
);

CREATE TABLE spatial_ref_sys
(
  srid         INTEGER UNIQUE,
  auth_name    TEXT,
  auth_srid    TEXT,
  srtext TEXT
);
