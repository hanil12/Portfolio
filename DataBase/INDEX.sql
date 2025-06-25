USE BaseballData;

SELECT*
FROM players;

CREATE TABLE IndexTest
(
	lahmanID INT NOT NULL,
	fullName NVARCHAR(40) NOT NULL,
	weight INT,
	height INT
);

--DROP TABLE IndexTest;

INSERT INTO IndexTest
SELECT lahmanID, nameFirst + nameLast, weight, height
FROM players
WHERE nameFirst IS NOT NULL AND nameLast IS NOT NULL;

-- Table Scan - Very Bad
SELECT *
FROM IndexTest
WHERE lahmanID = 1500;

-- Clustered Index
-- Index Seek - LogN
-- 데이터가 많을 수록 효율적이다.
-- 정렬되어있다.
-- 테이블에서 단 하나만 존재할 수 있다.
-- 영한 사전
CREATE CLUSTERED INDEX Clustered_Test ON IndexTest(lahmanID);
DROP INDEX Clustered_Test ON IndexTest;

-- Non Clustered Index
-- Index Scan - N
-- 데이터가 많을 수록 비효율적이다.
-- 정렬 X
-- 테이블에서 여러개 존재할 수 있다.
-- 색인
-- Clustered Index의 존재여부에 영향을 받는다.
CREATE INDEX Non_FullName ON IndexTest(fullName);
DROP INDEX Non_FullName ON IndexTest;

SELECT *
FROM IndexTest
WHERE fullname = 'JimAbbott';

-- 복합 인덱스
-- 순서 차이에 의해서 찾는 시간의 차이가 발생한다.
CREATE INDEX Non_Composite ON IndexTest(lahmanID,fullName);

SELECT *
FROM IndexTest
WHERE lahmanID = 80 AND fullName = 'DeweyAdkins';

SELECT *
FROM IndexTest
WHERE lahmanID = 150;

SELECT *
FROM IndexTest
WHERE fullName = 'DeweyAdkins';