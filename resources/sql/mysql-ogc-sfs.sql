CREATE TABLE spatial_ref_sys
(
  srid      INT          NOT NULL PRIMARY KEY,
  auth_name VARCHAR(256),
  auth_srid INT,
  srtext    VARCHAR(2048)
) ENGINE = InnoDB;

INSERT INTO spatial_ref_sys VALUES(0, "", 0, "");
--INSERT INTO spatial_ref_sys VALUES(4326, "", 4326, "");

CREATE TABLE geometry_columns
(
  f_table_catalog   VARCHAR(256) NULL,
  f_table_schema    VARCHAR(256) NULL,
  f_table_name      VARCHAR(256) NOT NULL,
  f_geometry_column VARCHAR(256) NOT NULL,
  coord_dimension   INT,
  srid              INT,
  type              VARCHAR(256) NOT NULL,
  FOREIGN KEY(srid) REFERENCES spatial_ref_sys(srid)
) ENGINE = InnoDB;

