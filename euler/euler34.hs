sumfact :: Int -> Int
sumfact n = sum . map factorial . digits $ n where
            digits = map (`mod` 10) . reverse . takeWhile (> 0) . iterate (`div` 10)


factorial :: Int -> Int
factorial 1 = 1
factorial n = n * factorial (n-1)
