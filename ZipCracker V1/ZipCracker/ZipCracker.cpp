

// Includes
#include <iostream>
#include <limits>
#include <string>
#include <zip.h>
#include <fstream>

// Usings
using namespace std;

// Prototipos de las funciones
int pedirDatos();

void verificarNumeroCantidadCaracteres(int& cantidadCaracteres, istream& cin);

void generarCombinaciones(int& cantidadCaracteres, string palabraActual);

void bruteForceZip(string& contra);


int main()
{
	



	int cantidadCaracteres = pedirDatos();



	ofstream archivoContra("PosiblesContras.txt");


	archivoContra << "Lista de contraseñas a probar" << "\n";

	archivoContra.close();


	generarCombinaciones(cantidadCaracteres, "");





	return 0;
}

void verificarNumeroCantidadCaracteres(int& cantidadCaracteres, istream& cin)
{
	while (!cin || cantidadCaracteres < 0)
	{
		if (!cin)
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Se brinca el input invalido
			cout << "Debe ingresar un numero en el campo de cantidad de caracteres" << endl;
		}

		if (cantidadCaracteres < 0)
		{
			cout << "Debe ingresar un numero positivo en el campo de cantidad de caracteres" << endl;
		}

		cout << "Digite la cantidad de caracteres de la contra: " << endl;

		cin >> cantidadCaracteres;
	}
}

int pedirDatos()
{

	int cantidadCaracteres;

	cout << "Digite la cantidad de caracteres de la contra: " << endl;

	cin >> cantidadCaracteres;

	verificarNumeroCantidadCaracteres(cantidadCaracteres, cin);

	return cantidadCaracteres;
}

void generarCombinaciones(int& cantidadCaracteres, string palabraActual) {
	string arregloLetrasNumeros = "abcdefghijklmnopqrstuvwxyz0123456789";


	if (palabraActual.length() == cantidadCaracteres) {


		bruteForceZip(palabraActual);
		return;
	}

	for (char c : arregloLetrasNumeros) {

		generarCombinaciones(cantidadCaracteres, palabraActual + c);
	}
};


void bruteForceZip(string& contra) {


	int codigoDeError;
	zip_t* archivoZip = zip_open("Prueba.zip", 0, &codigoDeError);


	if (archivoZip == NULL) {
		zip_error_t errorZip;
		zip_error_init_with_code(&errorZip, codigoDeError);

		string errorString = zip_error_strerror(&errorZip);
		zip_error_fini(&errorZip);
		cout << "Error al abrir el archivo zip" << errorString << endl;
		return;
	};
	cout << "Archivo zip abierto con exito" << endl;


	struct zip_stat informacionZip;
	zip_stat_init(&informacionZip);
	int estadoInformacionZip = zip_stat(archivoZip, "Prueba1.txt", 0, &informacionZip);






	if (informacionZip.encryption_method == ZIP_EM_AES_256) {
		cout << "El archivo se encuentra cifrado utilizando AES-256" << endl;
	}


	char* contenidos = new char[informacionZip.size];

	cout << "Contra generada: " << contra << "\n";


	zip_file* archivoDentroZip = zip_fopen_encrypted(archivoZip, informacionZip.name, 0, contra.c_str());

	if (archivoDentroZip == NULL) {
		zip_error_t* error = zip_get_error(archivoZip);

		string errorString = zip_error_strerror(error);
		zip_error_fini(error);
		zip_close(archivoZip);
		cout << "Error al abrir el archivo dentro del zip: " << errorString << endl;
		delete[] contenidos;
		return;
	}
	else {
		int tamanoArchivo = zip_fread(archivoDentroZip, contenidos, informacionZip.size);

		
		if (tamanoArchivo < 0) {
			cout << "Error el tamano del archivo no puede ser menor a 0";
			delete[] contenidos;
			return;
		}

		ofstream archivoContra("PosiblesContras.txt", ios::app);


		archivoContra << "Posible contraseña: " << contra << "\n";

		archivoContra.close();



		cout << "Tamano del archivo buscado" << " " << tamanoArchivo << " Bytes" << endl;

	};




	zip_fclose(archivoDentroZip);
	zip_close(archivoZip);
	delete[] contenidos;
}
