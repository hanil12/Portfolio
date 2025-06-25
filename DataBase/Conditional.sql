USE BaseballData;

SELECT *
FROM salaries;

-- 변수 선언
DECLARE @i AS INT = 1000000;
DECLARE @s AS NVARCHAR(15) = 'hanil';

SET @s = 'sotoma01';

SELECT *
FROM salaries
WHERE salary > @i AND playerID = @s;

-- s에 players에서 몸무게가 가장 많이 나가는 선수의 이름으로 세팅해주세요.

SET @s = (
SELECT TOP(1) (nameFirst + nameLast) as FullName
FROM players
ORDER BY weight DESC
)

SELECT @s;

-- 분기문

DECLARE @i2 AS INT = 5;

-- if(i=5)..~~
IF @i2 = 5
BEGIN
PRINT('TRUE')
END

ELSE
BEGIN
PRINT('FALSE')
END

SET @i2 = 0;
WHILE @i2 < 5
BEGIN
	SET @i2 = @i2 + 1;

	PRINT @i2;
END

-- 임시 테이블
DECLARE @testTable TABLE
(
	fullName VARCHAR(50) NOT NULL,
	playerWeight INT NOT NULL
);


-- testTable에 이름과 몸무게를 넣어주세요.
INSERT INTO @testTable
SELECT (nameFirst + nameLast) as FullName, weight
FROM players
WHERE weight IS NOT NULL AND nameFirst IS NOT NULL AND nameLast IS NOT NULL
ORDER BY weight DESC

SELECT *
FROM @testTable;
