#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <windows.h>
using namespace std;

// Clase Episodio
class Episodio {
private:
    int numeroEpisodio; //que numero de episdodio es

public:
    // Constructor 
    Episodio(int _numeroEpisodio)
        : numeroEpisodio(_numeroEpisodio) {}

    int getNumeroEpisodio() const { return numeroEpisodio; }
};

// Clase Temporada
class Temporada {
private:
    int numeroTemporada;
    vector<Episodio> episodios;

public:
    Temporada(int _numeroTemporada) : numeroTemporada(_numeroTemporada) {}

    void agregarEpisodio(const Episodio& episodio) {
        episodios.push_back(episodio);
    }
	
    int getNumeroTemporada() const { return numeroTemporada; }
    const vector<Episodio>& getEpisodios() const { return episodios; }
};

// Clase Serie
class Serie {
private:
    string titulo;
    int anoEstreno;
    string genero;
    float calificacion; 
    vector<Temporada> temporadas;

public:
    Serie(const string& _titulo, int _anoEstreno, const string& _genero, float _calificacion = 0.0)
        : titulo(_titulo), anoEstreno(_anoEstreno), genero(_genero), calificacion(_calificacion) {}

    float getCalificacion() const { return calificacion; }
    void setCalificacion(float _calificacion) { calificacion = _calificacion; }

    void agregarTemporada(const Temporada& temporada) {
        temporadas.push_back(temporada);
    }

    string getTitulo() const { return titulo; }
    int getAnoEstreno() const { return anoEstreno; }
    string getGenero() const { return genero; }
    
    const vector<Temporada>& getTemporadas() const { return temporadas; }
};

vector<Serie> series;


// Clase Persona
class Persona {
protected:
    string nombre;
    int edad;
    char sexo;

public:
    Persona(string _nombre, int _edad, char _sexo)
        : nombre(_nombre), edad(_edad), sexo(_sexo) {}
    virtual ~Persona() = default;
};

class Usuario : public Persona {
private:
    string nombreUsuario;
    string contrasena;
    bool estaRegistrado;
    vector<Serie> seriesFavoritas; // Almacenamos objetos de tipo Serie

public:
    // Constructor para crear un usuario
    Usuario(string _nombre, int _edad, char _sexo, string _nombreUsuario, string _contrasena)
        : Persona(_nombre, _edad, _sexo), nombreUsuario(_nombreUsuario), contrasena(_contrasena), estaRegistrado(true) {}
        
    // Agregar una serie completa a las series favoritas del usuario
    void agregarSerieFavorita(const Serie& serie) {
        seriesFavoritas.push_back(serie);
    }

    // Mostrar todas las series favoritas del usuario
     void mostrarSeriesFavoritas() const {
    if (seriesFavoritas.empty()) {
        cout << "No tienes series favoritas." << endl;
        return;
    }

    // Mostrar las series favoritas del usuario en formato tabular
    cout << "=== SERIES FAVORITAS ===" << endl;
    cout << left << setw(5) << "No."
         << setw(30) << "Título"
         << setw(10) << "Año"
         << setw(15) << "Calificación"
         << setw(20) << "Género"
         << setw(15) << "Temporadas" << endl;
    cout << string(95, '-') << endl;

    for (size_t i = 0; i < seriesFavoritas.size(); i++) {
        cout << left << setw(5) << i + 1
             << setw(30) << seriesFavoritas[i].getTitulo()
             << setw(10) << seriesFavoritas[i].getAnoEstreno()
             << setw(15) << seriesFavoritas[i].getCalificacion()
             << setw(20) << seriesFavoritas[i].getGenero()
             << setw(15) << seriesFavoritas[i].getTemporadas().size() << endl;
    }
}

    // Obtener las series favoritas
    const vector<Serie>& getSeriesFavoritas() const {
        return seriesFavoritas;
    }

    // Métodos para acceder a los atributos del usuario
    string getNombre() const { return nombre; }
    int getEdad() const { return edad; }
    char getSexo() const { return sexo; }
    string getNombreUsuario() const { return nombreUsuario; }
    bool getEstaRegistrado() const { return estaRegistrado; }

    // Modificar la contraseña del usuario
    void setContrasena(const string& nuevaContrasena) { contrasena = nuevaContrasena; }

    // Modificar el estado de registro del usuario
    void setEstaRegistrado(bool estado) { estaRegistrado = estado; }

