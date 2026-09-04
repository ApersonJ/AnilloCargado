#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <limits>

using namespace std;

const double PI = 3.14159265358979323846;
const double INFINITO = numeric_limits<double>::infinity();

// -----------------------------------------------------------------------------
// Prototipos de funciones
// -----------------------------------------------------------------------------

double calcularDistancia(double theta, double x, double y, double z, double R);
double funcionIntegrando(double theta, double x, double y, double z, double R);

double metodoRectangulos(double a, double b, int n, double x, double y, double z, double R);
double metodoTrapecio(double a, double b, int n, double x, double y, double z, double R);
double metodoSimpson(double a, double b, int n, double x, double y, double z, double R);

double calcularPotencial(double x, double y, double z, double R, int n, int metodo);
double potencialAnaliticoZ(double z, double R);

void graficar(const string &script, const string &nombre, const string &imagen);

// Rutinas para los 5 experimentos
void ConvergenciaEspacial();
void ConvergenciaAngular();
void CortesFisicos();
void CampoElectrico();
void ValidacionAnalitica();
void DependenciaGeometrica();

// -----------------------------------------------------------------------------
// Función Principal
// -----------------------------------------------------------------------------

int main()
{
    cout << "--- LABORATORIO: ANILLO CARGADO ---" << endl;

    ConvergenciaEspacial();
    ConvergenciaAngular();
    CortesFisicos();
    CampoElectrico();
    ValidacionAnalitica();
    DependenciaGeometrica();

    cout << "\nExperimentos finalizados correctamente." << endl;

    return 0;
}

// -----------------------------------------------------------------------------
// Geometría e Integración
// -----------------------------------------------------------------------------

double calcularDistancia(double theta, double x, double y, double z, double R)
{
    double xCarga = R * cos(theta);
    double yCarga = R * sin(theta);

    double dx = x - xCarga;
    double dy = y - yCarga;

    return sqrt(dx * dx + dy * dy + z * z);
}

double funcionIntegrando(double theta, double x, double y, double z, double R)
{
    double distancia = calcularDistancia(theta, x, y, z, R);

    if (distancia == 0.0) {
        return INFINITO;
    }

    return 1.0 / distancia;
}

double metodoRectangulos(double a, double b, int n, double x, double y, double z, double R)
{
    double dtheta = (b - a) / n;
    double suma = 0.0;

    for (int i = 1; i <= n; i++) {
        double theta_i_1 = a + (i - 1) * dtheta;
        double theta_i   = a + i * dtheta;
        double thetaMedio = (theta_i_1 + theta_i) / 2.0;

        suma += funcionIntegrando(thetaMedio, x, y, z, R);
    }

    return suma * dtheta;
}

double metodoTrapecio(double a, double b, int n, double x, double y, double z, double R)
{
    double dtheta = (b - a) / n;
    double suma = 0.0;

    suma += funcionIntegrando(a, x, y, z, R);

    for (int i = 1; i <= n - 1; i++) {
        double theta_i = a + i * dtheta;
        suma += 2.0 * funcionIntegrando(theta_i, x, y, z, R);
    }

    suma += funcionIntegrando(b, x, y, z, R);

    return (dtheta / 2.0) * suma;
}

double metodoSimpson(double a, double b, int n, double x, double y, double z, double R)
{
    if (n <= 0 || (n % 2) != 0) return 0.0;

    double dtheta = (b - a) / n;
    double suma = funcionIntegrando(a, x, y, z, R) + funcionIntegrando(b, x, y, z, R);

    for (int j = 1; j <= (n / 2) - 1; j++) {
        int i = 2 * j;
        suma += 2.0 * funcionIntegrando(a + i * dtheta, x, y, z, R);
    }

    for (int j = 1; j <= (n / 2); j++) {
        int i = 2 * j - 1;
        suma += 4.0 * funcionIntegrando(a + i * dtheta, x, y, z, R);
    }

    return (dtheta / 3.0) * suma;
}

double calcularPotencial(double x, double y, double z, double R, int n, int metodo)
{
    double distanciaAlCentro2D = sqrt(x * x + y * y);
    double tolerancia = 1.0e-10 * R;

    if (fabs(z) < tolerancia && fabs(distanciaAlCentro2D - R) < tolerancia) {
        return INFINITO;
    }

    double a = 0.0;
    double b = 2.0 * PI;
    double integral = 0.0;

    if (metodo == 1) {
        integral = metodoRectangulos(a, b, n, x, y, z, R);
    } else if (metodo == 2) {
        integral = metodoTrapecio(a, b, n, x, y, z, R);
    } else if (metodo == 3) {
        integral = metodoSimpson(a, b, n, x, y, z, R);
    }

    return R * integral;
}

