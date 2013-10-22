import System.IO

main = do
  lss <- fmap words $ fmap lines $ readFile "grid.txt"
  print $ lss