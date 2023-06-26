#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int codigo;
    char nome[25];
    float valorUnitario;
    int quantidade;

    int qtdVendida;
    float valorVendido;
}Produto;

Produto *produtos = NULL;
int indiceG = 0;

//MENUS
int menuPrincipal();
void menuProdutos();
void menuVendas();
void menuPagamento(float total);

//MENU PRODUTOS
void exibirProdutos();
void cadastrarProduto();
void atualizarProduto();
void excluirProduto();
void salvarProdutos();
void lerProdutos();

//MENU VENDAS
void relizarVendas();
float subtotal(int codigo, int quantidade);
void cupomFiscal(Produto *prtVendidos, int qPrtVendidos, float total);
void pagVista(float total);
void pagPrazo(float total);
void relatorioVendas();

//OUTRAS FUNÇÕES
void listarProdutos();
void primeiraLeitura();
void bbSort(Produto *prtVendidos, int qPrtVendidos);
void bbSortCodigo();
void calcularTroco(float valorFinal);
char sair();

int main(){

    primeiraLeitura();

    int op;
    char continuar = 's';

    printf("\nBem vindo a Padaria da Berenice! \n");

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
                continuar = sair();
                break;
        }
    }

    return 0;
}

void primeiraLeitura(){

    int duplicado = 0;
    Produto prtLidos;

    FILE *arqProdutos;
    arqProdutos = fopen("produtos.txt", "r");

    if (arqProdutos == NULL){
        printf("\nErro ao abrir o arquivo.\n");
    }

    while(fscanf(arqProdutos, "%d %25s %f %d %i %f", &prtLidos.codigo, prtLidos.nome, &prtLidos.valorUnitario, &prtLidos.quantidade, &prtLidos.qtdVendida, &prtLidos.valorVendido) == 6){
        produtos = (Produto *)realloc(produtos, (indiceG + 1) * sizeof(Produto));

        for(int i = 0; i < indiceG; i++){
            if(produtos[i].codigo == prtLidos.codigo){
                duplicado = 1;
                break;
            }
        }

        if(!duplicado){
            produtos = (Produto *)realloc(produtos, (indiceG + 1) * sizeof(Produto));
            produtos[indiceG++] = prtLidos;
        }
    }

    fclose(arqProdutos);
}

int menuPrincipal(){

    int op;

    printf("_________________________________\n");
    printf("\nMENU PRINCIPAL\n\n");

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
                main();
                break;
        }

        if(op < 1 || op > 7){
            printf("Opcao invalida, tente novamente.\n");
        }
    }while(op < 1 || op > 7);
}

void bbSortCodigo(){

    int i, j;

    for(i=0;i<indiceG-1;i++){
        for(j=0;j<indiceG-i-1;j++){
            if(produtos[j].codigo > produtos[j+1].codigo){
                Produto temp = produtos[j];
                produtos[j] = produtos[j+1];
                produtos[j+1] = temp;
            }
        }
    }
}

