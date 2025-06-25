USE BaseballData;

-- DATABASE가 무엇인지
-- DATABASE를 왜 쓰는지
-- DATABASE 종류 2가지 이상 쓰고, 관계형 DB에 대해서 조사하기

-- 기본키 무엇인지
-- 클러스터드형 인덱스 / 넌 클러스터드형 인덱스가 무엇인지
-- Clustered Index 자료구조, Non Clustered Index 자료구조 

-- 문법
-- SELECT ~ : ~를 추출해주세요.
-- FROM : (~ 어떤 테이블)에서
-- WHERE
-- ORDER BY

-- * : ALL

-- 해석
-- FROM
-- WHERE
-- SELECT
-- ORDER BY

-- ORDER BY
-- ASC : ASCEDING 약자
-- DESC : DESCENDING 약자

SELECT TOP(100) playerID as ID, birthYear
FROM dbo.players
WHERE birthYear > 1950 AND birthMonth = 7 AND birthCountry = 'USA'
ORDER BY playerID ASC;

SELECT *
FROM dbo.players
WHERE birthYear > 1950 AND birthMonth = 7 AND birthCountry = 'USA' AND (deathYear IS NOT NULL)
ORDER BY playerID ASC;

-- SELECT
-- 모든 레코드 조회하기
-- 동물의 아이디와 이름
-- 조건에 맞는 회원수 구하기
-- 어린동물찾기
-- 아픈동물찾기
-- 역순 정렬하기
-- 상위 n개 레코드(TOP(100) => LIMIT)