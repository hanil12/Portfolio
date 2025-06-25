-- INSERT (Row 삽입)
-- Delete (Row 삭제)
-- Update (Row에 있는 데이터 변경)

USE BaseballData;

SELECT *
FROM salaries
ORDER BY salary DESC

INSERT INTO salaries VALUES (2025, 'HAN', 'NL', 'hanil2', 50000000)

INSERT INTO salaries(yearID,teamID,lgID, playerID, salary)
VALUES (2025,'HAN','NL','hanil3', 60000000)

INSERT INTO salaries(yearID,teamID,lgID, playerID, salary)
VALUES (2025,'HAN','NL','hanil4', 100000000)

-- DELETE FROM [테이블] WHERE [조건]
DELETE FROM salaries
WHERE playerID = 'hanil4';

-- UPDATE [테이블명] SET[열 = 값], WHERE [조건]
UPDATE salaries
SET salary = salary * 1.5
WHERE yearID = 2025 AND teamID = 'HAN' AND salaries.lgID = 'NL' AND playerID = 'hanil2'