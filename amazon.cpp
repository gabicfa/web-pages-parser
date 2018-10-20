#include "Producer.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
#include <boost/regex.hpp>
#include <chrono>

using namespace std;
using namespace boost;
using namespace std::chrono;


void findMatches(string str, regex reg, vector<string>& result, int index){
    smatch matches;
    while(regex_search(str, matches, reg)){
        result.push_back(matches[index]);
        str = matches.suffix().str();   
    }
}

int totalPages(string page){
    vector<string> lastPage;
    regex numPages("a-disabled\">(.*)</li>(\n|\\s)+<li class=\"a-last\"><a href=\"([^\"]+)\">");
    findMatches(page, numPages, lastPage, 1);
    int totalPages = stoi(lastPage[0]);
    return totalPages;
}

vector<vector<string>> findMatchesPages(string page, int totalPages, int numPag){
    vector<string> urlsProducts;
    vector<string> prices;
    vector<string> lastPage;
    vector<vector<string>> urlPriceLast;

    regex price("a-size-small a-color-price\">([^<]+)</span>");
    findMatches(page, price, prices, 1);

    regex href("href=\"([^\"]+)\">(\n|\\s)+<div class=\"a-section aok-relative s-image-fixed-height\">");
    findMatches(page, href, urlsProducts, 1);

    urlPriceLast.push_back(urlsProducts);
    urlPriceLast.push_back(prices);

    if(numPag != totalPages){
        regex nextPage("<li class=\"a-last\"><a href=\"([^\"]+)\">");
        findMatches(page, nextPage, lastPage, 1);
        lastPage[0] = "https://amazon.com.br"+lastPage[0];
        urlPriceLast.push_back(lastPage);
        return urlPriceLast;
    }
    else{
        lastPage[0] = "none";
        urlPriceLast.push_back(lastPage);
        return urlPriceLast;
    }
}

vector<vector<string>> findMatchesFirstPage(string page){
    vector<string> urlsProducts;
    vector<string> prices;
    vector<string> urlProxPag;
    vector<vector<string>> urlPriceNext;

    regex price("a-text-bold\">R([^<]+)<");
    findMatches(page, price, prices, 1);

    regex href ("href=\"([^\"]+)\"><h2");
    findMatches(page, href, urlsProducts, 1);

    regex proximaPag("href=\"([^\"]+)\">[\\s|\n]+<span id=\"pagnNextString\">");
    findMatches(page, proximaPag, urlProxPag, 1);
    urlProxPag[0] = "https://amazon.com.br"+urlProxPag[0];

    urlPriceNext.push_back(urlsProducts);
    urlPriceNext.push_back(prices);
    urlPriceNext.push_back(urlProxPag);
    return urlPriceNext;
}

void collectProduct(string page, string url, string price){
    vector<string> buffer;

    regex n ("<span id=\"productTitle\" class=\"a-size-large\">([^<]+)");
    findMatches(page, n, buffer, 1);
    regex espaco("[^(\n|\\s]+(.*)[^(\n|\\s]");
    string nameString = buffer[0];

    buffer.clear();
    findMatches(nameString, espaco, buffer, 0);
    nameString = buffer[0];

    buffer.clear();
    regex desc("product-description-and-details([^']+)auto(.*?)3Cdiv");
    findMatches(page, desc, buffer, 2);
    string description = buffer[0];

    buffer.clear();
    regex image("data-old-hires=\"(.*?)https(.*?).jpg");
    findMatches(page, image, buffer, 2);
    string photo = "https"+buffer[0];

    buffer.clear();
    regex category("<a class=\"a-link-normal a-color-tertiary\" href=\"(.*?)(\n|\\s)+(.*?)(\n\\s)+");
    findMatches(page, category, buffer, 3);
    string categoryString = buffer[buffer.size()-1];

    cout<< "{" << '\n';
    cout << "   \"nome\": " <<  nameString << "," << '\n';
    cout << "   \"descricao\": "<< description << "," <<  '\n';
    cout << "   \"foto\": "<< photo << "," <<  '\n';
    cout << "   \"preco\": "<< price << "," <<  '\n';
    cout << "   \"preco_parcelado\": "<< "0" << "," <<  '\n';
    cout << "   \"preco_num_parcelas\": "<< "0" << "," <<  '\n';
    cout << "   \"categoria\": "<< categoryString << "," <<  '\n';
    cout << "   \"url\": "<< url <<   '\n';
    cout<< "}" << '\n';
}

