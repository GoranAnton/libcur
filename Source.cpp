#define CURL_STATICLIB
#include<iostream>
#include<string>
#include<map>
#include"curl/curl.h"
#include"json.hpp"
using json = nlohmann::json;

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

static size_t write_body(void* buffer, size_t size, size_t nmemb, std::string* param)   // funkcija za citanje body-ja
{
	size_t totalsize = size * nmemb;
	param->append(static_cast<char*>(buffer), totalsize);
	return totalsize;
}

static size_t write_header(void* buffer, size_t size, size_t nmemb, std::map<std::string, std::string>* param)  // funkcija za citanje header-a
{
	size_t totalsize = size * nmemb;
	std::string a, b,s;
	s.append(static_cast<char*>(buffer), totalsize);
	int n = s.find(':');
	if (n != -1)  // ovako eliminisem prvu liniju gde se nalazi status
	{
		a = s.substr(0, n);
		b = s.substr(n + 1);
		param->insert(std::pair<std::string, std::string>(a, b));
	}
	return totalsize;
}

struct Response   // struktura u koju smestam status, header-e i body
{
	long status;
	std::map<std::string,std::string> header;
	std::string body;
	friend std::ostream& operator<<(std::ostream& out, const Response& r);
};

std::ostream& operator<<(std::ostream& out, const Response& r) // definicija friend metode za ispis strukture
{
	out << "Status: "<< r.status << std::endl;
	out << "--------------------------------------------------\n";
	for (auto it=r.header.begin(); it!=r.header.end();++it)
	{
		out << it->first << " : " << it->second;
	}
	out << "--------------------------------------------------\n";
	out << std::endl;
	out << "--------------------------------------------------\n";
	out << r.body << std::endl;
	return out;
}

int main()
{
	json j{ {"name","morpheus"},{"job","leader"} };  // json koji cu da posaljem uz metodu POST
	Response response;
	CURL* curl;											// POST https://reqres.in/api/users
	CURLcode res;										// GET  https://jsonplaceholder.typicode.com/todos/1
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl)
	{			
		struct curl_slist *chunk = NULL;  // zadavanje hedera
		chunk = curl_slist_append(chunk, "application:json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

		curl_easy_setopt(curl, CURLOPT_URL, "https://reqres.in/api/users");    // zadavanje URL
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, j.dump());		// dump-ovanje json-a kada radimo POST / kada radimo GET --> iskomentarisati

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body);		// zadavanje funkcije za citanje body-ja
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.body);		// zadavanje string-a u koji se smesta body

		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header);	// zadavanje funkcije za citanje header-a
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response.header);	// zadavanje map-e u koju se smestaju header-i

		res = curl_easy_perform(curl);
		if (CURLE_OK != res)
		{
			std::cerr << "CURL error" << res << '\n';
		}
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.status); // iscitavanje statusa i smestanje u int varijablu u strukturi response
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	std::cout << response;  // ispis strukutre
}