double potencialAnaliticoZ(double z, double R)
{
    return (2.0 * PI * R) / sqrt(R * R + z * z);
}

void graficar(const string &script, const string &nombre, const string &imagen)
{
    ofstream file(nombre.c_str());
    if (file.is_open()) {
        file << script;
        file.close();

        string comando = "gnuplot " + nombre;
        system(comando.c_str());
        cout << "Grafico listo: " << imagen << endl;
    } else {
        cerr << "Error al abrir script: " << nombre << endl;
    }
}

// -----------------------------------------------------------------------------
// Experimentos
// -----------------------------------------------------------------------------

void ConvergenciaEspacial()
{
    cout << "\n1. Convergencia Espacial" << endl;

    double R = 1.0;
    int n = 3000;
    double xMin = -2.0, xMax = 2.0, yMin = -2.0, yMax = 2.0;

    int valoresN[4] = {30, 60, 120, 240};
    double xTest = 1.5, yTest = 1.5;

    for (int k = 0; k < 4; k++) {
        int N = valoresN[k];
        string nombre = "../resultados/potencial_N" + to_string(N) + ".dat";
        ofstream archivo(nombre.c_str());

        double dx = (xMax - xMin) / (N - 1);
        double dy = (yMax - yMin) / (N - 1);
        archivo << scientific << setprecision(10);

        for (int i = 0; i < N; i++) {
            double x = xMin + i * dx;
            for (int j = 0; j < N; j++) {
                double y = yMin + j * dy;
                double V = calcularPotencial(x, y, 0.0, R, n, 2);

                if (isinf(V)) archivo << x << " " << y << " NaN\n";
                else archivo << x << " " << y << " " << V << "\n";
            }
            archivo << "\n";
        }
        archivo.close();

        double V_rec = calcularPotencial(xTest, yTest, 0.0, R, n, 1);
        double V_trap = calcularPotencial(xTest, yTest, 0.0, R, n, 2);
        double V_simp = calcularPotencial(xTest, yTest, 0.0, R, n, 3);

        cout << "N=" << N << " -> V_Rect=" << V_rec << " V_Trap=" << V_trap << " V_Simp=" << V_simp << endl;
    }

    string script =
        "set terminal pngcairo size 800,600\n"
        "set output '../grafica/potencial_3D_N120.png'\n"
        "set title 'Superficie 3D del Potencial V(x,y) (N=120)'\n"
        "set xlabel 'x'\n"
        "set ylabel 'y'\n"
        "set zlabel 'V(x,y)'\n"
        "set pm3d\n"
        "set hidden3d\n"
        "splot '../resultados/potencial_N120.dat' using 1:2:3 with lines\n";

    graficar(script, "../scripts/script_espacial.gnuplot", "../grafica/potencial_3D_N120.png");
}

void ConvergenciaAngular()
{
    cout << "\n2. Convergencia Angular" << endl;

    double R = 1.0;
    double xP = 1.2, yP = 0.0;

    double V_ref = calcularPotencial(xP, yP, 0.0, R, 3000, 3);

    cout << "V_ref (n=3000): " << V_ref << endl;

    ofstream archivoData("../resultados/convergencia_angular.dat");
    archivoData << scientific << setprecision(10);
    archivoData << "#n Err_Rect Err_Trap Err_Simp\n";

    int valoresN[6] = {10, 20, 50, 100, 200, 500};
    for (int k = 0; k < 6; k++) {
        int n = valoresN[k];

        double V_rec = calcularPotencial(xP, yP, 0.0, R, n, 1);
        double V_trap = calcularPotencial(xP, yP, 0.0, R, n, 2);
        double V_simp = calcularPotencial(xP, yP, 0.0, R, n, 3);

        double errRec = fabs(V_rec - V_ref) / V_ref;
        double errTrap = fabs(V_trap - V_ref) / V_ref;
        double errSimp = fabs(V_simp - V_ref) / V_ref;

        archivoData << n << " " << errRec << " " << errTrap << " " << errSimp << "\n";

        cout << "n=" << n << " | ErrRect: " << errRec << " ErrTrap: " << errTrap << " ErrSimp: " << errSimp << endl;
    }
    archivoData.close();

    string script =
        "set terminal pngcairo size 800,600\n"
        "set output '../grafica/convergencia_angular.png'\n"
        "set title 'Convergencia del Error Relativo vs Subintervalos (n)'\n"
        "set xlabel 'n (Subintervalos)'\n"
        "set ylabel 'Error Relativo'\n"
        "set logscale x\n"
        "set logscale y\n"
        "set grid\n"
        "plot '../resultados/convergencia_angular.dat' using 1:2 with linespoints title 'Rectangulos', "
        "'../resultados/convergencia_angular.dat' using 1:3 with linespoints title 'Trapecio', "
        "'../resultados/convergencia_angular.dat' using 1:4 with linespoints title 'Simpson'\n";

    graficar(script, "../scripts/script_angular.gnuplot", "../grafica/convergencia_angular.png");
}

