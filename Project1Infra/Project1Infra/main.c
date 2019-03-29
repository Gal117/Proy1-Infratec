// ISIS-1304 - FUNDAMENTOS DE INFRAESTRUCTURA TECNOLOGICA - PROYECTO 1 - 201910
// EL GRUPO DEBE SER DESARROLLADO EN GRUPOS DE A 3 PERSONAS MAXIMO
//
// DESARROLLADO POR:
// Daniel Galindo - 201813517
// Santiago Estupi��n - 201813414
// Jaime Torres - 201815481

#define _CRT_SECURE_NO_DEPRECATE 
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

// La representacion de la imagen
typedef struct img
{
	int ancho;
	int alto;
	unsigned char *informacion;
} Imagen;


// Funci�n que carga el bmp en la estructura Imagen
void cargarBMP24(Imagen * imagen, char * nomArchivoEntrada);

// Funci�n que guarda el contenido de la estructura imagen en un archivo binario
void guardarBMP24(Imagen * imagen, char * nomArchivoSalida);

//Funci�n que inserta un mensaje en la imagen usando n bits por Byte
void insertarMensaje(Imagen * img, unsigned char mensaje[], int n);

//Funci�n que lee un mensaje de una imagen dando la longitud del mensaje y el numero de bits por byte usados
void leerMensaje(Imagen * img, unsigned char msg[], int l, int n);

//Funci�n que saca n bits de una secuencia de caracteres a partir de una posici�n dada
unsigned char sacarNbits(unsigned char secuencia[], int bitpos, int n);

// Programa principal
// NO MODIFICAR
int main(int argc, char* argv[]) {

	Imagen *img = (Imagen *)malloc(sizeof(Imagen));
	char msg[10000];
	int op, num, l, i, n;
	char nomArch[256];

	printf("ISIS-1304 - FUNDAMENTOS DE INFRAESTRUCTURA TECNOLOGICA - PROYECTO 1\n");
	printf("Indique la accion a realizar:\n\t1) Insertar mensaje\n\t2) Leer mensaje\n\n");
	printf("Opcion: ");
	scanf("%d", &op);

	printf("Ingrese el nombre/ruta de la imagen (incluya el formato .bmp): ");

	char temp;
	scanf("%c", &temp);
	gets(nomArch);

	// Cargar los datos
	cargarBMP24(img, nomArch);

	if (op == 1) {
		printf("Ingrese el mensaje a insertar:\n\n");

		gets(msg);

		printf("\nLongitud mensaje: %d bytes\n", strlen(msg));
		msg[strlen(msg)] = '\0';

		printf("Ingrese el numero de bits por Byte: ");
		scanf("%d", &num);

		printf("Insertando . . .\n");
		insertarMensaje(img, msg, num);
		printf("Insertado correctamente !\n");

		char nuevo[256];
		printf("Ingrese el nombre del archivo a guardar (incluya el formato .bmp): ");

		scanf("%c", &temp);
		gets(nuevo);

		guardarBMP24(img, nuevo);

		printf("Mensaje insertado y guardado exitosamente en el archivo/ruta '%s' ! :D\n\n", nuevo);
	}
	else if (op == 2) {
		printf("Ingrese la longitud a leer del mensaje insertado: ");

		scanf("%d", &l);

		printf("Ingrese el numero de bits por Byte: ");
		scanf("%d", &n);

		for (i = 0; i < l; i++) {
			msg[i] = 0;
		}

		leerMensaje(img, msg, l, n);

		msg[l] = 0;

		printf("Mensaje obtenido exitosamente ! El mensaje es:\n\n%s\n\n", msg);
	}
	else {
		printf("La opcion ingresada (%d) no es correcta\n", op);
		system("pause");
		return -1;
	}
	system("pause");
	return 0;
}

