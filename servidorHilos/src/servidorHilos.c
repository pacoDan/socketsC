/*
 ============================================================================
 Name        : serverHilos.c
 Author      : DennisRitchie
 Version     :
 Copyright   : UTN.SO
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "socket.h"
#include "socketServer.h"
#include <pthread.h>
#include <sys/ioctl.h>

int nClientes = 0;

int isclosed(int sock) {
  fd_set rfd;
  FD_ZERO(&rfd);
  FD_SET(sock, &rfd);
  struct timeval tv = { 0 };
  select(sock+1, &rfd, 0, 0, &tv);
  if (!FD_ISSET(sock, &rfd))
    return 0;
  int n = 0;
  ioctl(sock, FIONREAD, &n);
  return n == 0;
}

void nueva_conexion (void *parametro) {
    int idCliente = ++nClientes;
    int buffer;
    int *sock = (int *) parametro;

    printf("Conectado cliente: %d\n", idCliente);

    Escribe_Socket (*sock, (char *) &idCliente, sizeof(int));

    while (!isclosed(*sock)) {

		if ((Lee_Socket (*sock, (char *)&buffer, sizeof(int)) > 0))
			printf ("Cliente %d envía %d\n", idCliente, buffer);
		else
		{
			printf ("Cliente %d ha cerrado la conexión\n", idCliente);
			nClientes--;
			break;
		}


    }
    close(*sock);
}

int main(void) {
	pthread_t idHilo;

	printf("Servidor con Hilos\n");

	Socket socket = crear_socket("127.0.0.1","8080");

	//Asocio el servidor a un puerto
	asociar_puerto(socket);

	//Escucho Conexiones Entrantes
	escuchar(socket);

	/*Por cada una de las conexiones que sean aceptadas, se lanza
	un Hilo encargado de atender la conexión*/
	while(1)
	{

		int socketCliente = Acepta_Conexion_Cliente(socket.socket);
		pthread_create (&idHilo, NULL, (void*)nueva_conexion, &socketCliente);
	}
}


