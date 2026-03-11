#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>


#define MAX_INPUT 500
#define MAX_TOKENS 50
#define COMANDO_SALIDA "exit"
// Crear el bucle de la terminal
// Procesar la linea

//Creacion del handler
#include <signal.h>
#include <string.h>

void handler(int signo);

void instalar_handlers() {

    struct sigaction sa;

    /* handler para SIGCHLD */
    //memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGCHLD, &sa, NULL);

    /* ignorar SIGINT */
    //sa.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa, NULL);
}
void handler(int signo){
    // Caso de SIGCHLD
    switch(signo){
        case SIGCHLD:
            int status;
            pid_t pid;
            while ((pid = waitpid(-1, &status, WNOHANG)) > 0);
            break;
        case SIGINT:
            //no hace nada xd
            break;
    }
}

//Shell (todo el funcionamiento)
void minishell(){
    char input[MAX_INPUT];
    char* parametros[MAX_TOKENS];

    // Ciclo donde vive la terminal
    while (1){
        // (1) Imprime el prompt
        printf("minishell");
        fflush(stdout);

        // (2) leer linea usuario
        //Se recibe el texto introducido por el usuairo:
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        //printf("%s", input);
        // Tokenizar los comandos
        int i_parametros = 0;
        char* token = strtok(input, " ");
        
        //printf("Tamanio: %ld", strlen(token));
        
        //printf("\nCOmando: %s", comando);
        
        while (token != NULL){
            parametros[i_parametros++] = token;
            //printf("%s\n", parametros[i_parametros-1]);  DEBUG
            token = strtok(NULL, " ");
        }
        // Es importante el NULL, porque execvp espera un array con el último elemento en NULL
        parametros[i_parametros] = NULL;
        int tam_comando = strlen(parametros[0]);
        char comando[tam_comando+1];
        strcpy(comando, parametros[0]);

        //for(int i = 0; i<i_parametros; i++){
        //    printf("%s ", parametros[i]);
        //}
        //printf("\n");

        if(strcmp(comando, COMANDO_SALIDA)==0){
            printf("Regresa pronto a minishell!\nAdios :)\n");
            break;
        } else {
            // CREACIÓN DEL FORK
            pid_t nvo_pid = fork();
            pid_t pid = getpid();
        

            if (nvo_pid < 0){ // fork sin exito
                printf("Hubo un error en el fork\n");
                printf("Terminando sin exito\n");
            }else if (nvo_pid == 0){ // pid del hijo
                //printf("Soy el proceso hijo, %i. Mi PID es: %i \n", nvo_pid, pid);
                
                // SObreescribe la señal de SIG_IGN del padre de ignorarla
                struct sigaction sa;
                //SIG_DFL: es la acción definida de la señal
                sa.sa_handler = SIG_DFL;
                sigemptyset(&sa.sa_mask);
                sa.sa_flags = 0;
                
                sigaction(SIGINT, &sa, NULL);

                execvp(comando, parametros);
                perror("execvp");
                _exit(1);
            }else{ // pid del padre
                //printf("Soy el proceso padre, %i. Mi PID es: %i \n", nvo_pid, pid);
                
            }
        }
    }
}


int main(){
    //aqui se instalan manejadores de señales
    instalar_handlers();
    minishell();
    return 0;
}
