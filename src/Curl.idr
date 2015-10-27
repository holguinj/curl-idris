module Main

import Curl.Request

||| Useful for debugging requests. Takes a URL and prints out the status, headers, and body.
verboseGet : (url : String) -> IO ()
verboseGet url = do response <- httpGet url
                    case status response of
                      0 => putStrLn $ "There was a horrible error: " ++ (body response)
                      n => do putStrLn $ "Status: " ++ (show $ status response) ++ "\n"
                              putStrLn $ "Headers:\n" ++ (headers response)
                              putStrLn $ "Body:\n" ++ (body response)

handleMultiple : List String -> IO ()
handleMultiple [] = return ()
handleMultiple (url :: urls) = do verboseGet url
                                  handleMultiple urls

main : IO ()
main = case tail' !getArgs of
         Nothing   => return () -- unreachable at CLI
         Just []   => putStrLn "Please supply one or more URLs."
         Just urls => handleMultiple urls