/**
* Inserta un mensaje, de a n bits por componente de color, en la imagen apuntada por img
* par�metro img: Apuntador a una imagen en cuyos pixeles se almacenar� el mensaje.
* par�metro mensaje: Apuntador a una cadena de caracteres con el mensaje.
* par�metro n: Cantidad de bits del mensaje que se almacenar�n en cada componente de color de cada pixel. 0 < n <= 8.
*/
void insertarMensaje(Imagen * img, unsigned char mensaje[], int n) {

	int ancho = img->ancho;
	int alto = img->alto;
	int numBytesAncho = ancho * 3;
	int numBytesAlto = alto * 3;
	int desp = 8 - n;
	int avance = 0;
	int bytesCount = 0;
	int groupsCount = 0;

	for (int y = 0; y < numBytesAlto; y++) {
		for (int x = 0; x < numBytesAncho; x++) {
			int pos = y * ancho + x;
			if ((n*groupsCount) % 8 == 0) {
				img->informacion[pos] >> desp;
				img->informacion[pos] << desp;
				unsigned char temp = sacarNbits(mensaje, bytesCount, n);
				img->informacion[pos] = img->informacion[pos] + temp;
				groupsCount++;

			}
			else if (bytesCount * 8 < groupsCount*n && (bytesCount + 1) * 8 > (groupsCount + 1)*n) {

				img->informacion[pos] >> desp;
				img->informacion[pos] << desp;
				int  izq = (groupsCount*n) - (bytesCount * 8);
				unsigned char temp = mensaje[bytesCount] << izq;
				temp = temp >> ((bytesCount + 1) * 8 > -(groupsCount + 1)*n) + izq;
				img->informacion[pos] = img->informacion[pos] + mensaje[bytesCount];
				groupsCount++;


			}
			else if (bytesCount * 8 < groupsCount*n && (bytesCount + 1) * 8 < (groupsCount + 1)*n) {

				img->informacion[pos] >> desp;
				img->informacion[pos] << desp;
				int  izq = ((bytesCount*n) - (groupsCount * 8));
				unsigned char temp = mensaje[bytesCount] << 8 - izq;
				temp = temp >> n - izq;
				unsigned char temp2 = mensaje[bytesCount++] >> 8 - (n - izq);
				unsigned char res = temp + temp2;
				img->informacion[pos] = img->informacion[pos] + res;
				groupsCount++;
				bytesCount++;


			}


		}
		// TODO: Desarrollar OBLIGATORIAMENTE en su totalidad.
	}
}
/**
* Extrae un mensaje de tamanio l, guardado de a n bits por componente de color, de la imagen apuntada por img
* par�metro img: Apuntador a una imagen que tiene almacenado el mensaje en sus pixeles.
* par�metro msg: Apuntador a una cadena de caracteres donde se depositar� el mensaje.
* par�metro l: Tamanio en bytes del mensaje almacenado en la imagen.
* par�metro n: Cantidad de bits del mensaje que se almacenan en cada componente de color de cada pixel. 0 < n <= 8.
*/
void leerMensaje(Imagen * img, unsigned char msg[], int l, int n) {

	int ancho = img->ancho;
	int alto = img->alto;
	int numBytesAncho = ancho * 3;
	int numBytesAlto = alto * 3;
	int contBytes = 0;
	int count = 0;
	for (int y = 0; y < numBytesAlto && contBytes < l; y++) {
		for (int x = 0; x < numBytesAncho && contBytes < l; x++) {
			int pos = y * ancho + x;

		}

		// TODO: Desarrollar OBLIGATORIAMENTE en su totalidad.
	}
}
/**
* Extrae n bits a partir del bit que se encuentra en la posici�n bitpos en la secuencia de bytes que
* se pasan como par�metro
* par�metro secuencia: Apuntador a una secuencia de bytes.
* par�metro n: Cantidad de bits que se desea extraer. 0 < n <= 8.
* par�metro bitpos: Posici�n del bit desde donde se extraer�n los bits. 0 <= n < 8*longitud de la secuencia
* retorno: Los n bits solicitados almacenados en los bits menos significativos de un unsigned char
*/
unsigned char sacarNbits(unsigned char secuencia[], int bitpos, int n) {

	unsigned char res; //esta es la variable de respuesta
	int byte = bitpos / 8; // con esto se sabe en que byte de la secuencia[] est� n
	unsigned char pre = secuencia[byte]; //Variable con el char de donde se quieren sacar los bits
	int desp = 8 - n; // valor de cu�ntos desplazamientos en el char hay que hacer para obtener los n bits
	if ((n < 8) && ((bitpos % 8) != 0)) { //Caso en que se quieren agrupar menos de 8 bits del byte y adem�s la posici�n del bit deseado no es el bit m�s significativo de cualquiera de los bytes de la secuencia par�mtero.
		unsigned char p = pre << bitpos; // Primero se desplaza a la izquierda para eliminar los dem�s bits significativos
		unsigned char q = p >> desp; // Luego se desplaza a la derecha para dejar los n bits a la derecha y que queden en la posici�n menos significativa del unsigned char
		res = q;	//Se asigna el valor modificado a la respuesta de retorno
	}
	else if ((n < 8) && ((bitpos % 8) == 0)) { //Caso en que se quieren sacar menos que 8 bits del byte y la posici�n deseada es el bit m�s significativo de cualquier byte de la secuencia par�metro. 
		unsigned char t = pre >> desp;	//Se desplaza lo necesario para dejar los n bits hacia la izquierda.
		res = t;	//Se asigna el valor modificado a la respuesta de retorno
	}
	else if (n == 8) { //Caso en que se quiera todo el byte de cualquier poisici�n de la secuencia par�metro
		res = pre;	//Se asigna el valor modificado a la respuesta de retorno

	}
	//Guardar el char con los n bits en el char de respuesta

	return res;
	// DESARROLLO OPCIONAL: Puede ser �til para el desarrollo de los procedimientos obligatorios.
}

