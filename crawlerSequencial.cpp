#include <iostream>
#include <string>
#include <stdio.h>
#include <boost/regex.hpp>
#include <chrono>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>
#include <sstream>
#include <fstream>

using namespace std;
using namespace boost;
using namespace std::chrono;

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
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

void findMatches(string str, regex reg, vector<string>& result, int index){
    smatch matches;
    while(regex_search(str, matches, reg)){
        if(matches.size()> 0){
            result.push_back(matches[index]);
            str = matches.suffix().str(); 
        }
    }
}

int totalPages(string page){
    vector<string> lastPage;
    regex numPages("\"lastPage\":([^,]+)");
    findMatches(page, numPages, lastPage, 1);
    int totalPages = stoi(lastPage[0]);
    return totalPages;
}

vector<string> findMatchesPages(string page, int totalPages, int numPag){
    vector<string> urlsProducts;
    vector<string> lastPage;

    regex href("name=\"linkToProduct\" href=\"([^\"]+)");
    findMatches(page, href, urlsProducts, 1);

    if(numPag != totalPages){
        regex nextPage("<link rel=\"next\" href=\"([^\"]+)");
        findMatches(page, nextPage, lastPage, 1);
        lastPage[0] = "https://www.magazineluiza.com.br"+lastPage[0];
        urlsProducts.push_back(lastPage[0]);
        return urlsProducts;
    }
    else{
        urlsProducts.push_back("none");
        return urlsProducts;
    }
}

void collectProduct(string page, string url){
    vector<string> buffer;

    regex name ("<h1 class=\"header-product__title\" itemprop=\"name\">([^<]+)");
    findMatches(page, name, buffer, 1);
    string productName = buffer[0];

    buffer.clear();
    regex desc("<h2 class=\"description__product-title\">([^<]+)</h2>    <p class=\"description__text\"></p>([^<]+)");
    findMatches(page, desc, buffer, 2);
    string productDescription = buffer[0];

    buffer.clear();
    regex image("showcase-product__big-img js-showcase-big-img\" src=\"(https[^\"]+)");
    findMatches(page, image, buffer, 1);
    string productImage = buffer[0];

    buffer.clear();
    regex price("price-template__text[^>]+>([^<]+)</span>");
    findMatches(page, price, buffer, 1);
    string productPrice = buffer[0];

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

    string out = 
    "  {\n"
    "    \"nome\" : \"" + productName +"\",\n"
    "    \"descricao\" : \"" + productDescription +"\",\n"
    "    \"foto\" : \"" + productImage +"\",\n"
    "    \"preco\" : \"" + productPrice +"\",\n"
    "    \"preco_parcelado\" : \"" + precoParcelado +"\",\n"
    "    \"preco_num_parcelas\" : \"" + numeroParcelas +"\",\n"
    "    \"categoria\" : \"" + productCategory +"\",\n"
    "    \"url\" : \"" + url +"\",\n"
    "  },\n";

    cout << out;
}

int main(int argc, char** argv) {
    high_resolution_clock::time_point total1, total2;
    duration<double> tempoTotal;
    total1 = high_resolution_clock::now();

    string url = argv[1];

    vector<string> urls;
    string productPage;
    string nextPageUrl;

    high_resolution_clock::time_point t1, t2, t3;
    duration<double> ocioso;
    duration<double> tempoProd;
    double tempoOcioso;
    double tempoMedioPorProduto=0;
    double numProd=0;

    t1 = high_resolution_clock::now();
        string currentPage = download(url);
    t2 = high_resolution_clock::now();
    ocioso = duration_cast<duration<double> >(t2 - t1);
    tempoOcioso = ocioso.count();

    int total =  totalPages(currentPage);

    for(int p=1; p<=total; p++){
        cout << p << '\n';
        urls = findMatchesPages(currentPage, total, p);
        for(unsigned int u=0; u<urls.size()-1; u++){
            t1 = high_resolution_clock::now();
                    
                productPage = download(urls[u]);
            
            t2 = high_resolution_clock::now();
            ocioso = duration_cast<duration<double> >(t2 - t1);
            tempoOcioso += ocioso.count();

                collectProduct(productPage, urls[u]);

            t3 = high_resolution_clock::now();
            tempoProd = duration_cast<duration<double> >(t3 - t1);
            cout << "Tempo gasto no produto: " << tempoProd.count() << '\n';
            cout <<'\n';
            tempoMedioPorProduto +=tempoProd.count();
            numProd+=1;
        }
        nextPageUrl = urls[urls.size()-1];
        t1 = high_resolution_clock::now();
            currentPage = download(nextPageUrl);
        t2 = high_resolution_clock::now();
        ocioso = duration_cast<duration<double> >(t2 - t1);
        tempoOcioso += ocioso.count();
    }
    cout << "Tempo total ocioso: " << tempoOcioso << '\n';
    cout << "Media de tempo gasto nos produtos: " << tempoMedioPorProduto/numProd << '\n';
    total2 = high_resolution_clock::now();
    tempoTotal = duration_cast<duration<double> >(total2 - total1);
    cout << "Tempo total do program: " << tempoTotal.count() << '\n';

}