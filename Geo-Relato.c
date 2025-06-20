// Texto teste apenas para aplicar atualizações no github
#include <stdio.h> // Biblioteca padrão de entrada e saída (para printf, scanf, fgets, etc.)
#include <string.h> // Biblioteca para manipulação de strings (para strlen, strcpy, strcmp, strcspn)
#include <time.h>   // Biblioteca para manipulação de tempo e datas (para time_t, struct tm, mktime)
#include <ctype.h>  // Biblioteca para manipulação de caracteres (para tolower)
#include <math.h>   // Biblioteca para funções matemáticas (para sin, cos, atan2, sqrt, M_PI)
#include <stdlib.h> // Para EXIT_FAILURE (usado para indicar falha na execução do programa)

// Define o número máximo de relatos que o sistema pode armazenar
#define MAX_RELATOS 100
// Define o raio máximo em quilômetros para considerar um desastre próximo à base
#define RAIO_MAX_KM 10.0

// --- Definição da Estrutura de Dados ---
// Define a estrutura 'Relator' para armazenar as informações de um relato de catástrofe
struct Relator {
    char nome[50];         // Nome completo do relator
    int documento;         // Número do documento do relator
    char email[50];        // Endereço de e-mail do relator
    int telefone;          // Número de telefone do relator
    double lat_base;       // Latitude da localização base (guardada para ilustração, não essencial para o struct em si)
    double lon_base;       // Longitude da localização base (idem)
    double lat_desastre;   // Latitude do local da catástrofe
    double lon_desastre;   // Longitude do local da catástrofe
    char tipo[50];         // Tipo da catástrofe (ex: enchente, incêndio)
    char descricao[50];    // Descrição detalhada da catástrofe
    char data[50];         // Data e hora da catástrofe no formato "dd/mm/aaaa hh:mm:ss"
};

// --- Funções Auxiliares ---

// Converte uma string de data e hora para um timestamp (time_t)
// Retorna -1 em caso de erro na conversão
time_t stringParaTimestamp(const char* dataStr) {
    struct tm dt = {0}; // Inicializa uma estrutura tm (que representa uma data e hora quebrada) com zeros
    int dia, mes, ano, hora, min, seg;

    // Tenta ler a string no formato dd/mm/aaaa hh:mm:ss
    if (sscanf(dataStr, "%d/%d/%d %d:%d:%d", &dia, &mes, &ano, &hora, &min, &seg) != 6) {
        return -1; // Retorna -1 se não conseguir ler todos os 6 componentes
    }

    // Preenche a estrutura tm com os valores lidos
    dt.tm_mday = dia;     // Dia do mês
    dt.tm_mon = mes - 1;  // Mês (janeiro = 0, então subtrai 1)
    dt.tm_year = ano - 1900; // Ano desde 1900
    dt.tm_hour = hora;    // Hora
    dt.tm_min = min;      // Minuto
    dt.tm_sec = seg;      // Segundo

    return mktime(&dt); // Converte a estrutura tm para um timestamp (número de segundos desde 1º jan 1970)
}

// Função Haversine para calcular a distância entre dois pontos geográficos (latitude, longitude) em quilômetros
double distancia_km(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371.0; // Raio médio da Terra em quilômetros
    // Converte as diferenças de latitude e longitude de graus para radianos
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    // Aplica a fórmula de Haversine
    double a = sin(dLat/2) * sin(dLat/2) +
               cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
               sin(dLon/2) * sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    return R * c; // Retorna a distância em km
}

// --- Funções de Operação do Sistema ---

