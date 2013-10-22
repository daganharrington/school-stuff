import Control.Monad.State
import qualified Data.IntMap as M


digits :: Int -> [Int]
digits = map (\x -> (fromEnum x) - (fromEnum '0')) . show

next :: Int -> Int
next = sum . map (\x -> x*x) . digits

{-
end :: Int -> Int
end n
  | n' == 1   = 1
  | n' == 89  = 89
  | otherwise = end n'
  where n' = next n
-}

terminator' :: Int -> State (M.IntMap Int) Int
terminator' n = do
  m <- get
  if M.member n m
     then return (m M.! n)
     else do t <- terminator (next n)
             modify (M.insert n t)
             return t
             
terminator :: Int -> State (M.IntMap Int) Int
terminator n = if (n == 1) || (n == 89) then return n else terminator' n

terminators :: [Int] -> State (M.IntMap Int) [Int]
terminators = mapM terminator

main :: IO ()
main = print $ length $ filter (==89) $ evalState (terminators [1..10000000]) M.empty