void CortesFisicos()
{
    cout << "\n3. Cortes Fisicos" << endl;

    double R = 1.0;
    int N = 200, n = 2000;
    double xMin = -2.0, xMax = 2.0;
    double y0_valores[3] = {0.0, 0.5, 1.0};

    for (int k = 0; k < 3; k++) {
        double y0 = y0_valores[k];
        string nombre = "../resultados/corte_y_" + to_string((int)(y0 * 10)) + ".dat";
        ofstream archivo(nombre.c_str());

        archivo << "# x  V_Rectangulos  V_Trapecio  V_Simpson\n";
        archivo << scientific << setprecision(10);
        double dx = (xMax - xMin) / (N - 1);

        for (int i = 0; i < N; i++) {
            double x = xMin + i * dx;
            double Vr = calcularPotencial(x, y0, 0.0, R, n, 1);
            double Vt = calcularPotencial(x, y0, 0.0, R, n, 2);
            double Vs = calcularPotencial(x, y0, 0.0, R, n, 3);

            if (isinf(Vt)) {
                archivo << x << " NaN NaN NaN\n";
            } else {
                archivo << x << " " << Vr << " " << Vt << " " << Vs << "\n";
            }
        }
        archivo.close();
        cout << "Corte y0 = " << y0 << " guardado." << endl;
    }

    string script =
        "set terminal pngcairo size 800,600\n"
        "set output '../grafica/cortes_fisicos.png'\n"
        "set title 'Cortes 1D del Potencial V(x, y_0)'\n"
        "set xlabel 'x'\n"
        "set ylabel 'V(x)'\n"
        "set grid\n"
        "plot '../resultados/corte_y_0.dat' using 1:3 with lines title 'y_0 = 0.0', "
        "'../resultados/corte_y_5.dat' using 1:3 with lines title 'y_0 = 0.5', "
        "'../resultados/corte_y_10.dat' using 1:3 with lines title 'y_0 = 1.0'\n";

    graficar(script, "../scripts/script_cortes.gnuplot", "../grafica/cortes_fisicos.png");
}

void CampoElectrico()
{
    cout << "\n4. Campo Electrico" << endl;

    double R = 1.0;
    int N = 20, n = 2000; // Reducimos N a 20 para no amontonar vectores
    double xMin = -2.0, xMax = 2.0, yMin = -2.0, yMax = 2.0;

    ofstream archivo("../resultados/field_data.dat");
    archivo << scientific << setprecision(10);

    double dx = (xMax - xMin) / (N - 1);
    double dy = (yMax - yMin) / (N - 1);

    for (int i = 1; i < N - 1; i++) {
        double x = xMin + i * dx;
        for (int j = 1; j < N - 1; j++) {
            double y = yMin + j * dy;

            double V_xp = calcularPotencial(x + dx, y, 0.0, R, n, 3);
            double V_xm = calcularPotencial(x - dx, y, 0.0, R, n, 3);
            double V_yp = calcularPotencial(x, y + dy, 0.0, R, n, 3);
            double V_ym = calcularPotencial(x, y - dy, 0.0, R, n, 3);

            if (isinf(V_xp) || isinf(V_xm) || isinf(V_yp) || isinf(V_ym)) {
                archivo << x << " " << y << " NaN NaN\n";
            } else {
                double Ex = -(V_xp - V_xm) / (2.0 * dx);
                double Ey = -(V_yp - V_ym) / (2.0 * dy);
                double EMag = sqrt(Ex * Ex + Ey * Ey);

                // Normalizamos el tamaño de las flechas a una longitud fija de 0.15
                double escala = 0.15;
                double dx_vector = (EMag > 0) ? (Ex / EMag) * escala : 0.0;
                double dy_vector = (EMag > 0) ? (Ey / EMag) * escala : 0.0;

                archivo << x << " " << y << " " << dx_vector << " " << dy_vector << "\n";
            }
        }
        archivo << "\n";
    }
    archivo.close();
    cout << "Campo electrico exportado." << endl;

    string script =
        "set terminal pngcairo size 800,600\n"
        "set output '../grafica/campo_electrico.png'\n"
        "set title 'Mapa Vectorial del Campo Electrico E(x,y)'\n"
        "set xlabel 'x'\n"
        "set ylabel 'y'\n"
        "set xrange [-2:2]\n"
        "set yrange [-2:2]\n"
        "set size square\n"
        "set grid\n"
        "plot '../resultados/field_data.dat' using 1:2:3:4 with vectors head filled lt 2 title 'Direccion de E(x,y)'\n";

    graficar(script, "../scripts/script_campo.gnuplot", "../grafica/campo_electrico.png");
}

