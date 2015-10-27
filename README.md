# curl-idris

A (very much in-progress) wrapper around [libcurl](http://curl.haxx.se/libcurl/)
for Idris.

## how to install
Clone this project and run `make` to build and install the package. Please note
that [libcurl](http://curl.haxx.se/libcurl/) is required.

## how to use
Right now the only somewhat-useful function is `Curl.Request.httpGet`, which has
the type `String -> IO CurlResponse`. A `CurlResponse` is a record containing
the following fields:

* status : Int     -- the HTTP status code of the response, or `0` if there was a curl error
* headers : String -- a single string representing the entire headers payload
* body : String    -- the body of the response, or the stringified curl error

There is an example in the `tests` directory.

## warning
This barely works for basic HTTP GET requests. It's probably pretty likely to
have a buffer overrun or something and may cause a segfault in your code.

Pull requests are welcome.
