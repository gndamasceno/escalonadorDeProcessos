# Implementação com Pthreads
<p>
Suporte de execução implementando o modelo n x m de threads para uma linguagem concorrente baseada na criação de tarefas. Este suporte deve ser implementado na forma de uma biblioteca, utilizando C e a biblioteca de threads Pthreads.
</p>
<p>
Neste modelo, n tarefas são executadas sobre m processadores virtuais (pvs). As tarefas são as atividades concorrentes criadas pelo programa (com a diretiva spawn). Os pvs são threads responsáveis por executar as tarefas geradas pelo programa. Os pvs compartilham, entre si, duas listas de tarefas: uma de tarefas prontas para executar e outra de tarefas finalizadas. As tarefas devem ser descritas, internamente, por uma estrutura de dados que possua, pelo menos, o ponteiro da função que executará o código da tarefa e dois ponteiros void* para áreas de memória contendo os parâmetros de entrada e o retorno da função.
</p>

## Limitações do projeto
<p>O projeto não executa da maneira correta quando o número de tarefas a ser executadas pelo programa é maior que a quantidade de processadores virtuais disponíveis, acreditamos que esse problema seja causado porque existem erros no uso das variáveis de condição do programa.
</p>

## Extras
<p> O projeto também tem um programa para calcular o fatorial utilizando nossa biblioteca.
</p>

## Executando o projeto
<p>Para executar esse projeto você precisa de ferramentas básicas utilizadas no desenvolvimento em C/C++, são essenciais nesse projeto: g++ e make. O git também é necessário.
</p>

```bash

#### Preparando o ambiente

# Primeiramente, utilize as ferramentas de gerenciamento de pacotes apt para atualizar seu índice de pacotes local.
$ sudo apt update

# Para instalar as ferramentas básicas de desenvolvimento em C/C++, incluindo c++ e make
$ sudo apt install build-essential

# Para instalar o git
$ sudo apt install git

#Você pode confirmar se instalou o Git e as ferramentas essencias do projeto corretamente executando os seguintes comandos e verificando se recebe um resultado relevante.
$ git --version
$ make --version
$ g++ --version

#### Compilando e executando o projeto

# Clone este repositório
$ git clone https://github.com/gndamasceno/implementacaoPthreads.git

# Entre no diretório "desafioTakeBlip"
$ cd implementacaoPthreads

# Para compilar o fibonacci 
$ make all

# Você também pode compilar o fibonacci escrevendo 
$ make fibo

# Para executar o fibonacci passe como parâmetro um número maior que zero para calcular, por exemplo 9
$ ./fibo.o 9

# Para compilar o fatorial
$ make fatorial

# Para executar o fatorial passe como parâmetro um número maior que zero para calcular, por exemplo 9
$ ./fatorial.o 9

# Para Remover excluir todos os arquivos com extensão .o e os de backup *~ 
$ make clean
```