    // Verificar la contraseña
    bool verificarContrasena(const string& contrasenaIntento) const {
    cout << "Contraseña guardada: " << contrasena << endl;  // Imprime la contraseña almacenada
    cout << "Contraseña intentada: " << contrasenaIntento << endl;  // Imprime la contraseña ingresada
    return contrasena == contrasenaIntento;
}

    
};
vector<Usuario> usuarios;


// Función auxiliar para guardar una serie en el archivo
void guardarSerieEnArchivo(ofstream& archivo, const Serie& serie) {
    archivo << serie.getTitulo() << "|"
            << serie.getAnoEstreno() << "|"
            << serie.getGenero() << "|"
            << serie.getCalificacion() << "|";
}

void guardarUsuariosEnArchivo(const vector<Usuario>& usuarios) {
    ofstream archivo("usuarios.txt", ios::app); // Usamos 'app' para agregar al final del archivo
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo usuarios.txt" << endl;
        return;
    }

    for (const auto& usuario : usuarios) {
        // Guardar datos del usuario
        archivo << usuario.getNombre() << "|"
                << usuario.getEdad() << "|"
                << usuario.getSexo() << "|"
                << usuario.getNombreUsuario() << "|"
                << "*****" << "|" // Enmascaramos la contraseña por seguridad
                << usuario.getEstaRegistrado() << "|";

        // Guardar series favoritas
        const auto& seriesFavoritas = usuario.getSeriesFavoritas();
        for (const auto& serie : seriesFavoritas) {
            guardarSerieEnArchivo(archivo, serie);
        }

        archivo << endl; // Fin de un usuario
    }

    archivo.close(); // Cerrar archivo después de escribir
}

void cargarUsuariosDesdeArchivo(vector<Usuario>& usuarios) {
    ifstream archivo("usuarios.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo usuarios.txt" << endl;
        return;
    }

    string nombre, nombreUsuario, sexo;
    int edad;
    char sexoChar;
    bool estaRegistrado;
    string line, contrasena, titulo, anio, genero, calificacion;

    while (getline(archivo, line)) {
        stringstream ss(line);
        
        // Leer los datos del usuario
        getline(ss, nombre, '|');  // Leer nombre
        ss >> edad;
        ss.ignore(); // Ignorar el separador '|'
        
        ss >> sexoChar;  // Leer sexo (hombre o mujer)
        ss.ignore(); // Ignorar el separador '|'
        
        getline(ss, nombreUsuario, '|');  // Leer nombre de usuario
        ss.ignore(); // Ignorar el separador '|'
        
        getline(ss, contrasena, '|');  // Leer la contraseña
        ss >> estaRegistrado;
        ss.ignore(); // Ignorar el separador '|'

        // Crear el objeto Usuario
        Usuario usuario(nombre, edad, sexoChar, nombreUsuario, contrasena);
        usuario.setEstaRegistrado(estaRegistrado);

        // Leer las series favoritas del usuario (separadas por comas)
        string seriesLine;
        getline(ss, seriesLine);  // Leer toda la línea de series favoritas

        // Dividir la línea de series favoritas por comas
        stringstream seriesStream(seriesLine);
        while (getline(seriesStream, titulo, ',')) {
            // Leer los detalles de cada serie, separados por '|'
            stringstream serieDetails(titulo);
            getline(serieDetails, titulo, '|');
            getline(serieDetails, anio, '|');
            getline(serieDetails, genero, '|');
            getline(serieDetails, calificacion, ',');

            // Asegurarnos de que el año y la calificación son números válidos
            int anioInt = stoi(anio);
            float calificacionFloat = stof(calificacion);

            // Crear y agregar la serie a las favoritas del usuario
            Serie serie(titulo, anioInt, genero, calificacionFloat);
            usuario.agregarSerieFavorita(serie);
        }

        // Agregar el usuario con sus series favoritas al vector de usuarios
        usuarios.push_back(usuario);
    }

    archivo.close();
}






Usuario* usuarioConectado = nullptr;

void menuOpcionesUsuario(Usuario&);

// Clase Dueno (hereda de Persona)
class Dueno : public Persona {
private:
    string nombreDueno;
    string contrasenaDueno;

public:
    Dueno(string _nombre, int _edad, char _sexo, string _nombreDueno, string _contrasenaDueno)
        : Persona(_nombre, _edad, _sexo), nombreDueno(_nombreDueno), contrasenaDueno(_contrasenaDueno) {}

    string getNombreDueno() const { return nombreDueno; }

    bool verificarContrasena(string contrasenaIntento) {
        return contrasenaDueno == contrasenaIntento;
    }
};

// Clase Episodio


