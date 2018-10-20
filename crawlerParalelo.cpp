#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <thread>
#include <chrono>
#include <boost/regex.hpp>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>
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

void consumePages(list<string>& listofPages, Semaphore& accessListofPages, Semaphore& listofPagesCount, Semaphore& accessJSON, const string &finalJSON, bool& noMorePages, int numConsumers, Semaphore& accesstempoMedioPorProduto, double& tempoMedioPorProduto){
    duration<double> tempoProd;
    high_resolution_clock::time_point t1, t2;

    string currentPage;
    string jsonProduct;
    double ociosoProd;
    string* tempJson = const_cast<string*>(&finalJSON);
    bool end = false;
    list<string> buffer;

    while(!end){
        listofPagesCount.acquire();
        accessListofPages.acquire();
            ociosoProd=0;
            if(noMorePages && listofPages.empty()){
                end = true;
            }
            else{
                
                currentPage = listofPages.front();
                listofPages.pop_front();
                if(noMorePages && listofPages.empty()){
                    for(int ct=0; ct< numConsumers; ct++){
                        listofPagesCount.release();
                    }
                }
            };
        accessListofPages.release();

        regex tempoociosoprod ("DownloadTime=([^>]+)>");
        findMatches(currentPage, tempoociosoprod, buffer, 1);
        if(buffer.size()> 0){
            ociosoProd= stod(buffer.front());
        }

        t1 = high_resolution_clock::now();
            jsonProduct = collectProduct(currentPage);
        t2 = high_resolution_clock::now();
        tempoProd = duration_cast<duration<double> >(t2 - t1);

        accesstempoMedioPorProduto.acquire();
            tempoMedioPorProduto = tempoMedioPorProduto + ociosoProd + tempoProd.count();
        accesstempoMedioPorProduto.release();
        
        accessJSON.acquire();
            *tempJson = finalJSON + jsonProduct;
        accessJSON.release();
    }
}

void producePages(list<string>&listofUrls, Semaphore& accessListofUrls, Semaphore& listofUrlCount, list<string>&listofPages, Semaphore& accessListofPages, Semaphore& listofPagesCount, bool& noMoreUrls, bool& noMorePages, int numProducers, Semaphore& accesstempoOcioso, double& tempoOcioso){
    string currentProductUrl;
    string currentProductPage;
    high_resolution_clock::time_point t1, t2;
    duration<double> ocioso;
    bool end = false;
    while(!end){
        listofUrlCount.acquire();
        accessListofUrls.acquire();
            if(noMoreUrls && listofUrls.empty()){
                end = true;
            }
            else{
                currentProductUrl = listofUrls.front();
                listofUrls.pop_front();
                if(noMoreUrls && listofUrls.empty()){
                    for(int pt=0; pt< numProducers; pt++){
                        listofUrlCount.release();
                    }
                }
            }
        accessListofUrls.release();

        t1 = high_resolution_clock::now();
            currentProductPage = download(currentProductUrl);
        t2 = high_resolution_clock::now();
        ocioso = duration_cast<duration<double> >(t2 - t1);

        accesstempoOcioso.acquire();
            tempoOcioso += ocioso.count();
            currentProductPage+="DownloadTime="+to_string(ocioso.count())+">";
        accesstempoOcioso.release();

        accessListofPages.acquire();
            listofPages.push_back(currentProductPage);
            listofPagesCount.release();
        accessListofPages.release();
    }
    noMorePages = true;
}

