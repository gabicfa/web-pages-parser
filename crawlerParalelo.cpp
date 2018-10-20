#include <iostream>
#include <string>
#include <stdio.h>
#include <boost/regex.hpp>
#include <chrono>
#include <chrono>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>
#include <sstream>
#include <list>
#include <thread>
#include <fstream>
#include "semaphore.cpp"

using namespace std;
using namespace boost;
using namespace std::chrono;

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string download(string url) {
    CURL *curl;
    string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_TRANSFERTEXT, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        // ofstream myfile;
        // myfile.open ("../page.txt");
        // myfile << readBuffer;
        // myfile.close();
    }
    return readBuffer;
}

void findMatches(string str, regex reg, list<string>& result, int index){
    smatch matches;
    while(regex_search(str, matches, reg)){
        if(matches.size()> 0){
            result.push_back(matches[index]);
            str = matches.suffix().str(); 
        }
    }
}

int totalPages(string page){
    list<string> lastPage;
    regex numPages("\"lastPage\":([^,]+)");
    findMatches(page, numPages, lastPage, 1);
    int totalPages = stoi(lastPage.back());
    return totalPages;
}

list<string> findMatchesPages(string page, int totalPages, int numPag){
    list<string> urlsProducts;
    list<string> lastPage;

    regex href("name=\"linkToProduct\" href=\"([^\"]+)");
    findMatches(page, href, urlsProducts, 1);
    
    if(numPag != totalPages){
        regex nextPage("<link rel=\"next\" href=\"([^\"]+)");
        findMatches(page, nextPage, lastPage, 1);
        string next = lastPage.front();
        next = "https://www.magazineluiza.com.br"+next;
        urlsProducts.push_back(next);
        return urlsProducts;
    }
    else{
        urlsProducts.push_back("none");
        return urlsProducts;
    }
}

string collectProduct(string page){
    list<string> buffer;

    regex name ("<h1 class=\"header-product__title\" itemprop=\"name\">([^<]+)");
    findMatches(page, name, buffer, 1);
    string productName = buffer.front();

    buffer.clear();
    regex desc("<h2 class=\"description__product-title\">([^<]+)</h2>    <p class=\"description__text\"></p>([^<]+)");
    findMatches(page, desc, buffer, 2);
    string productDescription = buffer.front();

    buffer.clear();
    regex image("showcase-product__big-img js-showcase-big-img\" src=\"(https[^\"]+)");
    findMatches(page, image, buffer, 1);
    string productImage = buffer.front();

    buffer.clear();
    regex price("price-template__text[^>]+>([^<]+)</span>");
    findMatches(page, price, buffer, 1);
    string productPrice = buffer.front();

    buffer.clear();
    regex parcelado("installmentAmount\": \" ([^\"]+)");
    findMatches(page, parcelado, buffer, 1);
    string precoParcelado = buffer.front();

    buffer.clear();
    regex numparcelas("installmentQuantity\": \"([^\"]+)");
    findMatches(page, numparcelas, buffer, 1);
    string numeroParcelas = buffer.front();

    buffer.clear();
    regex category("itemprop=\"item\"> ([^>]+)</a>  </li>  </ul>");
    findMatches(page, category, buffer, 1);
    string productCategory = buffer.front();

    buffer.clear();
    regex url("socialShare[^u]+url: \'([^\']+)");
    findMatches(page, url, buffer, 1);
    string productUrl = buffer.front();

    string out = 
    "  {\n"
    "    \"nome\" : \"" + productName +"\",\n"
    "    \"descricao\" : \"" + productDescription +"\",\n"
    "    \"foto\" : \"" + productImage +"\",\n"
    "    \"preco\" : \"" + productPrice +"\",\n"
    "    \"preco_parcelado\" : \"" + precoParcelado +"\",\n"
    "    \"preco_num_parcelas\" : \"" + numeroParcelas +"\",\n"
    "    \"categoria\" : \"" + productCategory +"\",\n"
    "    \"url\" : \"" + productUrl +"\",\n"
    "  },\n";
    return out;
}

void produceJSONs(list<string>& listofPages, Semaphore& accessListofPages, Semaphore& listofPagesCount, Semaphore& accessJSON, string finalJSON){
    string currentPage;
    string jsonProduct;
    while(true){
        listofPagesCount.acquire();
        
        accessListofPages.acquire();
            currentPage = listofPages.front();
            listofPages.pop_front();
            cout << "Pagina retirada de listofPages" << '\n';
        accessListofPages.release();

        jsonProduct = collectProduct(currentPage);
        cout << "Analise do produto vindo de listofPages feita" << '\n';
        
        accessJSON.acquire();
            finalJSON+=jsonProduct;
            cout << "JSON do produto adicionado ao json final" << '\n';
            cout << '\n';
        accessJSON.release();
    }
}

void downloadPages(list<string>&listofUrls, Semaphore& accessListofUrls, Semaphore& listofUrlCount, list<string>&listofPages, Semaphore& accessListofPages, Semaphore&listofPagesCount){
    string currentProductUrl;
    string currentProductPage;
    while(true){
        listofUrlCount.acquire();

        accessListofUrls.acquire();
            currentProductUrl = listofUrls.front();
            listofUrls.pop_front();
        accessListofUrls.release();
        
        currentProductPage = download(currentProductUrl);
        cout << "Download da url pega de listofUrls" << '\n';

        accessListofPages.acquire();
            listofPages.push_back(currentProductPage);
            cout << "Pagina adicionada `a listofPages" << '\n';
            listofPagesCount.release();
        accessListofPages.release();
    }
}

void produceUrls(list<string>& listofUrls, Semaphore& accessListofUrls, Semaphore& listofUrlCount, string url){
    string page = download(url);
    string nextPageUrl;
    int total = totalPages(page);
    list<string> urlsPage;

    for(int p=1; p<=total; p++){
        urlsPage = findMatchesPages(page, total, p);
        nextPageUrl = urlsPage.back();
        urlsPage.pop_back();

        accessListofUrls.acquire();
            for(auto u = urlsPage.begin(); u != urlsPage.end(); ++u){
                listofUrls.push_back(*u);
                cout << "Adicionada url `a listofUrls" << '\n';
                listofUrlCount.release();
            }
        accessListofUrls.release();
        page = download(nextPageUrl);
    }
}

int main(int argc, char *argv[]) {
    
    Semaphore accessListofUrls(1);
    Semaphore accessListofPages(1);
    Semaphore accessJSON(1);
    Semaphore listofUrlCount(0);
    Semaphore listofPagesCount(0);

    list<string> listofUrls;
    list<string> listofPages;

    string finalJSON = "[\n";

    string url = "https://www.magazineluiza.com.br/dvd-player/tv-e-video/s/et/tvdb/";

    std::thread prodLinkCollectorThread(produceUrls, std::ref(listofUrls), std::ref(accessListofUrls), std::ref(listofUrlCount), url);
    std::thread prodPageCollectorThread(downloadPages, std::ref(listofUrls), std::ref(accessListofUrls), std::ref(listofUrlCount), std::ref(listofPages), std::ref(accessListofPages), std::ref(listofPagesCount));
    std::thread prodCollectorThread(produceJSONs,  std::ref(listofPages), std::ref(accessListofPages), std::ref(listofPagesCount), std::ref(accessJSON), finalJSON);

    prodLinkCollectorThread.join();
    prodPageCollectorThread.join();
    prodCollectorThread.join();

    return 0;
}