////					datos.txt				///
void cargarSeriesDesdeArchivo(vector<Serie>& series) {
    ifstream archivo("datos.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo datos.txt" << endl;
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        istringstream stream(linea);
        string titulo, genero;
        int anoEstreno;
        float calificacion;

        // Leer datos básicos de la serie
        getline(stream, titulo, '|');
        stream >> anoEstreno;
        stream.ignore(); // Ignorar el delimitador '|'
        getline(stream, genero, '|');
        stream >> calificacion;
        stream.ignore(); // Ignorar el delimitador '|'

        // Crear la serie
        Serie serie(titulo, anoEstreno, genero, calificacion);

        // Leer el número de temporadas
        int numeroTemporadas;
        stream >> numeroTemporadas;
        stream.ignore(); // Ignorar el delimitador '|'

        // Leer las temporadas
        for (int i = 0; i < numeroTemporadas; ++i) {
            int numeroTemporada;
            stream >> numeroTemporada;
            stream.ignore(); // Ignorar el delimitador '|'

            Temporada temporada(numeroTemporada);

            // Leer el número de episodios de la temporada
            int numeroEpisodios;
            stream >> numeroEpisodios;
            stream.ignore(); // Ignorar el delimitador '|'

            // Leer episodios
            for (int j = 0; j < numeroEpisodios; ++j) {
                int numeroEpisodio;

                // Leer el número de episodio
                stream >> numeroEpisodio;
                stream.ignore(); // Ignorar el delimitador '|'

                // Crear y agregar el episodio a la temporada 
                Episodio episodio(numeroEpisodio);
                temporada.agregarEpisodio(episodio);
            }

            // Agregar la temporada a la serie
            serie.agregarTemporada(temporada);
        }

        // Agregar la serie al vector de series
        series.push_back(serie);
    }

    archivo.close();
}

void guardarSeriesEnArchivo(const vector<Serie>& series) {
    ofstream archivo("datos.txt", ios::trunc);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo datos.txt" << endl;
        return;
    }

    for (const auto& serie : series) {
        // Guardar los datos básicos de la serie
        archivo << serie.getTitulo() << "|"
                << serie.getAnoEstreno() << "|"
                << serie.getGenero() << "|"
                << serie.getCalificacion() << "|"
                << serie.getTemporadas().size() << "|";

        // Guardar las temporadas de la serie
        for (const auto& temporada : serie.getTemporadas()) {
            archivo << temporada.getNumeroTemporada() << "|"
                    << temporada.getEpisodios().size() << "|";

            // Guardar los episodios de la temporada
            for (const auto& episodio : temporada.getEpisodios()) {
                archivo << episodio.getNumeroEpisodio() << "|";  // Solo guardar el número del episodio
            }
        }
        archivo << endl; // Nueva línea al final de la serie
    }

    archivo.close();
}



void limpiarConsola() {
    system("cls");
}

void pausar() {
    cout << "Presione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
              [](unsigned char c) { return tolower(c); });
    return lowerStr;
}