void produceUrls(list<string>& listofUrls, Semaphore& accessListofUrls, Semaphore& listofUrlCount, string url, bool& noMoreUrls, Semaphore& accesstempoOcioso, double& tempoOcioso, double& numProd){
    high_resolution_clock::time_point t1, t2;
    duration<double> ocioso;
    
    list<string> urlsPage;
    string nextPageUrl;
    
    t1 = high_resolution_clock::now();
        string page = download(url);
    t2 = high_resolution_clock::now();
    ocioso = duration_cast<duration<double> >(t2 - t1);
    accesstempoOcioso.acquire();
        tempoOcioso += ocioso.count();
    accesstempoOcioso.release();
    
    
    int total = totalPages(page);

    for(int p=1; p<=total; p++){
        urlsPage = findMatchesPages(page, total, p);
        nextPageUrl = urlsPage.back();
        urlsPage.pop_back();
        numProd+=urlsPage.size();
        accessListofUrls.acquire();
            for(auto u = urlsPage.begin(); u != urlsPage.end(); ++u){
                listofUrls.push_back(*u);
                listofUrlCount.release();
            }
        accessListofUrls.release();
        

        t1 = high_resolution_clock::now();
            page = download(nextPageUrl);
        t2 = high_resolution_clock::now();
        ocioso = duration_cast<duration<double> >(t2 - t1);
        accesstempoOcioso.acquire();
            tempoOcioso += ocioso.count();
        accesstempoOcioso.release();
    }
    noMoreUrls = true;
}

int main(int argc, char *argv[]) {
    high_resolution_clock::time_point total1, total2;
    duration<double> total;
    total1 = high_resolution_clock::now();

    Semaphore accessListofUrls(1);
    Semaphore accessListofPages(1);
    Semaphore accessJSON(1);
    Semaphore listofUrlCount(0);
    Semaphore listofPagesCount(0);

    list<string> listofUrls;
    list<string> listofPages;

    Semaphore accesstempoOcioso(1);
    double tempoOcioso=0;
    Semaphore accesstempoMedioPorProduto(1);
    double tempoMedioPorProduto=0;
    double numProd=0;

    bool noMorePages = false;
    bool noMoreUrls = false;

    string finalJSON = "[\n";

    // string url = "https://www.magazineluiza.com.br/dvd-player/tv-e-video/s/et/tvdb/";
    string url = argv[1];
    // int numProducers = 4;
    int numProducers = atoi(argv[2]);

    // int numConsumers = 5;
    int numConsumers = atoi(argv[3]);

    thread prodPageCollectorThread[numProducers];
    thread prodCollectorThread[numConsumers];

    thread prodLinkCollectorThread(produceUrls, std::ref(listofUrls), std::ref(accessListofUrls), std::ref(listofUrlCount), url, std::ref(noMoreUrls), std::ref(accesstempoOcioso), std::ref(tempoOcioso), std::ref(numProd));
    for(int p=0; p<numProducers; p++){
        prodPageCollectorThread[p] = thread(producePages, std::ref(listofUrls), std::ref(accessListofUrls), std::ref(listofUrlCount), std::ref(listofPages), std::ref(accessListofPages), std::ref(listofPagesCount), std::ref(noMoreUrls), std::ref(noMorePages), numProducers, std::ref(accesstempoOcioso), std::ref(tempoOcioso));
    }
    for(int c=0; c<numConsumers; c++){
        prodCollectorThread[c] = thread(consumePages,  std::ref(listofPages), std::ref(accessListofPages), std::ref(listofPagesCount), std::ref(accessJSON), std::ref(finalJSON), std::ref(noMorePages), numConsumers, std::ref(accesstempoMedioPorProduto), std::ref(tempoMedioPorProduto));
    }

    prodLinkCollectorThread.join();
    for(int p=0; p<numProducers; p++){
        prodPageCollectorThread[p].join();
    }
    for(int c=0; c<numConsumers; c++){
        prodCollectorThread[c].join();
    }

    finalJSON += "]\n";
    cout << finalJSON;
    cout << "Tempo total ocioso: " << tempoOcioso << '\n';
    cout << "Media de Tempo por produto: " << tempoMedioPorProduto/numProd << '\n';

    total2 = high_resolution_clock::now();
    total = duration_cast<duration<double> >(total2 - total1);
    cout << "Tempo total do program: " << total.count() << '\n';
    
    return 0;
}