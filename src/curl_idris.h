#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

extern struct CurlResult* Result_create(long, const char*, const char*);
extern void destroy_result(struct CurlResult*);
extern struct CurlResult* foreign_get(char*);
extern long get_status(struct CurlResult*);
extern const char* get_headers(struct CurlResult*);
extern const char* get_body(struct CurlResult*);
