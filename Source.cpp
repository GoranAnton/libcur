#define CURL_STATICLIB
#include<iostream>
#include<string>
#include"curl/curl.h"

#ifdef _DEBUG
#pragma comment (lib,"curl/libcurl_a_debug.lib")
#else
#pragma comment (lib,"curl/libcurl_a.lib")
#endif

#pragma comment(lib,"Normaliz.lib")
#pragma comment(lib,"Ws2_32.lib") 
#pragma comment(lib,"Wldap32.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"advapi32.lib")

static size_t my_write(void* buffer, size_t size, size_t nmemb, std::string* param)
{
	size_t totalsize = size * nmemb;
	param->append(static_cast<char*>(buffer), totalsize);    // ova funkcija dodaje prvih totalsize karaktera iz buffer-a u string param
	return totalsize;
}

int main()
{
	std::string result;
	CURL* curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "https://reqres.in/api/users");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=morpheus&job=leader");		// sve podatke koji stignu sa servera salje u my_write funkciju
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write);		// sve podatke koji stignu sa servera salje u my_write funkciju
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);				// setovanje param argumenta iz my_write funkcije (param=&result)
		res = curl_easy_perform(curl);
		if (CURLE_OK != res)
		{
			std::cerr << "CURL error" << res << '\n';
		}
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	std::cout << result << std::endl;
}