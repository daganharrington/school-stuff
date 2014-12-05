-- compute the max-spacing k-clusters of a graph.
-- a greedy clustering algorithm equivalent to kruskal (MST) terminated early (@ k disjoint clusters)
-- dagan '14
import System.IO
import Data.List
import Data.Ord --for comparing
import qualified Data.Map as M
--import Data.Sequence --for sortBy

type ConMap = [(Int,[Int])] --lead node,[nodes in cluster] --use UnionFind here! Data.UnionFind.IntMap???
type Edge = (Int,Int,Int) --(u,v,weight)
type Graph = [Edge]


toNums = map (\x -> map read (words x) :: [Int])

toTuple = map (\[x,y,z] -> (x,y,z))

sort' tups = sortBy (comparing (\(x,y,z) -> z)) $ tups --sort by z (weight)


f :: [(Int,[Int])] -> [Int]--make more general .. this expects a singleton list.
f [] = []
f [(_,rs)] = rs  -- f (x:xs) = snd x ?

--below is testament to the usefulness of Data.IntMap
updateCList :: Int -> ConMap -> Edge -> ConMap
updateCList k cmap (x,y,z) 
  | cmap == [] = []
  | k == (length cmap) = cmap --stop once you have k-clusters (disjoint k-partition of the nodes)
  | otherwise          = ( (cleanls cmap) ++ [(x, newls cmap)]) where  
  cleanls cs = (filter (\(n,ns) -> notElem y ns && notElem x ns) cs)
  newls cs = union (f . (filter (\(_,ls) -> elem x ls)) $ cs) (f . filter (\(_,rs) -> elem y rs) $ cs)


kClusters :: Graph -> Int -> ConMap -> ConMap
kClusters grph k strt = foldl (updateCList k ) strt grph 
         
-- some debuging and illustrative prints for testing in the IO monad
main :: IO ()               
main = do
  handle <- openFile "./clustertest2.txt" ReadMode
  contents <- hGetContents handle
  --initialize each node in its own cluster
  let initClusters = [(x,[x]) | x <- 
                         [1..(read . head . lines $ contents)]]
  let points = tail $ lines contents --ignoring first line
  let sorted = sort' . toTuple . toNums $ points
  --print $ sorted
  print $ length initClusters
  print $ initClusters
  print $ sorted
  let result = (kClusters sorted 4 initClusters) --compute a 4-cluster
  print $ result
  print $ length result
  print $ (map (length . (\(_,rs) -> rs))) $ result
  print $ sum . (map (length . (\(_,rs) -> rs))) $ result
