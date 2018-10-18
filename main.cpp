#include "HTTPDownloader.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <curl/curl.h>
#include <boost/regex.hpp>
#include <boost/locale.hpp>
#include <fstream>

using namespace std;
using namespace boost;

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
    cout << lastPage[0] << '\n';
    int totalPages = stoi(lastPage[0]);
    return totalPages;
}

void collectProduct(string url, ofstream &myfile, HTTPDownloader &downloader){
    string page = downloader.download(url);
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
    regex image("data-old-hires=\"(.*?)\"");
    findMatches(page, image, buffer, 1);
    string photo = buffer[0];

    buffer.clear();
    regex price("\"a-size-medium a-color-price\">([^<]+)");
    findMatches(page, price, buffer, 1);
    string priceString = buffer[0];

    buffer.clear();
    regex category("<a class=\"a-link-normal a-color-tertiary\" href=\"(.*?)(\n|\\s)+(.*?)(\n\\s)+");
    findMatches(page, category, buffer, 3);
    string categoryString = buffer[buffer.size()-1];

    myfile<< "{" << '\n';
    myfile << "   \"nome\": " <<  nameString << "," << '\n';
    myfile << "   \"description\": "<< description << "," <<  '\n';
    myfile << "   \"foto\": "<< photo << "," <<  '\n';
    myfile << "   \"preco\": "<< priceString << "," <<  '\n';
    myfile << "   \"preco_parcelado\": "<< "0" << "," <<  '\n';
    myfile << "   \"preco_num_parcelas\": "<< "0" << "," <<  '\n';
    myfile << "   \"categoria\": "<< categoryString << "," <<  '\n';
    myfile << "   \"url\": "<< url <<   '\n';
    myfile<< "}" << '\n';
    myfile <<'\n';
}

string findMatchesPages(string page, ofstream &myfile, HTTPDownloader &downloader, int totalPages, int numPag){
    vector<string> urlsProducts;
    vector<string> lastPage;

    regex href ("href=\"([^\"]+)\">(\n|\\s)+<div class=\"a-section aok-relative s-image-fixed-height\">");
    findMatches(page, href, urlsProducts, 1);
    for(unsigned int u=0; u < urlsProducts.size(); u++){
        string urlProd = urlsProducts.at(u);
        urlProd = "https://amazon.com.br"+urlProd;
        cout << urlProd << '\n';
        // collectProduct(urlProd, myfile, downloader);
    }
    if(numPag != totalPages){
        regex nextPage("<li class=\"a-last\"><a href=\"([^\"]+)\">");
        findMatches(page, nextPage, lastPage, 1);
        lastPage[0] = "https://amazon.com.br"+lastPage[0];
        cout << lastPage[0] << '\n';
        return lastPage[0];
    }
    else{
        return "none";
    }
}

string findMatchesFirstPage(string page, ofstream &myfile, HTTPDownloader &downloader){
    vector<string> urlsProducts;
    vector<string> urlProxPag;

    regex href ("href=\"([^\"]+)\"><h2");
    findMatches(page, href, urlsProducts, 1);

    for(unsigned int u=0; u < urlsProducts.size(); u++){
        string urlProd = urlsProducts.at(u);
        cout << urlProd << '\n';
        // collectProduct(urlProd, myfile, downloader);
    }
    
    regex proximaPag("href=\"([^\"]+)\">[\\s|\n]+<span id=\"pagnNextString\">");
    findMatches(page, proximaPag, urlProxPag, 1);
    urlProxPag[0] = "https://amazon.com.br"+urlProxPag[0];
    return urlProxPag[0];
}

int main(int argc, char** argv) {
    HTTPDownloader downloader;
    // string url = argv[1];
    string url = "https://www.amazon.com.br/s?i=outdoor&rh=n%3A17349396011%2Cn%3A17349397011%2Cn%3A17716665011%2Cn%3A17834036011%2Cn%3A17834350011&qid=1539783282&ref=sr_pg_1";
    
    ofstream myfile;
    myfile.open ("../products.json");
    
    string firtPage = downloader.download(url);
    string urlpage2 =findMatchesFirstPage(firtPage, myfile, downloader);
    string secondPage = downloader.download(urlpage2);
    
    int totalP = totalPages(secondPage);
    string nextURL=urlpage2;
    string nextPage = secondPage;

    for (int p= 2; p <= totalP; p++){
        nextURL = findMatchesPages(nextPage, myfile, downloader,totalP, p);
        nextPage = downloader.download(nextURL);
    }

    myfile.close();
    return 0;
}