// Lee un archivo en formato BMP y lo almacena en la estructura img
// NO MODIFICAR
void cargarBMP24(Imagen * imagen, char * nomArchivoEntrada) {
	// bmpDataOffset almacena la posici�n inicial de los datos de la imagen. Las otras almacenan el alto y el ancho
	// en pixeles respectivamente
	int bmpDataOffset, bmpHeight, bmpWidth;
	int y;
	int x;
	int	residuo;

	FILE *bitmapFile;
	bitmapFile = fopen(nomArchivoEntrada, "rb");
	if (bitmapFile == NULL) {
		printf("No ha sido posible cargar el archivo: %s\n", nomArchivoEntrada);
		exit(-1);
	}

	fseek(bitmapFile, 10, SEEK_SET); // 10 es la posici�n del campo "Bitmap Data Offset" del bmp	
	fread(&bmpDataOffset, sizeof(int), 1, bitmapFile);

	fseek(bitmapFile, 18, SEEK_SET); // 18 es la posici�n del campo "height" del bmp
	fread(&bmpWidth, sizeof(int), 1, bitmapFile);
	bmpWidth = bmpWidth * 3;

	fseek(bitmapFile, 22, SEEK_SET); // 22 es la posici�n del campo "width" del bmp
	fread(&bmpHeight, sizeof(int), 1, bitmapFile);

	residuo = (4 - (bmpWidth) % 4) & 3; // Se debe calcular los bits residuales del bmp, que surjen al almacenar en palabras de 32 bits

	imagen->ancho = bmpWidth;
	imagen->alto = bmpHeight;
	imagen->informacion = (unsigned char *)calloc(bmpWidth * bmpHeight, sizeof(unsigned char));

	fseek(bitmapFile, bmpDataOffset, SEEK_SET); // Se ubica el puntero del archivo al comienzo de los datos

	for (y = 0; y < bmpHeight; y++) {
		for (x = 0; x < bmpWidth; x++) {
			int pos = y * bmpWidth + x;
			fread(&imagen->informacion[pos], sizeof(unsigned char), 1, bitmapFile);
		}
		fseek(bitmapFile, residuo, SEEK_CUR); // Se omite el residuo en los datos
	}
	fclose(bitmapFile);
}

