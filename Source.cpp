#include<iostream>
#include<string>
#include <stdio.h>
#define CURL_STATICLIB
#include"curl/curl.h"

#ifdef _DEBUG
#	pragma comment(lib, "curl/libcurl_a.lib") 
#else
#	   pragma comment(lib, "curl/libcurl_a_debug.lib")
#endif
 

int main(void) 
{
	CURL *curl;			// CURL = void 
	CURLcode res;		// CURLcode je enum

	curl = curl_easy_init();  // create an easy handle (Start a libcurl easy session)  UVEK SE PRVA POZIVA
	if (curl) 
	{
		curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/todos/1"); // set options for a curl easy handle --> tell libcurl how to behave
																							 // (HANDLE ON CURL, OPTION, PARAMETAR)
		
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);         // example.com is redirected, so we tell libcurl to follow redirection

		
		res = curl_easy_perform(curl);			//  perform a file transfer synchronously, Perform the request, res will get the return code 
												//  perform a blocking file transfer
												//  performs the entire request in a blocking manner and returns when done  --> VRSI ISPIS NA KONZOLU

		if (res != CURLE_OK)					// AKO JE CURLE_OK = 0 --> All fine. Proceed as usual
		{ 
			fprintf(stderr, "curl_easy_perform() failed: %s\n",			// (STREAM,FORMAT,SOURCE)
				curl_easy_strerror(res));								// return string describing error code
		}

		curl_easy_cleanup(curl);  // ODGOVARA NA FUNKCIJU curl_easy_init();  --> end a libcurl easy handle
								  // This function must be the last function to call for an easy session
								  // It is the opposite of the curl_easy_init function 
								  // must be called with the same handle as input that a curl_easy_init call returned.
	}
	return 0;
}

//
//int main(void)
//{
//	CURL *curl;
//	CURLcode res;
//
//	static const char *postthis = "moo mooo moo moo";
//
//	curl = curl_easy_init();
//	if (curl) {
//		curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/posts/1");
//		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);                                 // specify data to POST to serve
//
//		/* if we don't provide POSTFIELDSIZE, libcurl will strlen() by
//		   itself */
//		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));              // size of POST data pointed to
//		       
//		/* Perform the request, res will get the return code */
//		res = curl_easy_perform(curl);
//		/* Check for errors */
//		if (res != CURLE_OK)
//			fprintf(stderr, "curl_easy_perform() failed: %s\n",
//				curl_easy_strerror(res));
//
//		/* always cleanup */
//		curl_easy_cleanup(curl);
//	}
//	return 0;
//}