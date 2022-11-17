/*
O programa simula o setor de uma empresa grande de vendas, que por conta do grande volume de pedidos, precisa de uma solução ágil e eficiente para o controle de todas as entregas. O usuário final se trata do setor de transporte que através do programa poderá dar um suporte maior aos clientes em relação aos pedidos.
O programa irá armazenar  numero do pedido, nome do cliente, cpf do cliente e Endereco de entrega e o status do produto (se já foi entregue, ou se está a caminho), o numero do pedido, juntamente com  as outras informações são gerados pelo setor de vendas e passado para o setor de transportes.
O método Hash utilizado será o Endereçamento Aberto, a função de hash utilizada é a função de divisão, será usado variáveis dinâmicas.
Em média será possível acumular 50.000 pedidos.
Após a entrega o status do produto será alterado para entregue.
Funcionalidades
1 - Inserir (Inserir um novo pedido)
2 - Remover (Remover um pedido caso seja necessário)
3 - Buscar (Buscar um pedido)
4 - Listar (Listar pedidos )
5 - Alterar Status (Altera status da situação do pedido, se já foi entregue ou nao)

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define TAM 50000

//Criação da estrutura para armazenar os dados do pedido
typedef struct pedido{
    int numero_pedido;
    char nome_cliente[50];
    char cpf_cliente[15];
    char endereco_entrega[100];
    char status[20];    
}pedido;

//Criação da estrutura para a tabe
typedef struct tabela{ 
    pedido *pedidos; 
    int qtd; 
}tabela; 

//Função para criar a tabela
tabela* criar_tabela(){ 
    tabela *tab = (tabela*) malloc(sizeof(tabela)); 
    if(tab != NULL){ 
        tab->qtd = 0; 
        tab->pedidos = (pedido*) malloc(TAM * sizeof(pedido)); 
        if(tab->pedidos == NULL){ 
            free(tab); 
            return NULL; /
        } 
    }
    return tab; 
}

//Função para liberar a tabela
void liberar_tabela(tabela *tab){ 
    free(tab->pedidos); 
    free(tab); 
}  

//Função para calcular o hash
int funcao_hash(int chave){
    return chave % TAM; 
}

//Tratamento de colisão
int tratamento_colisao(int pos, int i){
    return (pos + i) % TAM;
}

//Função para inserir 
int inserir_pedido(tabela *tab, pedido ped){
    if(tab == NULL || tab->qtd == TAM){
        return 0;
    }
    int chave = ped.numero_pedido;
    int pos = funcao_hash(chave);
    pedido novo;
    novo.numero_pedido = ped.numero_pedido;
    strcpy(novo.nome_cliente, ped.nome_cliente);
    strcpy(novo.cpf_cliente, ped.cpf_cliente);
    strcpy(novo.endereco_entrega, ped.endereco_entrega);
    strcpy(novo.status, ped.status);
    int i = 0;
    while(i != TAM){
        if(tab->pedidos[pos].numero_pedido == 0){
            tab->pedidos[pos] = novo;
            tab->qtd++;
            return 1;
        }
        pos = tratamento_colisao(pos, i);
        i++;
    }
    return 0;
}

//Função para remover um pedido
int remover_pedido(tabela *tab, int chave){
    if(tab == NULL){
        return 0;
    }
    int pos = funcao_hash(chave);
    int i = 0;
    while(i != TAM){
        if(tab->pedidos[pos].numero_pedido == chave){
            tab->pedidos[pos].numero_pedido = 0;
            tab->qtd--;
            return 1;
        }
        pos = tratamento_colisao(pos, i);
        i++;
    }
    return 0;
}

//Função para buscar um pedido
int buscar_pedido(tabela *tab, int numero_pedido){
    if(tab == NULL){
        return 0;
    }
    int pos = funcao_hash(numero_pedido);
    int i = 0;
    while(i != TAM){
        if(tab->pedidos[pos].numero_pedido == numero_pedido){
            printf(" Numero do pedido: %d ", tab->pedidos[pos].numero_pedido);
            printf(" Nome do cliente: %s ", tab->pedidos[pos].nome_cliente);
            printf(" CPF do cliente: %s ", tab->pedidos[pos].cpf_cliente);
            printf(" Endereco de entrega: %s ", tab->pedidos[pos].endereco_entrega);
            printf(" Status do pedido: %s ", tab->pedidos[pos].status);
            return 1;
        }
        pos = tratamento_colisao(pos, i);
        i++;
    }
    return 0;
}
int buscar_pedido_aux(tabela *tab, int numero_pedido){
    if(tab == NULL){
        return 0;
    }
    int pos = funcao_hash(numero_pedido);
    int i = 0;
    while(i != TAM){
        if(tab->pedidos[pos].numero_pedido == numero_pedido){
            return 1;
        }
        pos = tratamento_colisao(pos, i);
        i++;
    }
    return 0;
}

//Função para listar os pedidos
void listar_pedidos(tabela *tab){
    if(tab == NULL){
        return;
    }
    int i;
    for(i = 0; i < TAM; i++){
        if(tab->pedidos[i].numero_pedido != 0){
            //printar os dados do pedido com quebra de linha para melhor visualização
            printf("\nNumero do pedido: %d ", tab->pedidos[i].numero_pedido);
            printf("\nNome do cliente: %s ", tab->pedidos[i].nome_cliente);
            printf("\nCPF do cliente: %s ", tab->pedidos[i].cpf_cliente);
            printf("\nEndereco de entrega: %s ", tab->pedidos[i].endereco_entrega);
            printf("Status do pedido: %s\n ", tab->pedidos[i].status);
            
        }
    }
}

//Função para perguntar ao usuario o novo status do pedido e alterar
int alterar_status(tabela *tab, int numero_pedido){
    if(tab == NULL){
        return 0;
    }
    int pos = funcao_hash(numero_pedido);
    int i = 0;
    while(i != TAM){
        if(tab->pedidos[pos].numero_pedido == numero_pedido){
            printf(" Digite o novo status do pedido: ");
            scanf("%s", tab->pedidos[pos].status);
            return 1;
        }
        pos = tratamento_colisao(pos, i);
        i++;
    }
    return 0;
}

//salvar os dados em um arquivo separando os campos por ; e cada pedido por |
void salvar_arquivo(tabela *tab){
    FILE *arq;
    arq = fopen("pedidos.txt", "w");
    if(arq == NULL){
        printf("Erro ao abrir o arquivo");
        exit(1);
    }
    int i;
    for(i = 0; i < TAM; i++){
        if(tab->pedidos[i].numero_pedido != 0){
            fprintf(arq, "%d;%s;%s;%s;%s|", tab->pedidos[i].numero_pedido, tab->pedidos[i].nome_cliente, tab->pedidos[i].cpf_cliente, tab->pedidos[i].endereco_entrega, tab->pedidos[i].status);
        }
    }
    fclose(arq);
}

//Colocar os dados do arquivo em uma lista depois colocar na tabela via inserir
void carregar_arquivo(tabela *tab){
    if(tab == NULL){
        return;
    }
    FILE *arq;
    arq = fopen("pedidos.txt", "r");
    if(arq == NULL){
        arq = fopen("pedidos.txt", "w");
        return;
    }
    pedido ped;
    while(!feof(arq)){
        //Insere os dados do arquivo na lista sem repetir os pedidos
        fscanf(arq, "%d;%[^;];%[^;];%[^;];%[^|]|", &ped.numero_pedido, ped.nome_cliente, ped.cpf_cliente, ped.endereco_entrega, ped.status);
        //Conferir se o pedido ja esta na lista, se ja estiver, nao insere e pula para o proximo
        if(buscar_pedido_aux(tab, ped.numero_pedido) == 1){
            continue;
        }
        //Insere o pedido na tabela
        inserir_pedido(tab, ped);
    }
    fclose(arq);
}

//Função para imprimir o menu
void menu(){
    printf("\n1 - Inserir Pedido\n ");
    printf(" 2 - Remover Pedido\n ");
    printf(" 3 - Buscar Pedido\n ");
    printf(" 4 - Listar Pedidos\n ");
    printf(" 5 - Alterar Status do Pedido\n ");
    printf(" 6 - Sair\n ");
}

//Função principal
int main(){
    tabela *tab = criar_tabela();
    carregar_arquivo(tab);
    int opcao;
    pedido ped;
    do{
        menu();
        printf(" Digite a opcao desejada: ");
        scanf("%d", &opcao);
        switch(opcao){
            case 1:
                printf(" Digite o numero do pedido: ");
                scanf("%d", &ped.numero_pedido);
                printf(" Digite o nome do cliente: ");
                fflush(stdin);
                gets(ped.nome_cliente);
                printf(" Digite o CPF do cliente: ");
                scanf("%s", ped.cpf_cliente);
                printf(" Digite o endereco de entrega: ");
                //aceita espaços no endereço
                fflush(stdin);
                fgets(ped.endereco_entrega, 100, stdin);
                printf(" Digite o status do pedido: ");
                fflush(stdin);
                gets(ped.status);
                inserir_pedido(tab, ped);
                break;
            case 2:
                printf(" Digite o numero do pedido: ");
                scanf("%d", &ped.numero_pedido);
                remover_pedido(tab, ped.numero_pedido);
                break;
            case 3:
                printf(" Digite o numero do pedido: ");
                scanf("%d", &ped.numero_pedido);
                buscar_pedido(tab, ped.numero_pedido);
                break;
            case 4:
                listar_pedidos(tab);
                break;
            case 5:
                printf(" Digite o numero do pedido: ");
                scanf("%d", &ped.numero_pedido);
                alterar_status(tab, ped.numero_pedido);
                break;
            case 6:
                salvar_arquivo(tab);
                liberar_tabela(tab);
                break;
            default:
                printf(" Opcao invalida! ");
                break;
        }
    }while(opcao != 6);
    return 0;
}