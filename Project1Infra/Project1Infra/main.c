// ISIS-1304 - FUNDAMENTOS DE INFRAESTRUCTURA TECNOLOGICA - PROYECTO 1 - 201910
// EL GRUPO DEBE SER DESARROLLADO EN GRUPOS DE A 3 PERSONAS MAXIMO
//
// DESARROLLADO POR:
// Daniel Galindo - 201813517
// Santiago Estupiñán - 201813414
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


// Función que carga el bmp en la estructura Imagen
void cargarBMP24(Imagen * imagen, char * nomArchivoEntrada);

// Función que guarda el contenido de la estructura imagen en un archivo binario
void guardarBMP24(Imagen * imagen, char * nomArchivoSalida);

//Función que inserta un mensaje en la imagen usando n bits por Byte
void insertarMensaje(Imagen * img, unsigned char mensaje[], int n);

//Función que lee un mensaje de una imagen dando la longitud del mensaje y el numero de bits por byte usados
void leerMensaje(Imagen * img, unsigned char msg[], int l, int n);

//Función que saca n bits de una secuencia de caracteres a partir de una posición dada
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
* parámetro img: Apuntador a una imagen en cuyos pixeles se almacenará el mensaje.
* parámetro mensaje: Apuntador a una cadena de caracteres con el mensaje.
* parámetro n: Cantidad de bits del mensaje que se almacenarán en cada componente de color de cada pixel. 0 < n <= 8.
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
	int k = 0;
	for(int i = 0; i < (alto*ancho); i++)
	{
		imagen->informacion[i] = (imagen->informacion[i] >> n)<<n;
		for(int j = 0; j < n; j++, k++)
		{
			if((mensaje[k] & (1<<(K%8))) != 0)
			{
				imagen->informacion[i] = ( 1<<(8 - n -j) | imagen->informacion[i] );
			}
		}
	}

}
/**
* Extrae un mensaje de tamanio l, guardado de a n bits por componente de color, de la imagen apuntada por img
* parámetro img: Apuntador a una imagen que tiene almacenado el mensaje en sus pixeles.
* parámetro msg: Apuntador a una cadena de caracteres donde se depositará el mensaje.
* parámetro l: Tamanio en bytes del mensaje almacenado en la imagen.
* parámetro n: Cantidad de bits del mensaje que se almacenan en cada componente de color de cada pixel. 0 < n <= 8.
*/
void leerMensaje(Imagen * img, unsigned char msg[], int l, int n) {
	int primeraVez = 1;		//variable de referencia para el primer caso (es decir, cuando se empieza a sacar los primeros n bits de todo el arreglo de información
	int pos = 0;	//Contador de posiciones del arreglo informacion de img
	int totbits = 0;	//contador de cuántos bits se tienen de una letra
	int vtb = 0;	//Contador de cuántas letras se han leído y a la par, es el índice donde se guardará la letra completa en msg
	int faltan = 0;	//Contador de cuántos bits faltan
	unsigned char temp1;	//Variable donde se guardan los primeros n bits de la letra en las posiciones más significativas del char
	unsigned char tempAux;	//Variable que guardan los siguientes n bits de la letra
	unsigned char tempFalta;	//Variable que guarda los ultimos bits necesarios para completar la letra
	unsigned char tempResiduo;	//Variable que guarda los bits restantes (son menos que n) de cada unsigned char de informacion
	unsigned char letra;	//Char completo a guardar en msg
	unsigned char temp1Def;	//Variable para guardar temp1 con el corrimiento necesario aplicado
	unsigned char tempAuxDef;	//variable para guardar tempAux con el corrimiento necesario aplicado
	unsigned char tempResiduoDef;	//Variable para guardar tempResiduo con el corriemiento necesario aplicado

	   
	while(vtb < l){	//Recorrer hasta que se complete la longitud del mensaje
		if (primeraVez == 1) {	//Caso inicial, de primera vez que se empieza la lectura de los unsigned char guardados en informacion
			
			//printf("secuencia seria: %s ", &img->informacion[pos]);
			temp1 = sacarNbits(&img->informacion[pos], 8 - n, n);	//Asignación de temp1
			temp1Def = temp1 << (8 - n); //Corrimiento de temp1 para guardar los bits extraídos en las posiciones más significativas
			totbits = n; //Aumento el total de bits que se tienen en la letra
			faltan = 8 - totbits;	//Calculo cuántos bits faltan para completar la letra
			primeraVez++; //Sumo primeravez para no volver a entrar en este if
			
		}
	
		else { //Caso cuando no es la primera vez que se lee la imagen en busca del mensaje
			
			if (faltan > n) {	//Caso cuando en la letra que se esta formando aun hay espacios para n bits más
				
				tempAux = sacarNbits(&img->informacion[pos], 8 - n, n);	//generar tempAux, desde los 8-n bits del unsigned char de informacion, guardando otros n bits
				totbits += n;	//Sumo n a totBits para saber que hay totBits en la letra
				faltan = 8 - totbits; //Calcula cuantos bits faltan para completar la palabra
				tempAuxDef = tempAux << faltan;	//Correr tempAux la cantidad necesaria para juntar estos bits con los de temp1Def
				temp1Def = temp1Def | tempAuxDef;	//Juntar bits de temp1Def con tempAuxDef con operador OR y dejar en cero la cantidad "faltan" de bits
			}
			else {	//Caso cuando en la letra que se está formando no caben n bits más
				
				tempFalta = sacarNbits(&img->informacion[pos], 8 - n, faltan);	//Asignar tempFalta con bits extraídos desde 8-n, tomando los que hagan falta (variable faltan) para completar la palabra
				tempResiduo = sacarNbits(&img->informacion[pos], 8 - (n - faltan), n - faltan); //En vista de que en el unsigned char de informacion aun queda cierta cantidad de bits del total de n bits, se toman los bit restantes y se asignan a esta variable a modo de residuo
				tempResiduoDef = tempResiduo << (8 - (n - faltan));	//Desplazo tempResiduo a la izquierda para dejar sus bits en la posición más significativa de un nuevo char que se usa más adelante
				totbits += faltan; //Se suman faltan a total de bits
				if (totbits == 8) {	//Si totBits es 8, es decir que ya se tiene lo necesario para formar una letra, se hace lo siguiente
					letra = temp1Def | tempFalta; //Se crea la letra con operando OR entre temp1Def y tempFalta
					msg[vtb] = letra; //Se guarda la letra creada en msg en posición vtb
					vtb++; //Se suma 1 a vtb indicando que ya se tiene una letra del mensaje y que la siguiente letra se guarda en la siguiente posicion de msg
					totbits = n - faltan;	//Ahora total de bits es n - faltan, lo que resulta en la cantidad de bits que se tienen guardados en tempResiduoDef (esto porque lo que hay en tempResiduoDef es el comienzo de la siguiente letra)
					faltan = 8 - totbits;	//Faltan 8-totalBits que tengo en tempReiduoDef
					temp1Def = tempResiduoDef;	//Antes de la siguiente iteración, se pasa lo que está en tempResiduoDef a temp1Def, ya que temp1Def es el comienzo de la siguiente letra
				}
			}
		}
		pos++; //Avanzar de posición en arreglo de informacion de la imagen
		
	}
	
	// TODO: Desarrollar OBLIGATORIAMENTE en su totalidad.

}
/**
* Extrae n bits a partir del bit que se encuentra en la posición bitpos en la secuencia de bytes que
* se pasan como parámetro
* parámetro secuencia: Apuntador a una secuencia de bytes.
* parámetro n: Cantidad de bits que se desea extraer. 0 < n <= 8.
* parámetro bitpos: Posición del bit desde donde se extraerán los bits. 0 <= n < 8*longitud de la secuencia
* retorno: Los n bits solicitados almacenados en los bits menos significativos de un unsigned char
*/
unsigned char sacarNbits(unsigned char secuencia[], int bitpos, int n) {

	unsigned char res; //esta es la variable de respuesta
	int byte = bitpos / 8; // con esto se sabe en que byte de la secuencia[] está n
	unsigned char pre = secuencia[byte]; //Variable con el char de donde se quieren sacar los bits
	int desp = 8 - n; // valor de cuántos desplazamientos en el char hay que hacer para obtener los n bits
	if ((n < 8) && ((bitpos % 8) != 0)) { //Caso en que se quieren agrupar menos de 8 bits del byte y además la posición del bit deseado no es el bit más significativo de cualquiera de los bytes de la secuencia parámtero.
		unsigned char p = pre << bitpos; // Primero se desplaza a la izquierda para eliminar los demás bits significativos
		unsigned char q = p >> desp; // Luego se desplaza a la derecha para dejar los n bits a la derecha y que queden en la posición menos significativa del unsigned char
		res = q;	//Se asigna el valor modificado a la respuesta de retorno
	}
	else if ((n < 8) && ((bitpos % 8) == 0)) { //Caso en que se quieren sacar menos que 8 bits del byte y la posición deseada es el bit más significativo de cualquier byte de la secuencia parámetro. 
		unsigned char t = pre >> desp;	//Se desplaza lo necesario para dejar los n bits hacia la izquierda.
		res = t;	//Se asigna el valor modificado a la respuesta de retorno
	}
	else if (n == 8) { //Caso en que se quiera todo el byte de cualquier poisición de la secuencia parámetro
		res = pre;	//Se asigna el valor modificado a la respuesta de retorno

	}
	//Guardar el char con los n bits en el char de respuesta

	return res;
	// DESARROLLO OPCIONAL: Puede ser útil para el desarrollo de los procedimientos obligatorios.
}

// Lee un archivo en formato BMP y lo almacena en la estructura img
// NO MODIFICAR
void cargarBMP24(Imagen * imagen, char * nomArchivoEntrada) {
	// bmpDataOffset almacena la posición inicial de los datos de la imagen. Las otras almacenan el alto y el ancho
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

	fseek(bitmapFile, 10, SEEK_SET); // 10 es la posición del campo "Bitmap Data Offset" del bmp	
	fread(&bmpDataOffset, sizeof(int), 1, bitmapFile);

	fseek(bitmapFile, 18, SEEK_SET); // 18 es la posición del campo "height" del bmp
	fread(&bmpWidth, sizeof(int), 1, bitmapFile);
	bmpWidth = bmpWidth * 3;

	fseek(bitmapFile, 22, SEEK_SET); // 22 es la posición del campo "width" del bmp
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

// Esta función se encarga de guardar una estructura de Imagen con formato de 24 bits (formato destino) en un archivo binario
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