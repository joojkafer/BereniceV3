#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int codigo;
    char nome[25];
    float valorUnitario;
    int quantidade;
    //int quantidadeVendidos;
}Produto;

Produto *produtos = NULL;
int indiceG = 0;

//MENUS
int menuPrincipal();
void menuProdutos();
void menuVendas();

//MENU PRODUTOS
void exibirProdutos();
void cadastrarProduto();
void atualizarProduto();
void excluirProduto();
void salvarProdutos();
void lerProdutos();

//MENU VENDAS
void relizarVendas();

//OUTRAS FUNÇÕES
void listarProdutos();
void primeiraLeitura();
float subtotal(int codigo, int quantidade);
void cupomFiscal();

int main(){

    primeiraLeitura();

    int op;
    char continuar = 's';

    while(continuar == 's' || continuar == 'S'){

        op = menuPrincipal();

        switch(op){
            case 1:
                menuProdutos();
                break;
            case 2:
                menuVendas();
                break;
            case 3:
                printf("\nSair");
                break;
        }
    }

    return 0;
}

void primeiraLeitura(){

    //int qprtLidos;
    Produto prtLidos;

    FILE *arqProdutos;
    arqProdutos = fopen("produtos.txt", "r");

    if (arqProdutos == NULL) {
        printf("\nErro ao abrir o arquivo.\n");
    }

    while (fscanf(arqProdutos, "%d %25s %f %d", &prtLidos.codigo, prtLidos.nome, &prtLidos.valorUnitario, &prtLidos.quantidade) == 4) {
        //printf("Codigo: %d\nNome: %-25s\nValor: %.2f\nQuantidade: %d\n--\n", prtLidos.codigo, prtLidos.nome, prtLidos.valorUnitario, prtLidos.quantidade);

        produtos = (Produto *)realloc(produtos, (indiceG + 1) * sizeof(Produto));
        produtos[indiceG++] = prtLidos;

        //qprtLidos++;
    }

    fclose(arqProdutos);

    //printf("\nForam lidos %i produtos.\n", qprtLidos);

    //menuProdutos();
}

int menuPrincipal(){

    int op;

    printf("_________________________________\n");
    printf("\nBem vindo a Padaria da Berenice!\n\n");

    do{
        printf("  1 - Produtos\n");
        printf("  2 - Vendas\n");
        printf("  3 - Sair\n");

        printf("\nDigite a opcao escolhida: ");
        scanf("%i", &op);
        getchar();

        if(op < 1 || op > 3){
            printf("Opcao invalida, tente novamente.\n\n");
        }
    }while(op < 1 || op > 3);

    return op;
}

void menuProdutos(){

    int op;

    do{
        printf("\nMENU DE PRODUTOS \n\n");

        printf("  1 - Exibir\n");
        printf("  2 - Cadastrar\n");
        printf("  3 - Atualizar\n");
        printf("  4 - Excluir\n");
        printf("  5 - Salvar\n");
        printf("  6 - Ler\n");
        printf("  7 - Voltar\n");

        printf("\nDigite a opcao escolhida: ");
        scanf("%i", &op);
        getchar();

        switch(op){
            case 1:
                exibirProdutos();
                break;
            case 2:
                cadastrarProduto();
                break;
            case 3:
                atualizarProduto();
                break;
            case 4:
                excluirProduto();
                break;
            case 5:
                salvarProdutos();
                break;
            case 6:
                lerProdutos();
                break;
            case 7:
                menuPrincipal();
                break;
        }

        if(op < 1 || op > 7){
            printf("Opcao invalida, tente novamente.\n");
        }
    }while(op < 1 || op > 7);
}

void listarProdutos(){

    int i;

    printf("\nProdutos registrados: \n\n");
    printf("Codigo    | Item                 | Valor Unitario  | Estoque\n");
    for(i=0;i<indiceG;i++){
        printf("  %-3d     | %-20s |    R$ %-6.2f    |    %-3d\n",
        produtos[i].codigo, produtos[i].nome, produtos[i].valorUnitario, produtos[i].quantidade);
    }
}

