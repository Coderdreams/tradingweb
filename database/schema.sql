
CREATE TABLE user (
  id INT unsigned NOT NULL auto_increment,
  name VARCHAR(255) COLLATE utf8mb4_unicode_ci,
  pass VARCHAR(255) COLLATE utf8mb4_unicode_ci,
  PRIMARY KEY (id)
) ENGINE=INNODB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
