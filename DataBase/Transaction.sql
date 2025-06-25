-- Transaction : 트랜섹션

-- 한일이 A 아이템을 가지고 있다. => 민수님한테 A 아이템을 주고 싶다. => 그리고 돈을 받는다.

--{ 한일이 A아이템을 드롭한다. <- 1 thead
-- 민수한테 A아이템을 추가한다.
-- 민수한테 돈을 뺀다.
-- 한일한테 돈을 추가한다. }

USE GameDB;

SELECT *
FROM PlayerAccount;

-- 원자성
-- 일련의 과정들이 단 한번의 수행처럼 수행되는 것
-- Tran 안에는 항상 원자적으로 실행되어야하는 코드들만 와야한다.

-- Commit : 실패하지 않을 경우 반영
BEGIN TRAN
INSERT INTO PlayerAccount VALUES (4, 'Hanil3', 1234, GETUTCDATE());
-- COMMIT : Tran 안에 있는 코드들이 다 OK면 반영

-- RollBack : 실패하면 되돌림
BEGIN TRAN
INSERT INTO PlayerAccount VALUES(5,'Hanil1313', 12341234, GETUTCDATE());
-- ROLLBACK; : Tran안에 있는 코드들 롤백

-- TRY (A) CATCH (B):  (A)를 시도하고, 실패 했을 시 CATCH
BEGIN TRY
	BEGIN TRAN
		INSERT INTO PlayerAccount VALUES(6,'Hanil11', 134234, GETUTCDATE())
		INSERT INTO PlayerAccount VALUES(9,'Minsu3', 3333, GETUTCDATE())
	COMMIT
		-- ALL
END TRY

BEGIN CATCH
	IF @@TRANCOUNT > 0 -- 현재 활성화된 트랜잭션의 수를 반환
		ROLLBACK
		-- NOTHING
END CATCH