// Função para registrar um novo relato de catástrofe
void registro(struct Relator *r, double baseLat, double baseLon) {
    printf("\n--- Registro ---\n");

    printf("Dados do relator\n");

    printf("Nome completo: ");
    // Usa fgets para ler a linha completa, incluindo espaços
    fgets(r->nome, sizeof(r->nome), stdin);
    // Remove o caractere de nova linha ('\n') que fgets pode incluir
    r->nome[strcspn(r->nome, "\n")] = 0;

    printf("Documento: ");
    // Lê o documento como um inteiro
    if (scanf("%d", &r->documento) != 1) {
        printf("Erro ao ler o documento.\n");
        // Limpa o buffer de entrada para evitar problemas com leituras futuras
        while(getchar() != '\n');
        return; // Sai da função em caso de erro
    }
    getchar(); // Consome o '\n' deixado pelo scanf

    printf("E-mail: ");
    fgets(r->email, sizeof(r->email), stdin);
    r->email[strcspn(r->email, "\n")] = 0;

    printf("Telefone: ");
    if (scanf("%d", &r->telefone) != 1) {
        printf("Erro ao ler o telefone.\n");
        while(getchar() != '\n');
        return;
    }
    getchar();

    printf("\nDados da Catástrofe\n");

    printf("Tipo de catástrofe (ex: enchente, incêndio, deslizamento): ");
    fgets(r->tipo, sizeof(r->tipo), stdin);
    r->tipo[strcspn(r->tipo, "\n")] = 0;

    printf("Descrição: ");
    fgets(r->descricao, sizeof(r->descricao), stdin);
    r->descricao[strcspn(r->descricao, "\n")] = 0;

    printf("Data e hora (dd/mm/aaaa hh:mm:ss): ");
    fgets(r->data, sizeof(r->data), stdin);
    r->data[strcspn(r->data, "\n")] = 0;

    // Coordenadas do desastre
    printf("Latitude do desastre (decimal): ");
    if (scanf("%lf", &r->lat_desastre) != 1) {
        printf("Erro ao ler a latitude do desastre.\n");
        while(getchar() != '\n');
        return;
    }
    printf("Longitude do desastre (decimal): ");
    if (scanf("%lf", &r->lon_desastre) != 1) {
        printf("Erro ao ler a longitude do desastre.\n");
        while(getchar() != '\n');
        return;
    }
    getchar();

    // Calcula a distância da localização base para o local do desastre
    double dist = distancia_km(baseLat, baseLon, r->lat_desastre, r->lon_desastre);
    // Verifica se a distância excede o raio máximo permitido
    if (dist > RAIO_MAX_KM) {
        printf("\n*** Atenção: o desastre está a %.2f km do centro base (limite %.1fkm).\n", dist, RAIO_MAX_KM);
    } else {
        printf("\nDesastre dentro do raio permitido (%.2f km).\n", dist);
    }
}

// Função para listar todos os relatos cadastrados
void listar(struct Relator relatos[], int total) {
    // Verifica se há relatos para listar
    if (total == 0) {
        printf("\nNenhum relato cadastrado ainda.\n");
        return;
    }

    printf("\n--- Lista de Relatos Cadastrados ---\n");

    // Itera sobre o array de relatos e imprime as informações de cada um
    for (int i = 0; i < total; i++) {
        printf("\nRelato #%d", i + 1);
        printf("\nNome: %s", relatos[i].nome);
        printf("\nDocumento: %d", relatos[i].documento);
        printf("\nE-mail: %s", relatos[i].email);
        printf("\nTelefone: %d", relatos[i].telefone);
        printf("\nTipo de catástrofe: %s", relatos[i].tipo);
        printf("\nDescrição: %s", relatos[i].descricao);
        printf("\nData e hora: %s", relatos[i].data);
        printf("\nLocalização do desastre: Lat %.6f, Lon %.6f\n", relatos[i].lat_desastre, relatos[i].lon_desastre);
    }
}

// Compara duas strings ignorando maiúsculas/minúsculas
// Retorna 1 se as strings forem iguais (ignorando caso), 0 caso contrário
int comparaIgnoreCase(const char *a, const char *b) {
    // Enquanto houver caracteres em ambas as strings
    while (*a && *b) {
        // Compara os caracteres convertidos para minúsculas
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0; // Se diferentes, retorna 0 (não são iguais)
        }
        a++; // Avança para o próximo caractere na string 'a'
        b++; // Avança para o próximo caractere na string 'b'
    }
    // Retorna 1 se ambas as strings terminaram (são iguais)
    return (*a == '\0' && *b == '\0');
}

