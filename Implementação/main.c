#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//255 caracteres por linha
#define max 255

//funções de back-up
void *car();
void *num();
void *esp();
void *linhas();

//possíveis erros com a manipulação de arquivos e threads
void erroAbrir(){
  printf("Erro ao abrir o arquivo\n");
}
void erroGrav(){
  printf("Erro de Gravação\n");
}
void erroT(){
  printf("Erro ao criar thread\n");
}

//tratamento do back-up pós edição do arquivo
void *choose(){
	//abrir arquivo como leitura
  FILE *arq;
  arq = fopen("arquivo.txt","r");
  if(arq == NULL){
    erroAbrir();
    return 0;
  }
	//Declarando array de threads
  pthread_t threads[4];
	//Declarando contador de linhas
  int cont = 0;
	//definição das strings de linha completa, de letras, de números e de caracteres especiais
  char str[max];
  char stcar[max],stnum[max],stesp[max];;
	//deleta os back-ups anteriores
  remove("car.txt");
  remove("num.txt");
  remove("esp.txt");
  remove("lin.txt");
	//leitura do arquivo linha por linha
  while(fgets(str,max,arq)!=NULL){
    //Limpa as Strings
    memset(stcar,0,max);
    memset(stnum,0,max);
    memset(stesp,0,max);
		//acresce o número de linhas
    cont++;
		//vendo caracter por caracter da linha
    for(int i=0; i<strlen(str)-1;i++){
			//se o caracter for uma letra, ele é adicionado em stcar
      if(isalpha(str[i])){
        stcar[strlen(stcar)] = str[i];
      }
			//se o caracter for um número, ele é adicionado em stnum
      else if(isdigit(str[i])){
        stnum[strlen(stnum)] = str[i];
			//se o caracter for um caracter especial, ele é adicionado em stesp
      }else if(ispunct(str[i])){
        stesp[strlen(stesp)] = str[i];
			//verifica se o caracter é um espaço
      }else if(str[i]== ' '){
        continue;
			//acusa se o caracter não for nenhum dos casos acima (Letras acentuadas)
      }else{
        printf("Não existe tratamento para : %c\n",str[i]);
      }
    }
		//Criando threads e salvando os dados de stcar,stnum e stesp em seus respectivos arquivos
    if(strlen(stcar)!=0){
      if(pthread_create(&threads[0], NULL, car, &stcar)!=0){
        erroT();
        return 0;
      }
    }
    if(strlen(stnum)!=0){
      if(pthread_create(&threads[1], NULL, num, &stnum)!=0){
        erroT();
        return 0;
      }
    }
    if(strlen(stesp)!=0){
      if(pthread_create(&threads[2], NULL, esp, &stesp)!=0){
        erroT();
        return 0;
      }
    }
		//Aguardando finalização das threads de back-up
    for(int j=0;j<3;j++){
      pthread_join(threads[j], NULL);
    } 
  }
	//Criando e aguardando thread de salvamento da quantidade de linhas
  if(pthread_create(&threads[3], NULL, linhas, &cont)!=0){
    erroT();
    return 0;
  }
  pthread_join(threads[3], NULL);
  fclose(arq);
}

//abre e realiza o back-up de letras
void *car(void *arg){
  FILE *car;
  car = fopen("car.txt","a+");
  if(car==NULL){
    erroAbrir();
    return 0;
  }
  if(fprintf(car,"%s\n",(char *)arg) == EOF){
    erroGrav();
    return 0;
  }
  fclose(car);
}

//abre e realiza o back-up de números
void *num(void *arg){
  FILE *num;
  num = fopen("num.txt","a+");
  if(num==NULL){
    erroAbrir();
    return 0;
  }
  if(fprintf(num,"%s\n",(char *)arg) == EOF){
    erroGrav();
    return 0;
  }
  fclose(num);
}

//abre e realiza o back-up de caracteres especiais
void *esp(void *arg){
  FILE *esp;
  esp = fopen("esp.txt","a+");
  if(esp==NULL){
    erroAbrir();
    return 0;
  }
  if(fprintf(esp,"%s\n",(char *)arg) == EOF){
    erroGrav();
    return 0;
  }
  fclose(esp);
}

//abre e realiza o back-up do contador de linhas
void *linhas(void *arg){
  FILE *lin;
  lin = fopen("lin.txt","a+");
  if(lin==NULL){
    erroAbrir();
    return 0;
  }
  if(fprintf(lin,"%d\n",*(int*)arg) == EOF){
    erroGrav();
    return 0;
  }
  fclose(lin); 
}

//Main: iniciando a thread de separação de dados(Choose)
int main() {
  pthread_t thread;
  if(pthread_create(&thread, NULL, choose, NULL)!=0){
    erroT();
    return 0;
  }
  pthread_join(thread, NULL);
  return 0;
}