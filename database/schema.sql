
CREATE TABLE user (
  id INT UNSIGNED NOT NULL AUTO_INCREMENT,
  name VARCHAR(255) COLLATE utf8mb4_unicode_ci,
  pass VARCHAR(255) COLLATE utf8mb4_unicode_ci,
  balancecash FLOAT NOT NULL DEFAULT 100000,
  PRIMARY KEY (id)
) ENGINE=INNODB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE stock (
  id INT UNSIGNED NOT NULL AUTO_INCREMENT,
  code VARCHAR(255) COLLATE utf8mb4_unicode_ci,
  lastSalePrice FLOAT NOT NULL,
  shares INT NOT NULL,
  PRIMARY KEY (id) 
) ENGINE=INNODB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE transaction (
  id INT UNSIGNED NOT NULL AUTO_INCREMENT,
  userId INT UNSIGNED NOT NULL,
  stockId INT UNSIGNED,
  quantity INT UNSIGNED,
  dateOfTransaction datetime NOT NULL,
  price FLOAT NOT NULL,
  status VARCHAR(255) NOT NULL,
  PRIMARY KEY (id),
  CONSTRAINT FK_transaction_user FOREIGN KEY (userId) REFERENCES user (id) ON DELETE CASCADE,
  CONSTRAINT FK_transaction_stock FOREIGN KEY (stockId) REFERENCES stock (id)
) ENGINE=INNODB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE portfolio (
  userId INT UNSIGNED NOT NULL,
  stockId INT UNSIGNED,
  quantity INT UNSIGNED,
  totalcost FLOAT NOT NULL,
  PRIMARY KEY (userId, stockId),
  CONSTRAINT FK_portfolio_user FOREIGN KEY (userId) REFERENCES user (id) ON DELETE CASCADE,
  CONSTRAINT FK_portfolio_stock FOREIGN KEY (stockId) REFERENCES stock (id)
) ENGINE=INNODB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;