int main(int argc, char** argv) {
    Producer downloader;
    string url = argv[1];
    high_resolution_clock::time_point t1, t2, t3;
    duration<double> ocioso;
    duration<double> tempoProd;
    double numProd=0;
    double tempoMedioPorProduto;
    double tempoOcioso;

    // string url = "https://www.amazon.com.br/s?i=outdoor&rh=n%3A17349396011%2Cn%3A17349397011%2Cn%3A17716665011%2Cn%3A17834036011%2Cn%3A17834350011&qid=1539896129&ref=sr_pg_1";
    
    t1 = high_resolution_clock::now();
    string firtPage = downloader.download(url); 
    t2 = high_resolution_clock::now();
    ocioso = duration_cast<duration<double> >(t2 - t1);
    tempoOcioso = ocioso.count();
    
    vector<vector<string>> urlsPricesPage1 =findMatchesFirstPage(firtPage);
    vector<string> urlsPage1 = urlsPricesPage1[0];
    vector<string> pricesPage1 = urlsPricesPage1[1];
    string urlPage2 = urlsPricesPage1[2][0];

    string productPage;
    for(unsigned int u=0; u < urlsPage1.size(); u++){
        string urlProd = urlsPage1[u]; 

        t1 = high_resolution_clock::now();
        productPage = downloader.download(urlProd);
        t2 = high_resolution_clock::now();
        ocioso = duration_cast<duration<double> >(t2 - t1);
        tempoOcioso += ocioso.count();

        collectProduct(productPage, urlProd, pricesPage1[u]);
        t3 = high_resolution_clock::now();
        tempoProd = duration_cast<duration<double> >(t3 - t1);
        cout << "Tempo gasto no produto: " << tempoProd.count() << '\n';
        cout <<'\n';
        tempoMedioPorProduto +=tempoProd.count();
        numProd+=1;
    }

    t1 = high_resolution_clock::now();
    string secondPage = downloader.download(urlPage2);
    t2 = high_resolution_clock::now();
    ocioso = duration_cast<duration<double> >(t2 - t1);
    tempoOcioso += ocioso.count();
    
    int totalP = totalPages(secondPage);

    vector<vector<string>> urlsPricesCurrentPage;
    vector<string> urlsCurrentPage;
    vector<string> pricesCurrentPage;
    string currentPage = secondPage;
    string nextPageUrl;
    

    totalP = 5;

    for (int p= 2; p <= totalP; p++){

        urlsPricesCurrentPage = findMatchesPages(currentPage, totalP, p);
        urlsCurrentPage = urlsPricesCurrentPage[0];
        pricesCurrentPage= urlsPricesCurrentPage[1];
        nextPageUrl = urlsPricesCurrentPage[2][0];
        
        for(unsigned int u=0; u < urlsCurrentPage.size(); u++){
            string urlProd = urlsCurrentPage[u];
            urlProd = "https://amazon.com.br"+urlProd;

            t1 = high_resolution_clock::now();
            productPage = downloader.download(urlProd);
            t2 = high_resolution_clock::now();
            ocioso = duration_cast<duration<double> >(t2 - t1);
            tempoOcioso += ocioso.count();

            collectProduct(productPage, urlProd, pricesCurrentPage[u]);
            t3 = high_resolution_clock::now();
            tempoProd = duration_cast<duration<double> >(t3 - t1);
            cout << "Tempo gasto no produto: " << tempoProd.count() << '\n';
            cout <<'\n';
            tempoMedioPorProduto +=tempoProd.count();
            numProd+=1;
        }

        t1 = high_resolution_clock::now();
        currentPage = downloader.download(nextPageUrl);
        t2 = high_resolution_clock::now();
        ocioso = duration_cast<duration<double> >(t2 - t1);
        tempoOcioso += ocioso.count();
    }    

    cout << "Tempo total ocioso: " << tempoOcioso << '\n';

    return 0;
}