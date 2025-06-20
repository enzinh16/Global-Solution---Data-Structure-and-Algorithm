
# Relatório de Sistema de Registro de Catástrofes em C - GeoRelato
Esse programa, feito em C, foi desenvolvido para coletar e gerenciar informações sobre relatos de catástrofes. Ele armazena dados detalhados tanto do relator quanto do evento em si.

## Coleta de Informações:
### Para o relator, o sistema solicita e armazena:
* Nome completo

* Documento

* E-mail

* Telefone

### Já para o desastre, são coletados:
* Tipo de catástrofe (ex: enchente, incêndio, deslizamento)

* Descrição detalhada do ocorrido

* Data e hora do evento

* Localização do desastre (latitude e longitude decimais)

* Métrica de Proximidade e Alerta Geográfico

## Início
No início da execução, o programa solicita as coordenadas base (latitude e longitude). Essa base é crucial para uma métrica de proximidade do projeto. Quando um novo relato é registrado, o sistema calcula a distância entre o local do desastre e a coordenada base utilizando a fórmula de Haversine.

Se a distância for maior que um raio predefinido de 10 km, o programa exibe um alerta, informando a distância exata do desastre em relação à base e quantos quilômetros excedeu o limite.

Caso contrário, se a distância estiver dentro ou igual aos 10 km, ele simplesmente confirma a proximidade e a distância até a base.

## Interface do Usuário e Opções do Menu
Após a definição das coordenadas base, o usuário é apresentado a um menu interativo com seis opções principais:

1. Registrar relato: Permite adicionar um novo registro de catástrofe.

2. Listar relatos: Exibe todos os relatos cadastrados no sistema.

3. Buscar por tipo de catástrofe: Permite pesquisar relatos com base no tipo de evento.

4. Buscar por período: Habilita a busca de relatos dentro de um intervalo de datas e horas específico.

5. Buscar por localização: Permite encontrar relatos dentro de um raio geográfico definido pelo usuário, em relação à coordenada base.

6. Sair: Encerra a execução do programa.

## Comportamento das Opções
**Buscas Iniciais:** Se qualquer uma das opções de busca (tipo, período ou localização) for selecionada antes de haver qualquer relato cadastrado, o sistema irá informar "Nenhum relato cadastrado ainda."

**Registro de Relato:** Ao escolher a opção de registro, o programa guia o usuário através da entrada de dados do relator e da catástrofe, conforme detalhado no início deste relatório, incluindo a validação da distância em relação à base.

**Saída do Programa:** A opção "Sair" exibe uma mensagem de encerramento e finaliza o loop principal do programa (do-while), retornando 0 na função main(), assim encerrando o programa.

## Como as Buscas Funcionam
### Listar Relatos
> A funcionalidade de listar relatos é a mais direta: ela simplesmente percorre a lista de todos os relatos atualmente cadastrados, utilizando um loop for que itera do primeiro ao último registro. Para cada relato, o sistema imprime todas as informações armazenadas (nome, documento, e-mail, telefone do relator, e tipo, descrição, data/hora e localização do desastre), de forma organizada.

### Buscar por Tipo de Catástrofe
> A busca por tipo de catástrofe funciona da seguinte maneira: o programa solicita ao usuário que digite o tipo de catástrofe desejado (por exemplo, "enchente"). Em seguida, ele percorre todos os relatos armazenados e compara o tipo de cada relato com o termo de busca fornecido. A comparação é realizada de forma case-insensitive (ignorando maiúsculas e minúsculas) para garantir que "Enchente" e "enchente" sejam tratados como o mesmo tipo. Se uma correspondência for encontrada, todos os detalhes do relato são exibidos. Ao final da busca, se nenhum relato corresponder ao tipo especificado, uma mensagem informará que "Nenhum relato encontrado com o tipo 'X'".

### Buscar por Período
> A busca por período permite que o usuário encontre relatos que ocorreram dentro de um intervalo de tempo específico. O usuário deve informar a data e hora inicial e a data e hora final no formato "dd/mm/aaaa hh:mm:ss". O programa então converte essas strings de data para um formato de timestamp (time_t), que é um número que representa o tempo decorrido desde uma data de referência. Cada relato cadastrado também tem sua data convertida para timestamp. Em seguida, o sistema verifica se o timestamp de cada relato está entre os timestamps inicial e final. Todos os relatos que se encaixam nesse período são exibidos com suas informações completas.

### Buscar por Localização
> A busca por localização permite encontrar desastres que ocorreram dentro de um raio específico de quilômetros a partir da coordenada base definida no início do programa. O usuário insere o raio desejado (ex: 5 km). O sistema, então, para cada relato cadastrado, utiliza a função Haversine para calcular a distância entre o local do desastre e a coordenada base. Se a distância calculada for menor ou igual ao raio especificado pelo usuário, os detalhes completos do relato são exibidos, juntamente com a distância exata à base.

## Conclusão
Este sistema de registro de catástrofes oferece uma solução prática para coletar, organizar e consultar informações sobre eventos importantes, auxiliando no gerenciamento e na resposta a desastres.