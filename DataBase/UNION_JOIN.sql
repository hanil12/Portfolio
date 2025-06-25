
-- 기본키와 외래키에 대해서 조사
-- 기본키 
-- 1. NULL값을 가질 수 없다.
-- 2. 테이블에서 단 하나의 기본키만 존재한다.
-- 3. 중복허용X
-- => Clusted Index

-- 외래키
-- 1. 하나의 테이블에서 다른 테이블의 키를 참조하는 키
-- 2. 데이터 무결성을 지키는 중요한 역할
-- 3. NULL값 허용
-- 4. 테이블에서 여러개의 외래키가 등장할 수 있다.
-- 5. 중복된 키도 외래키로 만들 수 있다.

USE BaseballData;
-- 테이블을 합친다.
-- UNION
-- players 테이블에서 생일이 1950년도 이상, 1900년도이하의 player들의 모든 정보
(SELECT *
FROM players
WHERE birthYear >= 1950)
UNION
(SELECT *
FROM players
WHERE birthYear <= 1900)

-- players와 salaries 테이블을 보면서 평균 연봉이 300만 달러 이상이거나 12월에 태어난 선수들의 playerID를 추출해주세요.
-- 이 때 중복된다면 중복을 허용해주세요.(UNION ALL)
(SELECT playerID
FROM salaries
GROUP BY playerID
HAVING AVG(salary) >= 3000000)
UNION ALL
(SELECT playerID
FROM players
WHERE birthMonth = 12 AND playerID IS NOT NULL)

-- JOIN
-- CROSS JOIN

-- INNER JOIN
-- OUTER JOIN

USE BaseballData;

SELECT *
FROM
((SELECT *
FROM players
WHERE lahmanID <= 10) T0
CROSS JOIN
(SELECT *
FROM batting
WHERE HR > 55) T1);

-- INNER JOIN : 가로로 합친다.
(SELECT *
FROM players) 
(SELECT *
FROM batting
WHERE HR > 20)

SELECT *
FROM players AS pTable
INNER JOIN 
batting AS bTable
ON pTable.playerID = bTable.playerID AND bTable.HR > 20;

-- OUTER JOIN
-- LEFT JOIN, RIGHT JOIN
(SELECT playerID, AVG(HR) AS HOMERUN
FROM batting
GROUP BY playerID)
LEFT JOIN 
(SELECT lahmanID,playerID, nameFirst, nameLast
FROM players) AS pTable