void ValidacionAnalitica()
{
    cout << "\n5. Validacion sobre eje Z" << endl;

    double R = 1.0;
    int n = 1000;
    double zValores[5] = {0.1, 0.5, 1.0, 2.0, 5.0};

    ofstream archivo("../resultados/validacion_z.dat");
    archivo << "# z  V_Analitico  V_Rect  V_Trap  V_Simp\n";
    archivo << scientific << setprecision(10);

    for (int i = 0; i < 5; i++) {
        double z = zValores[i];
        double V_exacto = potencialAnaliticoZ(z, R);

        double Vr = calcularPotencial(0.0, 0.0, z, R, n, 1);
        double Vt = calcularPotencial(0.0, 0.0, z, R, n, 2);
        double Vs = calcularPotencial(0.0, 0.0, z, R, n, 3);

        archivo << z << " " << V_exacto << " " << Vr << " " << Vt << " " << Vs << "\n";

        double errT = fabs(Vt - V_exacto) / V_exacto;
        double errS = fabs(Vs - V_exacto) / V_exacto;

        cout << "z=" << z << " V_exacto=" << V_exacto << " ErrTrap=" << errT << " ErrSimp=" << errS << endl;
    }
    archivo.close();

    string script =
        "set terminal pngcairo size 800,600\n"
        "set output '../grafica/validacion_eje_z.png'\n"
        "set title 'Validacion sobre el Eje Z: Numérico vs Analitico'\n"
        "set xlabel 'z'\n"
        "set ylabel 'V(0,0,z)'\n"
        "set grid\n"
        "plot '../resultados/validacion_z.dat' using 1:2 with lines lw 2 title 'Analitico Exacto', "
        "'../resultados/validacion_z.dat' using 1:5 with points pt 7 ps 1.5 title 'Simpson (Numérico)'\n";

    graficar(script, "../scripts/script_validacion.gnuplot", "../grafica/validacion_eje_z.png");
}

void DependenciaGeometrica()
{
    cout << "\n6. Dependencia Geometrica con R" << endl;

    int N = 200, n = 2000;
    double xMin = -3.0, xMax = 3.0;
    double rValores[3] = {0.5, 1.0, 2.0};

    for (int k = 0; k < 3; k++) {
        double R = rValores[k];
        string nombre = "../resultados/corte_R_" + to_string((int)(R * 10)) + ".dat";
        ofstream archivo(nombre.c_str());

        double dx = (xMax - xMin) / (N - 1);
        archivo << scientific << setprecision(10);

        for (int i = 0; i < N; i++) {
            double x = xMin + i * dx;
            double V = calcularPotencial(x, 0.0, 0.0, R, n, 3);

            if (isinf(V)) archivo << x << " NaN\n";
            else archivo << x << " " << V << "\n";
        }
        archivo.close();
        cout << "Corte R=" << R << " guardado." << endl;
    }

    string script =
        "set terminal pngcairo size 800,600\n"
        "set output '../grafica/dependencia_geometrica.png'\n"
        "set title 'Dependencia del Potencial con el Radio R (Corte y=0)'\n"
        "set xlabel 'x'\n"
        "set ylabel 'V(x)'\n"
        "set grid\n"
        "plot '../resultados/corte_R_5.dat' using 1:2 with lines title 'R = 0.5', "
        "'../resultados/corte_R_10.dat' using 1:2 with lines title 'R = 1.0', "
        "'../resultados/corte_R_20.dat' using 1:2 with lines title 'R = 2.0'\n";

    graficar(script, "../scripts/script_geometria.gnuplot", "../grafica/dependencia_geometrica.png");
}