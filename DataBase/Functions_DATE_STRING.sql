-- MSSQL
-- DATETIME Type의 비교
-- VARCHAR(string) 비교
-- 집계함수
-- Sub Query

USE BaseballData;

SELECT *
FROM players;

-- Players 테이블에서 모든 사람의 이름과 태어난 날짜, 사망 날짜를 출력해주세요. 나이 출력
SELECT nameFirst, nameLast,
DATEFROMPARTS(birthYear, birthMonth, birthDay) as 생일, -- DATETIME
DATEFROMPARTS(deathYear, deathMonth, deathDay) as 돌아가신날,
(deathYear - birthYear) as 나이
FROM players
WHERE DATEFROMPARTS(deathYear, deathMonth, deathDay) IS NOT NULL AND DATEFROMPARTS(birthYear, birthMonth, birthDay) IS NOT NULL
ORDER BY 나이

-- VARCHAR
-- LIKE 'NEW%' => NEW로 시작하는 모든 단어
-- LIKE 'NEW___' => NEW로 시작하고 뒤에 세글자
-- players 테이블에서 birthCity가 New로 시작하는 사람들의 모든 정보를 출력해주세요
SELECT *
FROM players
WHERE birthCity LIKE 'NEW_____';

-- 집계함수
-- players에서 birthCity별로 평균 나이대
SELECT birthCity,AVG(2025 - birthYear)
FROM players
GROUP BY birthCity


-- SubQuery => Query in Query
SELECT TOP(1) playerID
FROM salaries
ORDER BY salary DESC;

-- rodrial01
SELECT *
FROM players
WHERE playerID =
(
	SELECT TOP(1) playerID
	FROM salaries
	ORDER BY salary DESC
);

-- 프로그래머스 문제 (SUM, MAX, MIN)
-- 가장 비싼 상품 구하기
-- 가격이 제일 비싼 식품의 정보 출력하기
-- 최대값 구하기
-- 동물 수 구하기
-- 중복 제거하기

-- GROUP BY
-- 고양이와 개는 몇 마리 있을까
-- 자동차 종류 별 특정 옵션이 포함된 자동차 수 구하기
-- 동명 동물 수 찾기