--dgn
--credit card validation algorithm
--enter credit card number

import System.IO
import Data.List as L

str_to_int :: String -> Int
str_to_int = (\x -> read x :: Int)

digits :: Int -> [Int]
digits = map (\x -> (fromEnum x) - (fromEnum '0')) . show

doubleEveryOther :: [Int] -> [Int]
doubleEveryOther = zipWith ($) (cycle [id, (*2)])

sumDigits :: [Int] -> Int
sumDigits xs = sum . L.concat . (map digits) $ xs


validate :: String -> Bool
validate = (\x -> mod x 10 == 0) . sumDigits . doubleEveryOther . reverse . digits . str_to_int

niceResponse :: Bool -> IO ()
niceResponse = undefined


main = getLine >>= (\cc_num -> return $ (validate cc_num))
