#ifndef HTTPDOWNLOADER_HPP
#define HTTPDOWNLOADER_HPP
#include <string>
using namespace std;

// static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

class HTTPDownloader {
public:
    HTTPDownloader();
    ~HTTPDownloader();
    string download(const std::string& url);
private:
    void* curl;
};
#endif  /* HTTPDOWNLOADER_HPP */