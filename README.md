# ParsingWebPages

Para o segundo projeto de super computação trabalhou-se com um problema em que concorrência tem um papel fundamental na obtenção de bom desempenho: download e análise de páginas web. Criou-se um crawler que identifica páginas de produtos em um site de e-commerce e extrai as informações básicas dos produtos.
Dada uma página de exibição de produto, o web crawler extrai as seguintes informações:

1. nome do produto
2. descrição do produto
3. url da foto do produto
4. preço à vista 
5. preço parcelado
6. categoria do produto
7. url da página de axibição

A identificação de páginas de produto é feita a partir de sua categoria, ou seja, o crawler desenvolvido é apontado para uma página com os produtos de uma categoria, e ele consegue obter as páginas de produto, lindando com possíveis paginação da listagem.

## URLs Suportadas

O crawler suporta as categorias de produtos do site https://www.magazineluiza.com.br/. Esses são alguns exemplos de categorias que se poderia fazer essa análise:

* [DVD Player](https://www.magazineluiza.com.br/dvd-player/tv-e-video/s/et/tvdb/) - https://www.magazineluiza.com.br/dvd-player/tv-e-video/s/et/tvdb/
* [Controle Remoto](https://www.magazineluiza.com.br/controle-remoto/tv-e-video/s/et/cmrt/) - https://www.magazineluiza.com.br/controle-remoto/tv-e-video/s/et/cmrt/
* [Roteadores](https://www.magazineluiza.com.br/roteador/informatica/s/in/rtdr/) - https://www.magazineluiza.com.br/roteador/informatica/s/in/rtdr/


## Pré-Requisito

Para conseguir compilar esse projeto é necessário as seguintes ferramentas e bibliotecas:

* [CMake](https://cmake.org/)
* [Boost.Regex](https://www.boost.org/doc/libs/1_66_0/libs/regex/doc/html/index.html)
* [Libcurl](https://curl.haxx.se/libcurl/)

## Compilação

Para compilar o projeto siga os seguintes passos:

```
cd build
cmake ..
make
```

## Rodando o programa

Dentro da pasta _build_, após a compilação é possível rodar a versão sequencial com o seguinte comando:
```
./crawlerSEQ url_da_listagem_por_categoria
```
Também é possível rodar a versão paralela com o seguinte comando: 
```
./crawlerPAR url_da_listagem_por_categoria numProducers numConsumers
```
No qual numProducers é o número de threads do tipo "produtores", ou seja, as que irão fazer os downloads das páginas, e numConsumers é o numero de threads do tipo "consumidores", as que irão fazer a análise das páginas.

### Output do Programa

O resultado de ambos os executáveis é escrio na saída padrão no formato json. Cada produto é um objeto com os seguintes campos: 

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
Além disso é escrito um arquivo de saída out.txt com informações para análise de tempo

```
tempoOcioso
numProd
tempoMedioPorProduto
tempoTotal 
```
tempoOcioso é o tempo total gasto esperando o download de páginas web, numProd é o número total de produtos analisados, tempoMedioPorProduto o tempo total de execução do programa dividido pelo total de produtos analisados e tempoTotal é o tempo total de execução do do programa.