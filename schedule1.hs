{-Question 1
In this programming problem and the next you'll code up the greedy algorithms from lecture for minimizing the weighted sum of completion times.. Download the text file here. This file describes a set of jobs with positive and integral weights and lengths. It has the format

[number_of_jobs]
[job_1_weight] [job_1_length]
[job_2_weight] [job_2_length]
...
For example, the third line of the file is "74 59", indicating that the second job has weight 74 and length 59. You should NOT assume that edge weights or lengths are distinct.

Your task in this problem is to run the greedy algorithm that schedules jobs in decreasing order of the difference (weight - length). Recall from lecture that this algorithm is not always optimal. IMPORTANT: if two jobs have equal difference (weight - length), you should schedule the job with higher weight first. Beware: if you break ties in a different way, you are likely to get the wrong answer. You should report the sum of weighted completion times of the resulting schedule --- a positive integer --- in the box below. -}


-- job scheduling
-- compute the weighted sum of completion times
-- use greedy algorithm design that schedules based on a score which is
-- weight - length is algorithm to try and minimize
-- the weighted sum of completion times
-- where Cj = Sum of C0 to Cj of weight_j * completion_j

-- zipWith (*) xs (scanl1 (+) xs)
-- (zipwith (*) <*> scanl1 (+)) xs

-- A = 69119377652

import System.IO
import Data.List
import Control.Monad
import Control.Applicative

toNums :: [String] -> [[Int]]
toNums [] = [] 
toNums (x:xs) = (map read . words $ x :: [Int]) : toNums xs

-- append score to [w,l] => [w-l,w,l] for ordering
appendScores :: [[Int]] -> [[Int]]
appendScores []     = []
appendScores (x:xs) = ((\[fst,scnd] -> fst - scnd) x : x) : appendScores xs

-- wSC = weighted sum of completion times
wSC :: [[Int]] -> Int
wSC []   = 0 
wSC xs   = sum $ zipWith (*) [w !! 1 | w <- xs] (scanl1 (+) [l !! 2 | l <- xs])

main = do
  handle <- openFile "./jobs.txt" ReadMode
  contents <- hGetContents handle
  let jobs = tail $ lines contents --ignoring first line
  
  print $ wSC . reverse . sort . appendScores . toNums $ jobs