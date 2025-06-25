
USE GameDB;

DECLARE @i INT;
SET @i = 1000;

WHILE(@i <= 10000)
BEGIN
	INSERT INTO PlayerAccount
	VALUES(@i, 'Hanil' + CONVERT(VARCHAR, @i), @i * 1000, GETUTCDATE());
	SET @i = @i + 1;
END

SELECT *
FROM PlayerAccount;

-- 캐시 : 
-- CPU
--  - 레지스터 : 계산 결과를 담아놓는 저장소
--  - 캐시 : 메모리 번지 수를 잠깐 기억하는 곳, 빈번하게 쓰는 메모리 번지수 저장

-- Index (B- 트리 : 자가균형이진탐색트리) 
-- Clustered Index
-- 영한사전 : 실제 데이터가 Index 순으로 정렬
-- => 여러개 존재할 수 없다.

-- Non Clustered Index
-- 색인 : 따로 어떤 테이블(B- 트리) 생긴다.
-- 많으면 많을 수록 데이터를 잡아먹는데, 보통 캐싱을 해놓는다 => 메모리를 많이 잡아먹을 수 있다.
-- 넌클러스티드 인덱스만 걸었을 시에, Heap 테이블이 생긴다.
--  => 넌클러스티드 인덱스는 클러스터디드 인덱스의 영향을 받는다.

-- Index Scan vs Index Seek
-- Index Seek만 써도 되는 거 아닐까?
-- 상황에 따라 Scan이 더 좋을 수도 있다.
-- => BOOKMARK LOOK UP : RID로 HEAP TABLE에서 찾기

CREATE INDEX index_name ON PlayerAccount(accountName);
DROP INDEX index_name ON PlayerAccount;

CREATE CLUSTERED INDEX index_id ON PlayerAccount(accountID);
DROP INDEX index_id ON PlayerAccount;

-- INDEX로 B-트리에가서 해당 RID를 찾고
-- HEAP테이블에가서 실제 정보를 찾는다. (Look UP)

SELECT index_id, name
FROM sys.indexes
WHERE object_id = object_id('PlayerAccount')

-- 3번 인덱스 정보
DBCC IND('GameDB', 'PlayerAccount', 3);
--    608
-- 568   576
DBCC PAGE('GameDB',1/*파일번호*/, 624/*페이지번호*/, 3/*출력옵션*/);
DBCC PAGE('GameDB',1/*파일번호*/, 584/*페이지번호*/, 3/*출력옵션*/);
DBCC PAGE('GameDB',1/*파일번호*/, 520/*페이지번호*/, 3/*출력옵션*/);

SET STATISTICS TIME ON;
SET STATISTICS IO ON;
SET STATISTICS PROFILE ON;

SELECT *
FROM PlayerAccount
WHERE accountName = 'Hanil2500'

USE BaseballData

SELECT *
FROM dbo.players
WHERE playerID = 'neubaha01'