void exibirProdutos(){

    if(produtos == NULL || indiceG == 0){
        printf("\nNao ha nenhum produto cadastrado.\n");
        menuProdutos();
    }else{
        listarProdutos();
    }

    menuProdutos();
}

void cadastrarProduto(){

    Produto novoProduto;

    //int codigo, quantidade;
    int i;
    int validacod = 1, validastr = 1;

    printf("\nCadastrar novo produto\n\n");

    printf("Preencha os dados do novo produto:\n");

    do{
        printf("Codigo: ");
        scanf("%i", &novoProduto.codigo);
        getchar();

        //printf("%i", novoProduto.codigo);
        if(novoProduto.codigo <= 0){
            printf("\nCodigo invalido, tente novamente.\n");
        }else{
            validacod = 1;
            for(i=0;i<indiceG;i++){
                if(novoProduto.codigo == produtos[i].codigo){
                    printf("\nCodigo ja cadastrado, tente novamente.\n");
                    validacod = 0;
                    break;
                }
            }
        }
    }while(novoProduto.codigo <= 0 || validacod == 0);

    do{
        printf("Item: ");
        gets(novoProduto.nome);
        fflush(stdin);

        //printf("%s", novoProduto.nome);
        for(i=0;novoProduto.nome[i] != '\0';i++){
            if (novoProduto.nome[i] != ' '){
                validastr = 0;
                break;
            }
        }
        if(validastr == 1){
            printf("\nNome vazio, tente novamente.\n");
        }

        if(strlen(novoProduto.nome) > 25){
            printf("\nNome invalido, tente novamente.\n");
        }
    }while(strlen(novoProduto.nome) > 25 || validastr == 1);

    do{
        printf("Valor unitario: ");
        scanf("%f", &novoProduto.valorUnitario);
        getchar();

        //printf("%f", novoProduto.valorUnitario);
        if(novoProduto.valorUnitario <= 0){
            printf("\nValor invalido, tente novamente.\n");
        }
    }while(novoProduto.valorUnitario <= 0);

    do{
        printf("Estoque: ");
        scanf("%i", &novoProduto.quantidade);
        getchar();

        if(novoProduto.quantidade < 0){
            printf("\nQuantidade invalida, tente novamente.\n");
        }
    }while(novoProduto.quantidade < 0);

    produtos = (Produto *)realloc(produtos,(indiceG+1)*sizeof(Produto));
    produtos[indiceG] = novoProduto;

    printf("\nProduto cadastrado com sucesso!\n");
    printf("Codigo: %i\n", produtos[indiceG].codigo);
    printf("Item: %s\n", produtos[indiceG].nome);
    printf("Valor unitario: %.2f\n", produtos[indiceG].valorUnitario);
    printf("Estoque: %i\n", produtos[indiceG].quantidade);

    indiceG++;

    menuProdutos();
}

void atualizarProduto(){

    int i;
    int codigo, novoQuant, encontrado;
    float novoValor;

    if(produtos == NULL || indiceG == 0){
        printf("\nNenhum produto cadastrado.\n");
        menuProdutos();
    }else{
        printf("\nAtualizar estoque/valor de produto\n");

        listarProdutos();
        do{
            printf("\nDigite o codigo do produto p/ alterar: ");
            scanf("%i", &codigo);
            getchar();

            encontrado = 0;
            for(i=0;i<indiceG;i++){
                if(codigo == produtos[i].codigo){
                    encontrado = 1;
                    printf("\nProduto encontrado:\n");
                    printf("Codigo: %i\n", produtos[i].codigo);
                    printf("Item: %s\n", produtos[i].nome);
                    printf("Valor Unitario: %.2f\n", produtos[i].valorUnitario);
                    printf("Estoque: %i\n\n", produtos[i].quantidade);

                    do{
                        printf("Digite o novo VALOR UNITARIO: ");
                        scanf("%f", &novoValor);
                        getchar();

                        if(novoValor <= 0){
                            printf("\nValor invalido, tente novamente.\n");
                        }
                    }while(novoValor <= 0);

                    do{
                        printf("Digite o novo ESTOQUE: ");
                        scanf("%i", &novoQuant);
                        getchar();

                        if(novoQuant <= 0){
                            printf("\nValor invalido, tente novamente.\n");
                        }
                    }while(novoQuant <= 0);

                    produtos[i].valorUnitario = novoValor;
                    produtos[i].quantidade = novoQuant;

                    printf("\nProduto atualizado com sucesso!\n");
                    printf("Codigo: %i\n", produtos[i].codigo);
                    printf("Item: %s\n", produtos[i].nome);
                    printf("Valor Unitario: %.2f\n", produtos[i].valorUnitario);
                    printf("Estoque: %i\n", produtos[i].quantidade);

                    menuProdutos();
                    break;
                }
            }
            if(!encontrado){
                printf("Produto nao encontrado, tente novamente.\n");
            }
        }while(!encontrado);
    }
    menuProdutos();
}

