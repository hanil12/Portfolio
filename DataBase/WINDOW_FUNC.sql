USE BaseballData;

SELECT *
FROM salaries

-- Window 함수
--           서브셋        정렬     범위
-- ~ OVER([Partition]  [Order By] [Rows])

SELECT *,
	ROW_NUMBER() OVER(ORDER BY salary DESC) as 로우넘버,
	RANK() OVER(ORDER BY salary DESC) as 랭킹,
	DENSE_RANK() OVER(ORDER BY salary DESC) as 덴스랭크,
	NTILE(100) OVER(ORDER BY salary DESC) as 백분율
FROM salaries;

-- 같은 플레이어에서 순위를 매기기
SELECT *,
	RANK() OVER(PARTITION BY playerID ORDER BY salary DESC)
FROM salaries;

-- LAG(바로 이전), LEAD(바로 다음)
SELECT *,
	RANK() OVER(PARTITION BY playerID ORDER BY salary DESC) AS RANK,
	LAG(salary) OVER(PARTITION BY playerID ORDER BY salary DESC) AS PREV,
	LEAD(salary) OVER(PARTITION BY playerID ORDER BY salary DESC) AS NEXT
FROM salaries;

-- FIRTST_VALUE, LAST_VALUE(범위)
SELECT *,
	--RANK() OVER(PARTITION BY playerID ORDER BY salary DESC) AS RANK,
	FIRST_VALUE(salary) OVER(
	PARTITION BY playerID ORDER BY salary DESC
	ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW) AS MAX_SALARY, -- 범위 설정

	LAST_VALUE(salary) OVER(
	PARTITION BY playerID ORDER BY salary DESC
	ROWS BETWEEN CURRENT ROW AND UNBOUNDED FOLLOWING) AS MIN_SALARY -- 범위 설정
FROM salaries;
