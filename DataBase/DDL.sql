
-- DDL (Database Definition Language)

CREATE DATABASE GameDB;

USE GameDB;

-- CREATE
CREATE TABLE PlayerAccount
(
	accountID INTEGER NOT NULL,
	accountName VARCHAR(10) NOT NULL,
	accountMoney INTEGER DEFAULT 0,
	accountCreateTime DATETIME,
)

INSERT INTO PlayerAccount(accountID, accountName, accountMoney, accountCreateTime)
VALUES (1,'Hanil',1000,GETDATE());

INSERT INTO PlayerAccount(accountID, accountName, accountMoney, accountCreateTime)
VALUES (2,'Hanil123',5000,GETDATE());

INSERT INTO PlayerAccount(accountID, accountName, accountMoney, accountCreateTime)
VALUES (3,'Hanil2',100000,GETDATE());

DELETE FROM PlayerAccount
WHERE accountCreateTime IS NULL;

SELECT *
FROM PlayerAccount;

-- ALTER : 테이블 디자인 변경
ALTER TABLE PlayerAccount
ADD CONSTRAINT PK_accountID PRIMARY KEY(accountID)

ALTER TABLE PlayerAccount
DROP CONSTRAINT PK_accountID;

ALTER TABLE PlayerAccount
ADD CONSTRAINT PK_accountName PRIMARY KEY(accountID,accountName)

-- DROP
DROP TABLE PlayerAccount;
