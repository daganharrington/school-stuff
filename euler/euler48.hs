selfexp :: Integer -> Integer
selfexp n = (^) n n

sums :: [Integer] -> Integer
sums []     = 0
sums (n:ns) = (selfexp n) + (sums ns)