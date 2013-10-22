--dagan

import System.IO
import Control.Monad

main = do  
  --contents <- readFile "100nums.txt"
  --print . map readInt . lines $ contents
  nums <- fmap lines $ readFile "100nums.txt"
  print $ sum (map read nums)

readInt :: String -> Int
readInt = read