void excluirProduto(){

    int i;
    int codigo, encontrado, confirmar, prtExcluir;

    if(produtos == NULL || indiceG == 0){
        printf("\nNenhum produto cadastrado.\n");
        menuProdutos();
    }else{
        printf("\nExcluir/deletar produto\n");

        listarProdutos();
        do{
            printf("\nDigite o codigo do produto p/ excluir: ");
            scanf("%i", &codigo);
            getchar();

            encontrado = 0;
            for(i=0;i<indiceG;i++){
                if(codigo == produtos[i].codigo){
                    encontrado = 1;
                    prtExcluir = i;
                    printf("\nProduto encontrado:\n");
                    printf("Codigo: %i\n", produtos[i].codigo);
                    printf("Item: %s\n", produtos[i].nome);
                    printf("Valor Unitario: %.2f\n", produtos[i].valorUnitario);
                    printf("Estoque: %i\n\n", produtos[i].quantidade);

                    printf("Confirmar exclusao?\n");
                    printf("  S - Sim | Qualquer tecla - Nao\n");

                    printf("\nDigite a opcao escolhida: ");
                    scanf("%c", &confirmar);
                    getchar();

                    if(confirmar == 's' || confirmar == 'S'){
                        for(i=prtExcluir;i<indiceG-1;i++){
                            produtos[i] = produtos[i+1];
                        }

                        produtos = (Produto *)realloc(produtos,(indiceG-1)*sizeof(Produto));
                        indiceG--;

                        printf("\nProduto excluido com sucesso!\n");
                        menuProdutos();
                    }else{
                        menuProdutos();
                        break;
                    }

                    menuProdutos();
                    break;
                }
            }
            if(!encontrado){
                printf("Produto nao encontrado, tente novamente.\n");
            }
        }while(!encontrado);
    }
}

void salvarProdutos(){

    int i;

    if(produtos == NULL || indiceG == 0){
        printf("\nNenhum produto cadastrado.\n");
        menuProdutos();
    }else{
        FILE *arqProdutos;

        arqProdutos = fopen("produtos.txt", "w");

        if(arqProdutos == NULL){
            printf("\nErro ao abrir o arquivo.\n");
            menuProdutos();
        }

        for(i=0;i<indiceG;i++){
            fprintf(arqProdutos, "%d\n%s\n%.2f\n%d\n", produtos[i].codigo, produtos[i].nome, produtos[i].valorUnitario, produtos[i].quantidade);
        }
        fclose(arqProdutos);

        printf("\nProdutos salvos com sucesso!\n");
        menuProdutos();
    }
}

void lerProdutos() {

    int qprtLidos;
    Produto prtLidos;

    FILE *arqProdutos;
    arqProdutos = fopen("produtos.txt", "r");

    if (arqProdutos == NULL) {
        printf("\nErro ao abrir o arquivo.\n");
        menuProdutos();
    }

    printf("\nProdutos lidos do arquivo:\n\n");
    while (fscanf(arqProdutos, "%d %25s %f %d", &prtLidos.codigo, prtLidos.nome, &prtLidos.valorUnitario, &prtLidos.quantidade) == 4) {
        printf("Codigo: %d\nNome: %-25s\nValor: %.2f\nQuantidade: %d\n--\n", prtLidos.codigo, prtLidos.nome, prtLidos.valorUnitario, prtLidos.quantidade);

        produtos = (Produto *)realloc(produtos, (indiceG + 1) * sizeof(Produto));
        produtos[indiceG++] = prtLidos;

        qprtLidos++;
    }

    fclose(arqProdutos);

    printf("\nForam lidos %i produtos.\n", qprtLidos);

    menuProdutos();
}

