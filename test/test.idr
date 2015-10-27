-- Compile this file with `idris -p curl -o curl-test test.idr` and execute it
-- to show the body of the response

module Main

import Curl.Request

main : IO ()
main = do res <- httpGet "http://example.com"
          putStrLn $ body res