void registrar() {
    string nombre, nombreUsuario, contrasena;
    int edad;
    char sexo;

    limpiarConsola();
    cout << "=== REGISTRARSE ===" << endl;
    cout << "Ingrese su nombre: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Ingrese su edad: ";
    while (!(cin >> edad) || edad <= 0) {
        cout << "Edad inválida. Ingrese una edad positiva: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Ingrese su sexo (M/F): ";
    cin >> sexo;
    sexo = toupper(sexo);
    while (sexo != 'M' && sexo != 'F') {
        cout << "Sexo inválido. Ingrese M o F: ";
        cin >> sexo;
        sexo = toupper(sexo);
    }
    cout << "Ingrese su nombre de usuario: ";
    cin >> nombreUsuario;
    cout << "Ingrese su contraseña: ";
    cin >> contrasena;

    // Crear el nuevo usuario
    Usuario nuevoUsuario(nombre, edad, sexo, nombreUsuario, contrasena);

    // Añadir el nuevo usuario a la lista en memoria
    usuarios.push_back(nuevoUsuario);

    // Guardar el usuario en el archivo
    ofstream archivo("usuarios.txt", ios::app); // Modo append para añadir al final
    if (!archivo.is_open()) {
        cerr << "Error: no se pudo abrir el archivo usuarios.txt para escribir." << endl;
        return;
    }

    // Escribir el nuevo usuario en el archivo, junto con sus datos básicos
    archivo << nombre << "|"
            << edad << "|"
            << sexo << "|"
            << nombreUsuario << "|"
            << contrasena << "|"
            << "1"; // Estado registrado (1 = registrado)

    // No hay necesidad de guardar las series favoritas en este caso
    archivo << endl;

    archivo.close();

    cout << "Usuario registrado exitosamente." << endl;
    pausar();
}

// Variable global para almacenar al usuario conectado

void iniciarSesion() {
    string nombreUsuario, contrasena;

    limpiarConsola();
    cout << "=== INICIAR SESION ===" << endl;
    cout << "Ingrese su nombre de usuario: ";
    cin >> nombreUsuario;
    cout << "Ingrese su contrasena: ";
    cin >> contrasena;

    // Buscar al usuario en la lista de usuarios
    for (auto& usuario : usuarios) {
        if (usuario.getNombreUsuario() == nombreUsuario) {
            if (usuario.verificarContrasena(contrasena)) {
                // Establecer al usuario como conectado
                usuarioConectado = &usuario; 
                cout << "Inicio de sesion exitoso. Bienvenido, " << usuario.getNombreUsuario() << "!" << endl;

                // Ir al menú del usuario conectado
                menuOpcionesUsuario(*usuarioConectado);
                usuarioConectado = nullptr; // Limpiar usuario conectado tras salir del menú
                return;
            } else {
                cout << "Contrasena incorrecta." << endl;
                pausar();
                return;
            }
        }
    }

    // Usuario no encontrado
    cout << "Usuario no encontrado." << endl;
    pausar();
}

void mostrarUsuariosRegistrados() {
    if (usuarios.empty()) {
        cout << "No hay usuarios registrados." << endl;
        return;
    }

    cout << left << setw(5) << "No."
         << setw(20) << "Nombre"
         << setw(15) << "Edad"
         << setw(10) << "Sexo"
         << setw(20) << "Nombre de Usuario"<<endl;
    cout << string(85, '-') << endl;

    for (size_t i = 0; i < usuarios.size(); i++) {
        cout << left << setw(5) << i + 1
             << setw(20) << usuarios[i].getNombre()
             << setw(15) << usuarios[i].getEdad()
             << setw(10) << usuarios[i].getSexo()
             << setw(20) << usuarios[i].getNombreUsuario()<<endl;
    }
}


void listarSeries(const vector<Serie>&);


void restablecerContra() {
    if (!usuarioConectado) {
        cout << "Debe iniciar sesión para restablecer la contraseña." << endl;
        pausar();
        return;
    }

    string contrasenaActual, nuevaContrasena;

    limpiarConsola();
    cout << "=== RESTABLECER CONTRASEÑA ===" << endl;
    cout << "Ingrese su contraseña actual: ";
    cin >> contrasenaActual;

    // Verificar que la contraseña actual sea correcta
    if (usuarioConectado->verificarContrasena(contrasenaActual)) {
        cout << "Ingrese su nueva contraseña: ";
        cin >> nuevaContrasena;

        // Establecer la nueva contraseña
        usuarioConectado->setContrasena(nuevaContrasena);

        // Leer usuarios desde el archivo
        vector<Usuario> usuarios;
        ifstream archivoEntrada("usuarios.txt");
        if (!archivoEntrada.is_open()) {
            cout << "No se pudo abrir el archivo usuarios.txt" << endl;
            pausar();
            return;
        }

        string linea;
        while (getline(archivoEntrada, linea)) {
            stringstream ss(linea);
            string nombre, nombreUsuario, contrasena;
            int edad;
            char sexoChar;
            bool estaRegistrado;

            // Leer los datos del usuario
            getline(ss, nombre, '|');
            ss >> edad;
            ss.ignore();
            ss >> sexoChar;
            ss.ignore();
            getline(ss, nombreUsuario, '|');
            getline(ss, contrasena, '|');
            ss >> estaRegistrado;
            ss.ignore();

            Usuario usuarioLeido(nombre, edad, sexoChar, nombreUsuario, contrasena);

            // Leer las series favoritas del usuario
            vector<Serie> seriesFavoritas;
            string serieTitulo, generoSerie;
            int anoEstrenoSerie;
            float calificacion;
            while (getline(ss, serieTitulo, '|')) {
                getline(ss, generoSerie, '|');
                ss >> anoEstrenoSerie;
                ss.ignore();
                ss >> calificacion;
                ss.ignore();

                Serie serie(serieTitulo, anoEstrenoSerie, generoSerie, calificacion);
                seriesFavoritas.push_back(serie);
            }

            // Si es el usuario actual, actualizar la contraseña
            if (usuarioLeido.getNombreUsuario() == usuarioConectado->getNombreUsuario()) {
                usuarioLeido.setContrasena(nuevaContrasena);
            }

            usuarios.push_back(usuarioLeido);
        }
        archivoEntrada.close();

        // Escribir los usuarios actualizados en el archivo
        ofstream archivoSalida("usuarios.txt", ios::trunc);
        if (!archivoSalida.is_open()) {
            cout << "No se pudo abrir el archivo usuarios.txt para guardar los cambios." << endl;
            pausar();
            return;
        }

        for (const auto& usuario : usuarios) {
            archivoSalida << usuario.getNombre() << "|"
                          << usuario.getEdad() << "|"
                          << usuario.getSexo() << "|"
                          << usuario.getNombreUsuario() << "|"
                          << usuario.getEstaRegistrado() << "|";

            for (const auto& serie : usuario.getSeriesFavoritas()) {
                archivoSalida << serie.getTitulo() << "|"
                              << serie.getGenero() << "|"
                              << serie.getAnoEstreno() << "|"
                              << serie.getCalificacion() << "|";
            }
            archivoSalida << endl;
        }

        archivoSalida.close();
        cout << "Contraseña actualizada correctamente." << endl;
    } else {
        cout << "Contraseña actual incorrecta." << endl;
    }

    pausar();
}


void agregarSerieAFavoritasDeUsuario(Usuario& usuario) {
    vector<Serie> seriesDisponibles;

    // Cargar las series desde el archivo datos.txt
    cargarSeriesDesdeArchivo(seriesDisponibles);

    if (seriesDisponibles.empty()) {
        cout << "No hay series disponibles para agregar a favoritos." << endl;
        pausar();
        return;
    }

    // Mostrar las series disponibles
    listarSeries(seriesDisponibles);

    int cantidad;
    cout << "¿Cuántas series deseas agregar a tus favoritos? ";
    while (!(cin >> cantidad) || cantidad <= 0) {
        cout << "Cantidad inválida. Intente nuevamente: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Permitir al usuario seleccionar las series por índice
    for (int i = 0; i < cantidad; ++i) {
        int indice;
        cout << "Ingrese el índice de la serie que desea agregar (" << i + 1 << " de " << cantidad << "): ";
        while (!(cin >> indice) || indice < 1 || indice > static_cast<int>(seriesDisponibles.size())) {
            cout << "Índice inválido. Intente nuevamente: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Agregar la serie seleccionada a las favoritas
        Serie serieSeleccionada = seriesDisponibles[indice - 1];

        // Verificar si ya está en favoritos
        bool yaFavorita = false;
        for (const auto& favorita : usuario.getSeriesFavoritas()) {
            if (favorita.getTitulo() == serieSeleccionada.getTitulo()) {
                yaFavorita = true;
                break;
            }
        }

        if (yaFavorita) {
            cout << "La serie \"" << serieSeleccionada.getTitulo() << "\" ya está en tus favoritos." << endl;
        } else {
            usuario.agregarSerieFavorita(serieSeleccionada);
            cout << "La serie \"" << serieSeleccionada.getTitulo() << "\" ha sido agregada a tus favoritos." << endl;
        }
    }

    // Mostrar las series favoritas después de agregar nuevas
    cout << "\n=== Tus series favoritas ===" << endl;
    usuario.mostrarSeriesFavoritas();

    // Guardar los cambios en el archivo usuarios.txt
    guardarUsuariosEnArchivo(usuarios);

    cout << "Las series seleccionadas han sido agregadas a tus favoritos." << endl;
    pausar();
}


void verSeriesOrdenadasAlfabeticamente() {
    if (series.empty()) {
        cout << "No hay series disponibles para mostrar." << endl;
        pausar();
        return;
    }

    // Copiar la lista de series para ordenarlas sin modificar el original
    vector<Serie> seriesOrdenadas = series;
    
    // Ordenar las series primero por título, luego por año, calificación, género y número de temporadas
    sort(seriesOrdenadas.begin(), seriesOrdenadas.end(), [](const Serie& a, const Serie& b) {
        if (a.getTitulo() != b.getTitulo()) return a.getTitulo() < b.getTitulo(); // Ordenar por título
        if (a.getAnoEstreno() != b.getAnoEstreno()) return a.getAnoEstreno() < b.getAnoEstreno(); // Luego por año
        if (a.getCalificacion() != b.getCalificacion()) return a.getCalificacion() < b.getCalificacion(); // Luego por calificación
        if (a.getGenero() != b.getGenero()) return a.getGenero() < b.getGenero(); // Luego por género
        return a.getTemporadas().size() < b.getTemporadas().size(); // Finalmente por número de temporadas
    });

    // Encabezado de la tabla
    cout << left << setw(5) << "No."
         << setw(30) << "Título"
         << setw(10) << "Año"
         << setw(15) << "Calificación"
         << setw(20) << "Género"
         << setw(15) << "Temporadas" << endl;
    cout << string(95, '-') << endl;

    // Imprimir las series ordenadas
    for (size_t i = 0; i < seriesOrdenadas.size(); i++) {
        cout << left << setw(5) << i + 1
             << setw(30) << seriesOrdenadas[i].getTitulo()
             << setw(10) << seriesOrdenadas[i].getAnoEstreno()
             << setw(15) << seriesOrdenadas[i].getCalificacion()
             << setw(20) << seriesOrdenadas[i].getGenero()
             << setw(15) << seriesOrdenadas[i].getTemporadas().size() << endl;
    }
    pausar();
}


void verSeriesPorCalificacion() {
    vector<Serie> seriesDisponibles;

    // Cargar las series desde el archivo datos.txt
    cargarSeriesDesdeArchivo(seriesDisponibles);

    if (seriesDisponibles.empty()) {
        cout << "No hay series disponibles para mostrar." << endl;
        pausar();
        return;
    }

    // Ordenar las series por calificación en orden descendente
    sort(seriesDisponibles.begin(), seriesDisponibles.end(), [](const Serie& a, const Serie& b) {
        return a.getCalificacion() > b.getCalificacion();
    });

    // Mostrar las series ordenadas por calificación
    cout << "=== SERIES ORDENADAS POR CALIFICACIÓN ===" << endl;
    cout << left << setw(5) << "No."
         << setw(30) << "Título"
         << setw(10) << "Año"
         << setw(15) << "Calificación"
         << setw(20) << "Género"
         << setw(15) << "Temporadas" << endl;
    cout << string(95, '-') << endl;

    for (size_t i = 0; i < seriesDisponibles.size(); i++) {
        const Serie& serie = seriesDisponibles[i];

        // Mostrar los detalles de cada serie
        cout << left << setw(5) << i + 1
             << setw(30) << serie.getTitulo()
             << setw(10) << serie.getAnoEstreno()
             << setw(15) << serie.getCalificacion()
             << setw(20) << serie.getGenero()
             << setw(15) << serie.getTemporadas().size() << endl;
    }

    pausar();
}



void buscarSerie() {
    if (series.empty()) {
        cout << "No hay series disponibles para buscar." << endl;
        pausar();
        return;
    }

    string tituloBusqueda;
    limpiarConsola();
    cout << "=== BUSCAR SERIE ===" << endl;
    cout << "Ingrese el título de la serie que desea buscar: ";
    cin.ignore();
    getline(cin, tituloBusqueda);

    // Convertir el título ingresado a minúsculas para una búsqueda insensible a mayúsculas/minúsculas
    string tituloBusquedaLower = toLower(tituloBusqueda);
    bool encontrada = false;

    for (const auto& serie : series) {
        // Buscar en el título de cada serie
        if (toLower(serie.getTitulo()).find(tituloBusquedaLower) != string::npos) {
            // Mostrar la información de la serie
            cout << "Título: " << serie.getTitulo() << endl;
            cout << "Año de estreno: " << serie.getAnoEstreno() << endl;
            cout << "Género: " << serie.getGenero() << endl;
            cout << "Calificación: " << serie.getCalificacion() << endl;

            encontrada = true;
            cout << string(40, '-') << endl;
        }
    }

    if (!encontrada) {
        cout << "No se encontró ninguna serie con el título: " << tituloBusqueda << endl;
    }

    pausar();
}




void menuUsuario(){
	int opUsuario;
	do{
		system("cls");
		cout<<" === USUARIO ===\n\n"
			<<"1. Inciar sesión\n"
			<<"2. Registrarse\n"
			<<"3. Restablecer contraseña\n"
			<<"4. SALIR\n\n"
			<<"Ingrese una de las opciones: ";
			cin>>opUsuario;
		switch(opUsuario){
			case 1:
				system("cls");
				iniciarSesion();
				system("pause");
				break;
			case 2:
				system("cls");
				registrar();
				system("pause");
				break;
			case 3:
				system("cls");
				restablecerContra();
				break;
			case 4:
				break;
			default:
				cout<<"Ingrese una opcion valida\n";
				system("pause");
				break;
			
		}
	}while(opUsuario!=4);
}






//		DUEÑO
//		DUEÑO
//		DUEÑO






vector<Dueno> duenos={
    Dueno("Elvis Ronald Arocutipa Quispe",18,'M',"Elvis99","119056"),
    Dueno("Admin2",40,'F',"admin2","admin456")
};


void listarSeries(const vector<Serie>& series) {
    if (series.empty()) {
        cout << "No hay series disponibles para listar." << endl;
        pausar();  // Pausar la ejecución después de mostrar el mensaje
        return;
    }

    // Imprimir los encabezados
    cout << left << setw(5) << "No."
         << setw(35) << "Título"
         << setw(10) << "Año"
         << setw(12) << "Calificación"
         << setw(42) << "    Género"
         << setw(10) << "Temporadas" << endl;
    cout << string(100, '-') << endl;  // Línea separadora

    // Mostrar las series
    for (size_t i = 0; i < series.size(); i++) {
        cout << left << setw(5) << i + 1
             << setw(35) << series[i].getTitulo()
             << setw(10) << series[i].getAnoEstreno()
             << setw(10) << series[i].getCalificacion()
             << setw(45) << series[i].getGenero()
             << setw(10) << series[i].getTemporadas().size() << endl;
    }

    pausar();  // Pausar la ejecución después de listar las series
}



void agregarSerie(vector<Serie>& series) {
    string titulo, genero;
    int anoEstreno, numeroTemporadas, numeroEpisodios;
    float calificacion;

    // Solicitar título de la serie
    cout << "Ingrese el título: ";
    cin.ignore(); // Limpiar el buffer
    getline(cin, titulo);

    // Solicitar año de estreno
    cout << "Ingrese el año de estreno: ";
    while (!(cin >> anoEstreno) || anoEstreno < 1900 || anoEstreno > 2100) {
        cout << "Año inválido. Intente nuevamente: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar entrada
    }

    // Solicitar calificación
    cout << "Ingrese la calificación (0-10): ";
    while (!(cin >> calificacion) || calificacion < 0 || calificacion > 10) {
        cout << "Calificación inválida. Intente nuevamente: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar entrada
    }

    // Solicitar género
    cout << "Ingrese el género: ";
    cin.ignore(); // Limpiar el buffer
    getline(cin, genero);

    // Solicitar número de temporadas
    cout << "Ingrese el número de temporadas: ";
    while (!(cin >> numeroTemporadas) || numeroTemporadas < 1) {
        cout << "Número de temporadas inválido. Intente nuevamente: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar entrada
    }

    // Crear la serie
    Serie nuevaSerie(titulo, anoEstreno, genero, calificacion);

    // Solicitar detalles de las temporadas y episodios
    for (int i = 1; i <= numeroTemporadas; ++i) {
        cout << "Ingrese el número de episodios para la temporada " << i << ": ";
        while (!(cin >> numeroEpisodios) || numeroEpisodios < 1) {
            cout << "Número de episodios inválido. Intente nuevamente: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar entrada
        }

        Temporada nuevaTemporada(i);
        for (int j = 1; j <= numeroEpisodios; ++j) {
            Episodio nuevoEpisodio(j);
            nuevaTemporada.agregarEpisodio(nuevoEpisodio);
        }

        nuevaSerie.agregarTemporada(nuevaTemporada);
    }

    // Agregar la serie al vector de series
    series.push_back(nuevaSerie);

    // Confirmación
    cout << "Serie agregada exitosamente." << endl;
}



void eliminarSerie(vector<Serie>& series) {
    if (series.empty()) {
        cout << "No hay series disponibles para eliminar." << endl;
        pausar();  // Usamos la función pausar para esperar una interacción del usuario
        return;
    }

    listarSeries(series);  // Llamamos a la función para listar todas las series
    int opcion;

    cout << "Seleccione el número de la serie que desea eliminar: ";
    while (!(cin >> opcion) || opcion < 1 || opcion > static_cast<int>(series.size())) {
        cout << "Opción inválida. Intente nuevamente: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Limpiar el buffer de entrada
    }

    // Eliminamos la serie seleccionada
    series.erase(series.begin() + (opcion - 1));

    // Guardamos la lista actualizada de series en el archivo
    guardarSeriesEnArchivo(series);  // Esta función debe ser implementada correctamente para guardar los datos
    cout << "Serie eliminada exitosamente." << endl;

    pausar();  // Pausamos para permitir al usuario ver el mensaje
}

void menuOpcionesUsuario(Usuario& usuario) {
    int opcion;
    do {
        system("cls");
        cout << "=== MENÚ DE OPCIONES ===\n\n"
             << "1. Ver series disponibles\n"
             << "2. Ver series ordenadas alfabéticamente\n"
             << "3. Ver series ordenadas por calificación\n"
             << "4. Buscar una serie por título\n"
             << "5. Agregar serie a Favoritos\n"
             << "6. Ver series favoritas\n"
             << "7. Cerrar sesión\n\n"
             << "Ingrese una opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            system("cls");
            listarSeries(series);
            system("pause");
            break;

        case 2:
            system("cls");
            verSeriesOrdenadasAlfabeticamente();
            break;

        case 3:
            system("cls");
            verSeriesPorCalificacion();
            break;

        case 4:
            system("cls");
            buscarSerie();
            break;

        case 5:
            system("cls");
            agregarSerieAFavoritasDeUsuario(usuario);
            // Guardar cambios en el archivo después de agregar una serie favorita
            guardarUsuariosEnArchivo(usuarios);
            break;

        case 6:
            system("cls");
            usuario.mostrarSeriesFavoritas(); // Mostrar las series favoritas del usuario conectado
            system("pause");
            break;

        case 7:
            cout << "Cerrando sesión...\n";
            usuarioConectado = nullptr; // Limpiamos el usuario conectado al cerrar sesión
            break;

        default:
            cout << "Ingrese una opción válida.\n";
            system("pause");
            break;
        }
    } while (opcion != 7);
}


void menuGestionSeries(){
	vector<Serie> series;
    cargarSeriesDesdeArchivo(series);
    int opcion;
    do {
        system("cls");
        cout << "=== GESTIÓN DE SERIES ===\n\n"
             << "1. Listar series\n"
             << "2. Agregar serie\n"
             << "3. Eliminar serie\n"
             << "4. Mostrar usuarios registrados\n"
             << "5. SALIR\n\n"
             << "Ingrese una opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            system("cls");
            listarSeries(series);
            system("pause");
            break;
        case 2:
            system("cls");
            agregarSerie(series);
            system("pause");
            break;
        case 3:
            system("cls");
            eliminarSerie(series);
            system("pause");
            break;
        case 4:
            system("cls");
            mostrarUsuariosRegistrados();  // Mostrar los usuarios registrados
            system("pause");
            break;
        case 5:
            break;
        default:
            cout << "Ingrese una opción válida." << endl;
            system("pause");
            break;
        }
    } while (opcion != 5);
}

void iniciarSesionDueno() {
    string nombreUsuario, contrasena;

    cout << "=== INICIAR SESIÓN (DUEÑO) ===" << endl;
    cout << "Ingrese su nombre de usuario: ";
    cin >> nombreUsuario;
    cout << "Ingrese su contraseña: ";
    cin >> contrasena;

    for (auto& dueno : duenos) {
        if (dueno.getNombreDueno() == nombreUsuario && dueno.verificarContrasena(contrasena)) {
            cout << "Inicio de sesión exitoso. ¡Bienvenido, " << dueno.getNombreDueno() << "!" << endl;
            pausar(); // Asumiendo que `pausar` es una función definida.
            menuGestionSeries();
            return;
        }
    }
    cout << "Usuario o contraseña incorrectos." << endl;
    pausar();
}

void menuDueno(){
    int opcion;
    do{
        system("cls");
        cout<<"=== MENÚ DUEÑO ===\n\n"
            <<"1. Iniciar sesión\n"
            <<"2. SALIR\n\n"
            <<"Ingrese una opción: ";
        cin>>opcion;

        switch(opcion){
        case 1:
            system("cls");
            iniciarSesionDueno();
            break;
        case 2:
            break;
        default:
            cout<<"Ingrese una opción válida."<<endl;
            system("pause");
            break;
        }
    }while(opcion!=2);
}

// Función principal
int main(){
	SetConsoleOutputCP(CP_UTF8);
	cargarSeriesDesdeArchivo(series);	
	cargarUsuariosDesdeArchivo(usuarios);
	
	
	int op;

	do{
		system("cls");
		cout<<" SERIES\n\n"
			<<"1. Usuario\n"
			<<"2. Dueño\n"
			<<"3. SALIR\n\n"
			<<"Ingrese una de las opciones: ";
			cin>>op;
			
		switch(op){
			case 1:
				system("cls");
				menuUsuario();
				system("pause");
				break;
			case 2:
				system("cls");
				menuDueno();
				system("pause");
				break;
			case 3:
				cout<<"SALIENDO . . .\n";
				system("pause");
				break;
			default:
				cout<<"Ingrese una opcion valida\n";
				system("pause");
				break;
		}
	}while(op!=3);
	

    return 0;
}