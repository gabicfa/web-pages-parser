# Web Pages Parser

For Insper's Supercomputing module's firs project, we worked with a problem in which concurrency plays a fundamental role in achieving good performance: downloading and analyzing web pages. For this study, a crawler that identifies product pages on an e-commerce site and extracts basic product information was created. 
Given a product display page, the web crawler extracts the following information:

1. product name
2. product description
3. product photo url
4. spot price
5. installment price
6. product category
7. display page url

The identification of product pages is made based on their category, which means that the developed crawler is pointed to a page with the products' category and manages to obtain the product pages, dealing with possible listing pagination.

## Possible URLs

The crawler supports product categories from the website https://www.magazineluiza.com.br/. These are some examples of categories that could be done in this analysis:

* [DVD Player](https://www.magazineluiza.com.br/dvd-player/tv-e-video/s/et/tvdb/) - https://www.magazineluiza.com.br/dvd-player/tv-e-video/s/et/tvdb/
* [Remote Control](https://www.magazineluiza.com.br/controle-remoto/tv-e-video/s/et/cmrt/) - https://www.magazineluiza.com.br/controle-remoto/tv-e-video/s/et/cmrt/
* [Impressoras](https://www.magazineluiza.com.br/multifuncional-jato-de-tinta/informatica/s/in/majt/) - https://www.magazineluiza.com.br/multifuncional-jato-de-tinta/informatica/s/in/majt/


## Prerequisites
To be able to compile this project, the following tools and libraries are needed:

* [CMake](https://cmake.org/)
* [Boost.Regex](https://www.boost.org/doc/libs/1_66_0/libs/regex/doc/html/index.html)
* [Libcurl](https://curl.haxx.se/libcurl/)

## Compilation

Follow these steps to compile the project :

```
cd build
cmake ..
make
```


## Running the program

Inside the _build_ folder, after the compilation, it is possible to run the sequential version with the following command:
```
./crawlerSEQ url_da_listagem_por_categoria
```
It is also possible to run the parallel version with the following command:
```
./crawlerPAR url_da_listagem_por_categoria numProducers numConsumers
```
In which numProducers is the number of threads of type "producers," the ones downloading the pages, and numConsumers is the number of threads of type "consumers," the ones analyzing the pages.

### Program Output

The output of all executables is written in a standard JSON format. Each product is an object with the following fields:

```
{
    "nome": "",
    "descricao": "",
    "foto": "",
    "preco: 0,
    "preco_parcelado": 0,
    "preco_num_parcelas": 0,
    "categoria": "",
    "url": """
}
```
In addition, an out.txt output file is written with information for time analysis.

```
tempoOcioso
numProd
tempoMedioPorProduto
tempoTotal 
```
tempoOcioso is the total time spent waiting for the web pages to download, numProd is the total number of analyzed products, tempoMedioPorProduto is the total program execution time divided by the total analyzed products, and tempoTotal is the total program execution time

Created at 2018.2