void listarProdutos(){

    int i;

    bbSortCodigo();

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

    novoProduto.qtdVendida = 0;
    novoProduto.valorVendido = 0;

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
            fprintf(arqProdutos, "%d\n%s\n%.2f\n%d\n%i\n%f\n",
            produtos[i].codigo, produtos[i].nome, produtos[i].valorUnitario, produtos[i].quantidade, produtos[i].qtdVendida, produtos[i].valorVendido);
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
    while(fscanf(arqProdutos, "%d %25s %f %d %i &f",
        &prtLidos.codigo, prtLidos.nome, &prtLidos.valorUnitario, &prtLidos.quantidade, &prtLidos.qtdVendida, &prtLidos.valorVendido) == 6){

        printf("Codigo: %d\nNome: %-25s\nValor: %.2f\nQuantidade: %d\n--\n",
        prtLidos.codigo, prtLidos.nome, prtLidos.valorUnitario, prtLidos.quantidade);

        produtos = (Produto *)realloc(produtos, (indiceG + 1) * sizeof(Produto));
        produtos[indiceG++] = prtLidos;

        qprtLidos++;
    }

    fclose(arqProdutos);

    printf("Foram lidos %i produtos.\n", qprtLidos);

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
                relatorioVendas();
                break;
            case 3:
                main();
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
    int encontrado = 0, qPrtVendidos = 0;
    float subtot = 0, total = 0;
    char continuar = 's';

    Produto *prtVendidos = NULL;

    if(produtos == NULL || indiceG == 0){
        printf("\nNenhum produto em estoque para venda.\n");
        menuVendas();
    }else{
        printf("\n__________________________\n");
        printf("Relizar venda\n");

        while(continuar == 's' || continuar == 'S'){
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
                                        total = total + subtot;

                                        qPrtVendidos++;
                                        prtVendidos = (Produto *)realloc(prtVendidos, (qPrtVendidos+1)*sizeof(Produto));

                                        prtVendidos[qPrtVendidos-1] = produtos[i];
                                        prtVendidos[qPrtVendidos-1].quantidade = quantidade;
                                        prtVendidos[qPrtVendidos-1].valorVendido = subtot;

                                        printf("\nO subtotal e: %.2f\n", subtot);

                                        printf("\nDeseja realizar outra venda?\n");
                                        printf(" S - Sim | Qualquer tecla - Nao\n");

                                        printf("\nDigite a opcao escolhida: ");
                                        scanf("%c", &continuar);
                                        getchar();
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
        cupomFiscal(prtVendidos, qPrtVendidos, total);
        menuPagamento(total);
    }
}

float subtotal(int codigo, int quantidade){

    int i;
    float subtot;

    for(i=0;i<indiceG;i++){
        if(codigo == produtos[i].codigo){
            subtot = produtos[i].valorUnitario * quantidade;
            produtos[i].quantidade = produtos[i].quantidade - quantidade;

            produtos[i].qtdVendida = produtos[i].qtdVendida + quantidade;
            produtos[i].valorVendido = produtos[i].valorVendido + subtot;
        }
    }
    return subtot;
}

void bbSort(Produto *prtVendidos, int qPrtVendidos){

    int i, j;
    Produto aux;

    for (i=0;i<qPrtVendidos-1;i++){
        for (j=0;j<qPrtVendidos-i-1;j++){
            if (prtVendidos[j].valorVendido < prtVendidos[j+1].valorVendido){
                // Troca os produtos de posição
                aux = prtVendidos[j];
                prtVendidos[j] = prtVendidos[j+1];
                prtVendidos[j+1] = aux;
            }
        }
    }
}

void cupomFiscal(Produto *prtVendidos, int qPrtVendidos, float total){

    int i;

    //bbSort(prtVendidos, qPrtVendidos);

    printf("\n CUPOM FISCAL: \n\n");

    printf(" Codigo | Item              | Valor Unitario | Quant. | Subtotal\n");
    for(i=0;i<qPrtVendidos;i++){
        printf("    %-3i | %-16s  |   R$ %-9.2f |   %-3i  |  R$ %5.2f\n",
        prtVendidos[i].codigo, prtVendidos[i].nome, prtVendidos[i].valorUnitario, prtVendidos[i].quantidade, prtVendidos[i].valorVendido);
    }
    printf("                                                   _________________\n");
    printf("                                                   Total: R$ %.2f\n", total);
}

void menuPagamento(float total){

    float totalCompra = total;
    int op;

    //tabela de descontos e acrescimos
    printf("_________________________________________________________\n");
    printf("Ate R$ 50,00                --> 5 por cento de desconto  |\n");
    printf("Entre R$ 50,01 e R$ 99,99   --> 10 por cento de desconto |\n");
    printf("Acima de R$ 100,00          --> 18 por cento de desconto |\n");
    printf("                                                         |\n");
    printf("Ate 03 parcelas             --> 5 por cento de desconto  |\n");
    printf("Acima de 03 parcelas        --> 8 por cento de acrescimo |\n");
    printf("_________________________________________________________|\n");

    //escolha da forma de pagamento
    printf("\nEscolha a forma de pagamento desejada:\n");
    printf(" 1 - Pagamento a vista\n");
    printf(" 2 - Pagamento a prazo\n\n");

    //loop para validação do codigo
    do{
        printf("Digite a opcao escolhida: ");
        scanf("%i", &op);
        getchar;

        if(op < 1 || op > 2){
            printf("Opcao invalida, tente novamente.\n\n");
        }else{
            //switch para definição da forma de pagamento
            switch(op){
                case 1:
                    pagVista(totalCompra); //chamando função de pagamento a vista
                    break;
                case 2:
                    pagPrazo(totalCompra); //chamando função de pagamento a vista
                    break;
            }
        }
    }while(op < 1 || op > 2);
}

void pagVista(float totalCompra){

    float tot = 0, valorFinal = 0, desconto = 0;

    printf("\nO valor total da compra foi de: R$ %.2f\n", totalCompra);

    //verificando valor da compra para aplicar os descontos cabiveis a cada
    if(totalCompra <= 50){
        printf("5 por cento de desconto aplicado.\n");
        desconto = totalCompra * 0.05;
        valorFinal = totalCompra - desconto;

        printf("\nO valor do desconto foi de: R$ %.2f\n", desconto);
        printf("O valor final da compra foi de: R$ %.2f\n\n", valorFinal);
    }else if(totalCompra > 50 && totalCompra < 100){
        printf("10 por cento de desconto aplicado.\n");
        desconto = totalCompra * 0.10;
        valorFinal = totalCompra - desconto;

        printf("\nO valor do desconto foi de: R$ %.2f\n", desconto);
        printf("O valor final da compra foi de: R$ %.2f\n\n", valorFinal);
    }else{
        printf("18 por cento de desconto aplicado.\n");
        desconto = totalCompra * 0.18;
        valorFinal = totalCompra - desconto;

        printf("\nO valor do desconto foi de: R$ %.2f\n", desconto);
        printf("O valor final da compra foi de: R$ %.2f\n\n", valorFinal);
    }

    calcularTroco(valorFinal); //chamando função para inserção e calculo do troco
    menuVendas();
}

void calcularTroco(float valorFinal){

    float pagamento = 0, troco = 0, vParcelas = 0;

    // loop de validação do troco (troco > 0)
    do{
        printf("Digite o valor recebido para o troco: R$ ");
        scanf("%f", &pagamento);
        getchar();

        if(pagamento <= 0){
            printf("Valor invalido, tente novamente.\n\n");
        }else{
            //verificação de o pagamento sera suficiente para cobrir a compra
            if(pagamento < valorFinal){
                troco = pagamento - valorFinal;
                troco = troco * -1;

                printf("\nFaltam R$ %.2f, digite um valor suficiente.\n", troco);
            //aplicação do troco
            }else{
                troco = pagamento - valorFinal;

                if(troco == 0){
                    printf("\nNao ha troco.\n");
                }else{
                    printf("\nO troco e: R$ %.2f\n", troco);
                }
            }
        }
    }while(pagamento <= 0 || pagamento < valorFinal);
}

void pagPrazo(float totalCompra){

    float acrescimo = 0, valorFinal = 0, vParcelas = 0;
    int parcelas;

    //loop de verificação de o numero de parcelas é valido
    do{
        printf("\nDigite a quantidade de parcelas: ");
        scanf("%i", &parcelas);
        getchar();

        printf("\nO valor total da compra foi de: R$ %.2f\n", totalCompra);

        if(parcelas < 1){
            printf("Valor invalido, tente novamente.\n");
        }else{
            //verificação do numero de parcelas para aplicação dos acrescimos cabiveis
            if(parcelas <= 3){
                printf("5 por cento de acrescimo aplicado.\n");
                acrescimo = totalCompra * 0.05;
                valorFinal = totalCompra + acrescimo;
                vParcelas = valorFinal / parcelas;

                printf("\nO valor do acrescimo foi de: R$ %.2f\n", acrescimo);
                printf("O valor final foi de: R$ %.2f\n\n", valorFinal);
                printf("O valor de cada parcela ficou: R$ %.2f\n", vParcelas);
            }else{
                printf("8 por cento de acrescimo aplicado.\n");
                acrescimo = totalCompra * 0.08;
                valorFinal = totalCompra + acrescimo;
                vParcelas = valorFinal / parcelas;

                printf("\nO valor do acrescimo foi de: R$ %.2f\n", acrescimo);
                printf("O valor final foi de: R$ %.2f\n\n", valorFinal);
                printf("O valor de cada parcela ficou: R$ %.2f\n", vParcelas);
            }
        }
    }while(parcelas < 1);

    menuVendas();
}

void relatorioVendas(){

    int i;
    float aux = 0, totalRelatorio;

    for(i=0;i<indiceG;i++){
        produtos[i].valorVendido = produtos[i].valorUnitario * produtos[i].qtdVendida;
        aux = aux + produtos[i].valorVendido;
    }

    totalRelatorio = aux;

    printf("_________________________________\n");
    printf("\nRelatorio de vendas:\n\n");

    printf(" Codigo | Item              | Valor Unitario | Estoque | Qtd. Vendida | Total Vendido\n");
    for(i=0;i<indiceG;i++){
        printf("    %-3i | %-16s  |   R$ %-9.2f |    %-3i  |      %-4i    |  R$ %5.2f\n",
        produtos[i].codigo, produtos[i].nome, produtos[i].valorUnitario, produtos[i].quantidade, produtos[i].qtdVendida, produtos[i].valorVendido);
    }
    printf("                                                                      ________________\n");
    printf("                                                                 Total:  R$ %.2f\n", totalRelatorio);
}

char sair(){

    char n = 'n';

    printf("Saindo...\n"); //saindo...

    //retorno para a main
    return n;
}
