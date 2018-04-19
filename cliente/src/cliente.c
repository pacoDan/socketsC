/*
 * cliente.c
 *
 *  Created on: 15 abr. 2018
 *      Author: utnso
 */

#include <socket/socket.h>

//Ejemplo de un Cliente que se conecta al Servidor: 127.0.0.1, Puerto: 8080
int main(void)
{
	//En buffer guardare mi numero de cliente
	int buffer;

	printf("Cliente\n");

	//Creo un Socket para conectarme al Servidor
	Socket socket = crear_socket("127.0.0.1","8080");

	//Me Conecto al Servidor con la estructura de socket que tuve anteriormente
	conectar(socket);

	//Leo un Mensaje del Servidor (Obtendre mi numero de cliente)
	Lee_Socket (socket.socket, (char *)&buffer, sizeof(int));

	/* Se muestra en pantalla el número de cliente que nos ha enviado el servidor */
	printf ("Soy cliente %d\n", buffer);

	//Envio un mensaje al Servidor con mi número de cliente
	Escribe_Socket (socket.socket, (char *)&buffer, sizeof(int));

	//Se queda "ejecutando"...
	while(1);
}

