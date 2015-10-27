#include "curl_idris.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};

struct CurlResult {
  long status;
  const char *headers;
  const char *body;
};

struct CurlResult *Result_create(long status, const char* headers, const char* body) {
  struct CurlResult *res = malloc(sizeof(struct CurlResult));
  assert(res != NULL);

  res->status = status;
  res->headers = headers;
  res->body = body;

  return res;
}

void destroy_result(struct CurlResult *res) {
  /* free((void*)&res->headers); */
  /* free((void*)&res->body); */
  free(res);
}

long get_status(struct CurlResult *res) {
  return res->status;
}

const char* get_headers(struct CurlResult *res) {
  return res->headers;
}

const char* get_body(struct CurlResult *res) {
  return res->body;
}

static size_t WriteMemoryCallback(void*, size_t, size_t, void*);

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

struct CurlResult* foreign_get(char* url)
{
  CURL *curl_handle;
  CURLcode res;
  long httpcode = 0;

  struct MemoryStruct chunk;
  chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
  chunk.size = 0;    /* no data at this point */

  struct MemoryStruct headers;
  headers.memory = malloc(1);
  headers.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl_handle = curl_easy_init();

  /* specify URL to get */
  curl_easy_setopt(curl_handle, CURLOPT_URL, url);

  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  // HEADERS OMG!!!!!!!!
  /* curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, WriteMemoryCallback); */
  curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, (void *)&headers);



  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  /* get it! */
  res = curl_easy_perform(curl_handle);

  curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &httpcode);

  /* check for errors */
  if(res != CURLE_OK) {
    /* fprintf(stderr, "curl_easy_perform() failed: %s\n", */
    /*         curl_easy_strerror(res)); */
    return Result_create(httpcode, "", curl_easy_strerror(res));
  }

  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);

  /* free(chunk.memory); */

  /* we're done with libcurl, so clean it up */
  curl_global_cleanup();

  return Result_create(httpcode, headers.memory, chunk.memory);
}
 /***************************************************************************/
/* This file is derived from getinmemory.c, courtesy of Daniel Stenburg
 * <DESC>
 * Shows how the write callback function can be used to download data into a
 * chunk of memory instead of storing it in a file.
 * Copyright (C) 1998 - 2015, Daniel Stenberg, <daniel@haxx.se>, et al.
 * </DESC>
 */