// Esta funci�n se encarga de guardar una estructura de Imagen con formato de 24 bits (formato destino) en un archivo binario
// con formato BMP de Windows.
// NO MODIFICAR
void guardarBMP24(Imagen * imagen, char * nomArchivoSalida) {
	unsigned char bfType[2];
	unsigned int bfSize, bfReserved, bfOffBits, biSize, biWidth, biHeight, biCompression, biSizeImage, biXPelsPerMeter, biYPelsPerMeter, biClrUsed, biClrImportant;
	unsigned short biPlanes, biBitCount;
	FILE * archivoSalida;
	int y, x;
	int relleno = 0;

	int residuo = (4 - (imagen->ancho) % 4) & 3; // Se debe calcular los bits residuales del bmp, que quedan al forzar en palabras de 32 bits


	bfType[2];       // Tipo de Bitmap
	bfType[0] = 'B';
	bfType[1] = 'M';
	bfSize = 54 + imagen->alto * ((imagen->ancho) / 3) * sizeof(unsigned char);       // Tamanio total del archivo en bytes
	bfReserved = 0;   // Reservado para uso no especificados
	bfOffBits = 54;    // Tamanio total del encabezado
	biSize = 40;      // Tamanio del encabezado de informacion del bitmap	
	biWidth = (imagen->ancho) / 3;     // Ancho en pixeles del bitmap	
	biHeight = imagen->alto;    // Alto en pixeles del bitmap	
	biPlanes = 1;    // Numero de planos	
	biBitCount = 24;  // Bits por pixel (1,4,8,16,24 or 32)	
	biCompression = 0;   // Tipo de compresion
	biSizeImage = imagen->alto * imagen->ancho;   // Tamanio de la imagen (sin ecabezado) en bits
	biXPelsPerMeter = 2835; // Resolucion del display objetivo en coordenada x
	biYPelsPerMeter = 2835; // Resolucion del display objetivo en coordenada y
	biClrUsed = 0;       // Numero de colores usados (solo para bitmaps con paleta)	
	biClrImportant = 0;  // Numero de colores importantes (solo para bitmaps con paleta)	

	archivoSalida = fopen(nomArchivoSalida, "w+b"); // Archivo donde se va a escribir el bitmap
	if (archivoSalida == 0) {
		printf("No ha sido posible crear el archivo: %s\n", nomArchivoSalida);
		exit(-1);
	}

	fwrite(bfType, sizeof(char), 2, archivoSalida); // Se debe escribir todo el encabezado en el archivo. En total 54 bytes.
	fwrite(&bfSize, sizeof(int), 1, archivoSalida);
	fwrite(&bfReserved, sizeof(int), 1, archivoSalida);
	fwrite(&bfOffBits, sizeof(int), 1, archivoSalida);
	fwrite(&biSize, sizeof(int), 1, archivoSalida);
	fwrite(&biWidth, sizeof(int), 1, archivoSalida);
	fwrite(&biHeight, sizeof(int), 1, archivoSalida);
	fwrite(&biPlanes, sizeof(short), 1, archivoSalida);
	fwrite(&biBitCount, sizeof(short), 1, archivoSalida);
	fwrite(&biCompression, sizeof(int), 1, archivoSalida);
	fwrite(&biSizeImage, sizeof(int), 1, archivoSalida);
	fwrite(&biXPelsPerMeter, sizeof(int), 1, archivoSalida);
	fwrite(&biYPelsPerMeter, sizeof(int), 1, archivoSalida);
	fwrite(&biClrUsed, sizeof(int), 1, archivoSalida);
	fwrite(&biClrImportant, sizeof(int), 1, archivoSalida);

	// Se escriben en el archivo los datos RGB de la imagen.
	for (y = 0; y < imagen->alto; y++) {
		for (x = 0; x < imagen->ancho; x++) {
			int pos = y * imagen->ancho + x;
			fwrite(&imagen->informacion[pos], sizeof(unsigned char), 1, archivoSalida);
		}
		fwrite(&relleno, sizeof(unsigned char), residuo, archivoSalida);
	}
	fclose(archivoSalida);
}