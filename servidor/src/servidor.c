/*
 ============================================================================
 Name        : servidor.c
 Author      : DennisRitchie
 Version     :
 Copyright   : UTN.SO
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <socket/socket.h>
#include <socket/socketServer.h>

/*
 * Programa principal.
 * Ejemplo de un Servidor con Multiplexación Ip: 127.0.0.1, Puerto: 8080
 * Crea un socket servidor y se mete en un select() a la espera de clientes.
 * Cuando un cliente se conecta, le atiende y lo añade al select() y se
 * queda constamente en un while infinito para actuar como server
 */

int main(void) {
	int socketServidor;				/* Descriptor del socket servidor */
	int socketCliente[MAX_CLIENTES];/* Descriptores de sockets con clientes */
	int numeroClientes = 0;			/* Número clientes conectados */
	fd_set descriptoresLectura;	/* Descriptores de interes para select() */
	int buffer;							/* Buffer para leer de los socket */
	int maximo;							/* Número de descriptor más grande */
	int i;

	printf("Servidor\n");

	//Creo un Socket para actuar como Servidor
	Socket socket = crear_socket("127.0.0.1","8080");
	socketServidor = socket.socket;
	//Asocio el servidor a un puerto
	asociar_puerto(socket);
	//Escucho Conexiones Entrantes
	escuchar(socket);

	while (1)
		{
			/* Cuando un cliente cierre la conexión, se pondrá un -1 en su descriptor
			 * de socket dentro del array socketCliente. La función compactaClaves()
			 * eliminará dichos -1 de la tabla, haciéndola más pequeña.
			 *
			 * Se eliminan todos los clientes que hayan cerrado la conexión */
			compactaClaves (socketCliente, &numeroClientes);

			/* Se inicializa descriptoresLectura */
			FD_ZERO (&descriptoresLectura);

			/* Se añade para select() el socket servidor */
			FD_SET (socketServidor, &descriptoresLectura);

			/* Se añaden para select() los sockets con los clientes ya conectados */
			for (i=0; i<numeroClientes; i++)
				FD_SET (socketCliente[i], &descriptoresLectura);

			/* Se el valor del descriptor más grande. Si no hay ningún cliente,
			 * devolverá 0 */
			maximo = dameMaximo (socketCliente, numeroClientes);

			if (maximo < socketServidor)
				maximo = socketServidor;

			/* Espera indefinida hasta que alguno de los descriptores tenga algo
			 * que decir: un nuevo cliente o un cliente ya conectado que envía un
			 * mensaje */
			select (maximo + 1, &descriptoresLectura, NULL, NULL, NULL);

			/* Se comprueba si algún cliente ya conectado ha enviado algo */
			for (i=0; i<numeroClientes; i++)
			{
				if (FD_ISSET (socketCliente[i], &descriptoresLectura))
				{
					/* Se lee lo enviado por el cliente y se escribe en pantalla */
					if ((Lee_Socket (socketCliente[i], (char *)&buffer, sizeof(int)) > 0))
						printf ("Cliente %d envía %d\n", i+1, buffer);
					else
					{
						/* Se indica que el cliente ha cerrado la conexión y se
						 * marca con -1 el descriptor para que compactaClaves() lo
						 * elimine */
						printf ("Cliente %d ha cerrado la conexión\n", i+1);
						socketCliente[i] = -1;
					}
				}
			}

			/* Se comprueba si algún cliente nuevo desea conectarse y se le
			 * admite */
			if (FD_ISSET (socketServidor, &descriptoresLectura))
				nuevoCliente (socketServidor, socketCliente, &numeroClientes);
		}
}