// Função para procurar relatos por tipo de catástrofe
void procurar_tipo(struct Relator relatos[], int total) {
    if (total == 0) {
        printf("Nenhum relato cadastrado.\n");
        return;
    }

    char busca[50]; // String para armazenar o tipo de catástrofe a ser buscado
    int encontrado = 0; // Flag para indicar se algum relato foi encontrado

    printf("Digite o tipo de catástrofe que deseja encontrar: ");
    fgets(busca, sizeof(busca), stdin);
    busca[strcspn(busca, "\n")] = 0; // Remove o '\n'

    // Itera sobre os relatos para encontrar correspondências
    for (int i = 0; i < total; i++) {
        // Compara o tipo do relato com o termo de busca, ignorando maiúsculas/minúsculas
        if (comparaIgnoreCase(relatos[i].tipo, busca)) {
            encontrado = 1; // Marca que um relato foi encontrado
            printf("\nRelato #%d encontrado:\n", i + 1);
            printf("Nome: %s\n", relatos[i].nome);
            printf("Documento: %d\n", relatos[i].documento);
            printf("E-mail: %s\n", relatos[i].email);
            printf("Telefone: %d\n", relatos[i].telefone);
            printf("Tipo de catástrofe: %s\n", relatos[i].tipo);
            printf("Descrição: %s\n", relatos[i].descricao);
            printf("Data e hora: %s\n", relatos[i].data);
            printf("Localização do desastre: Lat %.6f, Lon %.6f\n", relatos[i].lat_desastre, relatos[i].lon_desastre);
        }
    }

    // Se nenhum relato foi encontrado, exibe uma mensagem
    if (!encontrado) {
        printf("Nenhum relato encontrado com o tipo '%s'.\n", busca);
    }
}