void menuVendas(){

    int op;

    do{
        printf("\nMENU DE VENDAS\n\n");

        printf("  1 - Realizar venda\n");
        printf("  2 - Relatorio de vendas\n");
        printf("  3 - Voltar\n");

        printf("\nDigite a opcao escolhida: ");
        scanf("%i", &op);
        getchar();

        switch(op){
            case 1:
                realizarVenda();
                break;
            case 2:
                printf("Relatorio de vendas\n");
                break;
            case 3:
                menuPrincipal();
                break;
        }

        if(op < 1 || op > 3){
            printf("\nOpcao invalida, tente novamente.\n");
        }
    }while(op < 1 || op > 3);
}

void realizarVenda(){

    int i;
    int codigo, quantidade;
    int encontrado = 0, total = 0;
    float subtot = 0;
    char continuar;

    if(produtos == NULL || indiceG == 0){
        printf("\nNenhum produto em estoque para venda.\n");
        menuVendas();
    }else{
        printf("\n__________________________\n");
        printf("Relizar venda\n");

        listarProdutos();
        do{
            printf("\nDigite o codigo do item p/ venda: ");
            scanf("%i", &codigo);
            getchar();

            encontrado = 0;
            for(i=0;i<indiceG;i++){
                if(codigo == produtos[i].codigo){
                    encontrado = 1;
                    break;
                }
            }

            if(encontrado == 1){
                do{
                    if(produtos[i].quantidade <= 0){
                        printf("Produto fora de estoque, tente novamente.\n");
                        realizarVenda();
                    }else{
                        printf("\nItem escolhido: %s\n", produtos[i].nome);

                        do{
                            printf("Digite a quantidade p/ venda: ");
                            scanf("%i", &quantidade);
                            getchar();

                            if(quantidade <= 0){
                                printf("\nValor invalido, tente novamente.\n");
                                realizarVenda();
                            }else{
                                if(produtos[i].quantidade - quantidade < 0){
                                    printf("\nEstoque insuficiente, tente novamente.\n");
                                    realizarVenda();
                                }else{
                                    subtot = subtotal(codigo, quantidade);
                                    total = total + subtot; // acrescimo do valor total da compra toda vez que fecha um subtotal
                                    //totalg = total; // passando valor total da compra para variavel global 'totalg'

                                    printf("\nO subtotal e: %.2f\n", subtot);

                                    printf("\nDeseja realizar outra venda?\n");
                                    printf(" S - Sim | Qualquer tecla - Nao\n");

                                    printf("\nDigite a opcao escolhida: ");
                                    scanf("%c", &continuar);
                                    getchar();

                                    if(continuar == 's' || continuar == 'S'){
                                        realizarVenda();
                                    }else{
                                        cupomFiscal();
                                    }
                                }
                            }
                        }while(quantidade <= 0);
                    }
                }while(produtos[i].quantidade < 0);
            }else{
                printf("Codigo nao encontrado, tente novamente.\n");
            }
        }while(encontrado != 1);
    }
}

float subtotal(int codigo, int quantidade){

    int i;
    float subtot;

    for(i=0;i<indiceG;i++){
        if(codigo == produtos[i].codigo){
            subtot = produtos[i].valorUnitario * quantidade;

            //passando os dados da struct de produtos para a struct do cupom
            //vendidos[indiceG2].codigo = codigo;
            //strcpy(vendidos[indiceG2].nome, estoque[i].nome);
            //vendidos[indiceG2].valor = estoque[i].valor;
            //vendidos[indiceG2].subtotal = subtot;
            //vendidos[indiceG2].quantidade = quantidade;

            produtos[i].quantidade = produtos[i].quantidade - quantidade; //subtraindo quantidade comprada do estoque no pós-venda
            //indiceG2++; //aumenta o tamanho do vetor para o cupom fiscal
        }
    }
    return subtot;
}

void cupomFiscal(){

    int i;

    printf("\n CUPOM FISCAL \n");
}
