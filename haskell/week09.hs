containsNumber :: Int -> [Int] -> Bool
containsNumber _ [] = False
containsNumber n (x : xs)
  | x == n = True
  | otherwise = containsNumber n xs

containsTupel :: (Int, Int) -> [(Int, Int)] -> Bool 
containsTupel _ [] = False 
containsTupel tupel (t: tupels) | areTupelEqual tupel t = True 
                                | otherwise = containsTupel tupel tupels

areTupelEqual :: (Int, Int) -> (Int, Int) -> Bool 
areTupelEqual one two = (tupelFirst one == tupelFirst two) && (tupelSecond one == tupelSecond two)

areTupelAndElementsEqual :: (Int, Int) -> Int -> Int -> Bool 
areTupelAndElementsEqual tupel a b = (tupelFirst tupel == a) && (tupelSecond tupel == b)

indexOfNormalized :: Int -> [Int] -> Int
indexOfNormalized n xs = indexOf n xs + 1

indexOf :: Int -> [Int] -> Int
indexOf _ [] = 0
indexOf n xs = indexOfInternal 0 n xs

tupelFirst :: (a, b) -> a
tupelFirst (a,b) = a

tupelSecond :: (a, b) -> b
tupelSecond (a,b) = b

allOf :: [(Int, Int)] -> [Int]
allOf tpls = [tupelFirst tpl | tpl <- tpls] ++ [tupelSecond tpl | tpl <- tpls]

allOfUnique :: [Int] -> [Int]
allOfUnique vals = allOfUniqueHelper vals []

allOfUniqueHelper :: [Int] -> [Int] -> [Int]
allOfUniqueHelper [] vals = vals
allOfUniqueHelper (x: vals) store | containsNumber x store = allOfUniqueHelper vals store -- If it already has the number, skip
                                  | otherwise = allOfUniqueHelper vals (store ++ [x]) -- else add to store and go to next

indexOfInternal :: Int -> Int -> [Int] -> Int
indexOfInternal _ _ [] = 0
indexOfInternal i n (x : xs)
  | n == x = i
  | otherwise = indexOfInternal (i + 1) n xs

listSwap :: Int -> Int -> [Int] -> [Int]
listSwap _ _ [] = []
listSwap i1 i2 xs = [if x == i1 then i2 else if x == i2 then i1 else x | x <- xs]

tupelToList :: (Int, Int) -> [Int]
tupelToList tpl = [tupelFirst tpl, tupelSecond tpl]

symmetricDifference :: [Int] -> [Int] -> [Int]
symmetricDifference [] [] = []
symmetricDifference xs ys = [x | x <- xs ++ ys, (containsNumber x xs && not (containsNumber x ys)) || (containsNumber x ys && not (containsNumber x xs))]

powerlist :: [Int] -> [[Int]]
powerlist [] = [[]]
powerlist (x : xs) = [x : ps | ps <- powerlist xs] ++ powerlist xs

nodes :: [(Int, Int)] -> [Int]
nodes edges = allOfUnique( allOf edges )

permutations :: [Int] -> [[Int]]
permutations [] = [[]]
permutations xs = [y : zs | (y, ys) <- select xs, zs <- permutations ys]
  where
    select [] = []
    select (x : xs) = (x, xs) : [(y, x : ys) | (y, ys) <- select xs]

allConnectionsToNode :: [(Int, Int)] -> Int -> [(Int, Int)]
allConnectionsToNode xs node = [x | x <- xs, tupelSecond x == node]

allConnectionsFromNode :: [(Int, Int)] -> Int -> [(Int, Int)]
allConnectionsFromNode xs node = [x | x <- xs, tupelSecond x == node]

allConnectedNodes :: [(Int, Int)] -> Int -> [Int]
allConnectedNodes edges node = [tupelFirst x | x <- allConnectionsToNode edges node]



-- existsPath :: [(Int, Int)] -> Int -> Int -> Bool 
-- existsPath [] a b = False 
-- existsPath (edge: edges) a b | a == b = True 
--                             | areTupelAndElementsEqual edge a b = True
--                             | containsTupel edge