// Função para procurar relatos por localização (dentro de um raio da base)
void procurar_localizacao(struct Relator relatos[], int total, double baseLat, double baseLon) {
    if (total == 0) {
        printf("Nenhum relato cadastrado.\n");
        return;
    }

    double raio;
    printf("Digite o raio em km para busca por localização: ");
    if (scanf("%lf", &raio) != 1) {
        printf("Erro ao ler o raio.\n");
        while(getchar() != '\n');
        return;
    }
    getchar();

    int encontrados = 0;
    printf("\n--- Relatos dentro de %.2f km da base ---\n", raio);
    for (int i = 0; i < total; i++) {
        // Calcula a distância do relato à base
        double dist = distancia_km(baseLat, baseLon, relatos[i].lat_desastre, relatos[i].lon_desastre);
        // Se a distância for menor ou igual ao raio especificado
        if (dist <= raio) {
            printf("\nRelato #%d:", i + 1);
            printf("\nNome: %s", relatos[i].nome);
            printf("\nDocumento: %d", relatos[i].documento);
            printf("\nE-mail: %s", relatos[i].email);
            printf("\nTelefone: %d", relatos[i].telefone);
            printf("\nTipo de catástrofe: %s", relatos[i].tipo);
            printf("\nDescrição: %s", relatos[i].descricao);
            printf("\nData e hora: %s", relatos[i].data);
            printf("\nLocalização do desastre: Lat %.6f, Lon %.6f\n", relatos[i].lat_desastre, relatos[i].lon_desastre);
            printf("Distância: %.2f km\n", dist);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum relato encontrado dentro do raio especificado.\n");
    }
}

// Função para procurar relatos por período de tempo
void procurar_periodo(struct Relator relatos[], int total) {
    if (total == 0) {
        printf("\nNenhum relato cadastrado.\n");
        return;
    }

    char inicioStr[50], fimStr[50]; // Strings para armazenar as datas de início e fim
    time_t inicio, fim;             // Timestamps para as datas de início e fim

    getchar();  // Limpa o buffer para o fgets funcionar corretamente

    printf("Digite a data INICIAL (dd/mm/aaaa hh:mm:ss): ");
    fgets(inicioStr, sizeof(inicioStr), stdin);
    inicioStr[strcspn(inicioStr, "\n")] = 0;

    printf("Digite a data FINAL (dd/mm/aaaa hh:mm:ss): ");
    fgets(fimStr, sizeof(fimStr), stdin);
    fimStr[strcspn(fimStr, "\n")] = 0;

    // Converte as strings de data para timestamps
    inicio = stringParaTimestamp(inicioStr);
    fim = stringParaTimestamp(fimStr);

    // Verifica se houve erro na conversão das datas
    if (inicio == -1 || fim == -1) {
        printf("Erro ao converter as datas.\n");
        return;
    }

    printf("\n--- Relatos entre %s e %s ---\n", inicioStr, fimStr);

    int encontrados = 0;
    for (int i = 0; i < total; i++) {
        time_t dataRelato = stringParaTimestamp(relatos[i].data); // Converte a data do relato para timestamp
        // Verifica se a data do relato está dentro do período especificado
        if (dataRelato >= inicio && dataRelato <= fim) {
            printf("\nRelato #%d", i + 1);
            printf("\nNome: %s", relatos[i].nome);
            printf("\nDocumento: %d", relatos[i].documento);
            printf("\nE-mail: %s\n", relatos[i].email);
            printf("\nTelefone: %d\n", relatos[i].telefone);
            printf("\nTipo de catástrofe: %s\n", relatos[i].tipo);
            printf("\nDescrição: %s\n", relatos[i].descricao);
            printf("\nData e hora: %s\n", relatos[i].data);
            printf("\nLocalização do desastre: Lat %.6f, Lon %.6f\n", relatos[i].lat_desastre, relatos[i].lon_desastre);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum relato encontrado neste período.\n");
    }
}

// --- Função Principal ---

int main() {
    // Declara um array de estruturas Relator para armazenar os relatos
    struct Relator relatos[MAX_RELATOS];
    int total = 0; // Contador de relatos cadastrados
    int op;        // Variável para armazenar a opção do menu

    double baseLat, baseLon; // Coordenadas da base, usadas para calcular distância

    // Solicita ao usuário as coordenadas da base
    printf("Digite a coordenada BASE (latitude decimal): ");
    if (scanf("%lf", &baseLat) != 1) {
        printf("Erro ao ler a latitude base.\n");
        return EXIT_FAILURE; // Sai do programa com erro
    }
    printf("Digite a coordenada BASE (longitude decimal): ");
    if (scanf("%lf", &baseLon) != 1) {
        printf("Erro ao ler a longitude base.\n");
        return EXIT_FAILURE;
    }
    getchar(); // Consome o '\n'

    // Loop principal do menu
    do {
        printf("\nGeoRelato:\n");
        printf("1 - Registrar relato\n");
        printf("2 - Listar relatos\n");
        printf("3 - Buscar por tipo de catástrofe\n");
        printf("4 - Buscar por período\n");
        printf("5 - Buscar por localização\n");
        printf("6 - Sair\n");
        printf("Escolha a opção: ");
        if (scanf("%d", &op) != 1) {
            printf("Opção inválida. Saindo.\n");
            break; // Sai do loop se a entrada não for um número
        }
        getchar(); // Consome o '\n'

        // Executa a ação de acordo com a opção escolhida pelo usuário
        switch(op) {
            case 1:
                if (total < MAX_RELATOS) {
                    registro(&relatos[total], baseLat, baseLon); // Chama a função de registro
                    // Incrementa o contador de relatos. Idealmente, a função registro retornaria
                    // um status para indicar se o registro foi bem-sucedido.
                    total++;
                } else {
                    printf("Limite máximo de relatos atingido.\n");
                }
                break;
            case 2:
                listar(relatos, total); // Chama a função para listar relatos
                break;
            case 3:
                procurar_tipo(relatos, total); // Chama a função de busca por tipo
                break;
            case 4:
                procurar_periodo(relatos, total); // Chama a função de busca por período
                break;
            case 5:
                procurar_localizacao(relatos, total, baseLat, baseLon); // Chama a função de busca por localização
                break;
            case 6:
                printf("Encerrando o programa.\n"); // Mensagem de saída
                break;
            default:
                printf("Opção inválida.\n"); // Mensagem para opção inválida
        }

    } while (op != 6); // Continua o loop até que o usuário escolha sair (opção 6)

    return 0; // Indica que o programa terminou com sucesso
}