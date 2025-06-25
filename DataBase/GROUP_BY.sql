USE BaseBallData;

-- GROUP BY 절
-- 집계함수 : COUNT, AVG, SUM, MIN, MAX
-- HAVING 절

-- FROM
-- WHERE
-- GROUP BY
-- HAVING
-- SELECT
-- ORDER BY

SELECT *
FROM dbo.players;

-- players 테이블에서 '국가별로' 몇명의 선수들이 있었는지
SELECT COUNT(lahmanID) as COUNT, birthCountry as Country
FROM dbo.players
GROUP BY birthCountry;

-- players 테이블에서 '국가별로' 몇명의 '현존하는' 선수들이 있었는지
SELECT COUNT(lahmanID) as COUNT, birthCountry as Country
FROM dbo.players
WHERE deathYear IS NULL
GROUP BY birthCountry

-- players 테이블에서 '국가별로' 몇명의 '현존하는' 선수들이 있었는지 출력할 때, COUNT가 10명 이상인 것들만 출력해주세요.
SELECT COUNT(lahmanID) as Number, birthCountry as Country
FROM dbo.players
WHERE deathYear IS NULL
GROUP BY birthCountry
HAVING COUNT(lahmanID) > 10;

SELECT *
FROM dbo.salaries

-- salaries 테이블에서 팀별로 salary의 총합을 출력해주세요.

-- salaries 테이블에서 팀별로 salary의 평균을 출력해주세요.

-- salaries 테이블에서 연도별로 최고의 salary를 출력해주세요.

-- salaries 테이블에서 연도별로 최저의 salary를 출력해주세요.