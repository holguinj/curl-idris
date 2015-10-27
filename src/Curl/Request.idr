module Curl.Request

%include C "curl_idris.h" -- the header containing declarations for the functions we need
%link C "curl_idris.o" -- an object file to link against
%lib C "curl" -- equivalent to passing '-lcurl' to gcc

||| a record corresponding to the CurlResult C struct
public
record CurlResult where
  constructor MkCurlResult
  status : Int
  headers : String
  body : String

||| This is a 'phantom type' wrapping a pointer to a CurlResult struct
data FFICurl a = MkFFICurl Ptr
%name MkFFICurl ptr

||| Perform a curl request and return a foreign pointer to the result.
foreignGet : String -> IO (FFICurl Ptr)
foreignGet url = do ptr <- get' url
                    return $ MkFFICurl ptr
                 where get' : String -> IO Ptr
                       get' = foreign FFI_C "foreign_get" (String -> IO Ptr)

||| Access the 'status' member of a CurlResult struct
getStatus : FFICurl Ptr -> IO Int
getStatus (MkFFICurl ptr) = foreign FFI_C "get_status" (Ptr -> IO Int) ptr

||| Access the 'headers' member of a CurlResult struct
getHeaders : FFICurl Ptr -> IO String
getHeaders (MkFFICurl ptr) = foreign FFI_C "get_headers" (Ptr -> IO String) ptr

||| Access the 'body' member of a CurlResult struct
getBody : FFICurl Ptr -> IO String
getBody (MkFFICurl ptr) = foreign FFI_C "get_body" (Ptr -> IO String) ptr

||| Free the malloc'd memory from a CurlResult struct. Safe to call once it has
||| been turned into an Idris record.
destroyResult : FFICurl Ptr -> IO ()
destroyResult (MkFFICurl ptr) = foreign FFI_C "destroy_result" (Ptr -> IO ()) ptr

||| Given a URL, perform a GET request using curl over the FFI. Note that the pointer
||| is _not_ freed manually because valgrind reports a much larger memory leak when it is!
public
httpGet : (url : String) -> IO CurlResult
httpGet url = do cResult <- foreignGet url
                 status  <- getStatus cResult
                 headers <- getHeaders cResult
                 body    <- getBody cResult
                 -- destroyResult cResult -- this line INTRODUCES a noticeable memory leak!
                 return $ MkCurlResult status headers body
