#include "conexionesComputadora.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "rapidcsv.h"

// Jerónimo García Landa A01644050
// Liliana Camacho Suárez A01369125

namespace csv = rapidcsv;                           // namespace alias
using computer = conexiones::ConexionesComputadora; // using declarative
typedef std::set<std::string> stringSet;            // type alias
typedef std::stack<std::string> stringStack;        // type alias
typedef std::vector<std::string> stringVector;      // type alias

// forward declarations
void fillingFromCSV(const std::string& file, const unsigned& col_1, 
const unsigned& col_2, const unsigned& col_3, const unsigned& col_4);

void readingCsv(const csv::Document& csv, const unsigned& col_1, 
const unsigned& col_2, const unsigned& col_3, const unsigned& col_4);

void fillSet(const std::string& s_site, const std::string& d_site);

void fillUM(const std::string& s_ip, const std::string& s_site, 
const std::string& d_ip, const std::string& d_site);

stringSet stackToSet (stringStack stack);

/* Un namespace sin nombre usado para almacenar nuestro set de sitios y 
nuestro unordered_map.

Se usa un namespace para poder acceder a nuestras estructuras a lo largo 
de todo nuestro código sí y sólo si el namespace sin nombre sea el namespace 
padre. 

Es sin nombre porque esto hace que nuestras variable tengan una vinculación 
interna, y sean variables estáticas es decir, son intanciadas desde el inició 
de ejecución y destruidas después de que la ejecución del programa termine. */
namespace 
{
    // El prefijo "u_" quiere decir unique
    stringSet u_sites {};
    std::unordered_map<std::string, computer> u_computers;
}

void fillingFromCSV(const std::string& file, const unsigned& col_1 = 2u, 
const unsigned& col_2 = 4u, const unsigned& col_3 = 5u, const unsigned& col_4 = 7u) 
{
    const long double NaNld {std::numeric_limits<long double>::signaling_NaN()};

    // se crea el objeto que representa a un CSV
    const csv::Document csv (file, 
    csv::LabelParams(-1, -1), 
    csv::SeparatorParams(',', true, true, false, true, '"'), 
    csv::ConverterParams(false, NaNld, -1, true), 
    csv::LineReaderParams(true, '#', true));

    // se lee al objeto CSV
    readingCsv(csv, col_1, col_2, col_3, col_4);
}

void readingCsv(const csv::Document& csv, const unsigned& col_1, 
const unsigned& col_2, const unsigned& col_3, const unsigned& col_4) 
{
    // cantidad de filas (conexiones) en el CSV
    const size_t numberOfRows {csv.GetRowCount()}; 

    for (unsigned row {0}; row < numberOfRows; ++row) 
    {
        // los prefijos "s_" y "d_" significan source y destination
        // respectivamente
        const std::string s_ip = csv.GetCell<std::string>(col_1, row);   // ip fuente
        const std::string s_site = csv.GetCell<std::string>(col_2, row); // sitio fuente
        const std::string d_ip = csv.GetCell<std::string>(col_3, row);   // ip destino
        const std::string d_site = csv.GetCell<std::string>(col_4, row); // sitio destino

        // rellena al set de sitios
        fillSet(s_site, d_site);
        // rellena el unordered_map de computadoras 
        // (objetos de tipo conexionesComputadora)
        fillUM(s_ip, s_site, d_ip, d_site);

        // para comparar con las keys del unordered_map, esto será borrado
    }
};

void fillSet(const std::string& s_site, const std::string& d_site) 
{
    // los prefijos "s_" y "d_" significan source y destination respectivamente    
    // si el sitio de origen no contiene la substring ".reto.com" se inserta al set
    if (s_site.rfind(".reto.com") == std::string::npos)
    {
        ::u_sites.insert(s_site);
    }
    // si el sitio de destio no contiene la substring ".reto.com" se inserta al set
    else if (d_site.rfind(".reto.com") == std::string::npos)
    {
        ::u_sites.insert(d_site);
    }
}

void fillUM(const std::string& s_ip, const std::string& s_site, 
const std::string& d_ip, const std::string& d_site) 
{
    // los prefijos "s_", "d_" y "u_ " significan source,
    // destination y unique  respectivamente     
    stringStack s_conections {};
    stringVector d_conections {};
    
    // si aún no existe la llave s_ip, se instancia e inicializa un objeto de tipo 
    // conexionesComputadora se toma s_ip como key para insertar un key value pair 
    // de la forma: s_ip, objeto tipo conexionesComputadora
    if(::u_computers.find(s_ip) == ::u_computers.end())
    {
        computer pc {s_ip, s_site, s_conections, d_conections};

        stringVector conexionesSalientes {pc.getConexionesSalientes()};
        conexionesSalientes.push_back(d_ip);
        pc.setConexionesSalientes(conexionesSalientes);

        ::u_computers.insert({s_ip, pc});
    } 

    // si aún no existe la llave d_ip, se instancia e inicializa un objeto de tipo 
    // conexionesComputadora se toma s_ip como key para insertar un key value pair 
    // de la forma: d_ip, objeto tipo conexionesComputadora
    if (::u_computers.find(d_ip) == ::u_computers.end()) 
    {
        computer pc {d_ip, d_site, s_conections, d_conections};

        stringStack conexionesSalientes {pc.getConexionesEntrantes()};
        conexionesSalientes.push(s_ip);
        pc.setConexionesEntrantes(conexionesSalientes);

        ::u_computers.insert({d_ip, pc});
    }

    // se añade d_ip a las conexiones salientes del objeto de tipo 
    // conexionesComputadora perteneciente al key value pair cuya key sea s_ip
    stringVector conexionesSalientes {::u_computers[s_ip].getConexionesSalientes()};
    conexionesSalientes.push_back(d_ip);
    ::u_computers[s_ip].setConexionesSalientes(conexionesSalientes);

    // se añade s_ip a las conexiones entrantes del objeto de tipo 
    // conexionesComputadora perteneciente al key value pair cuya key sea d_ip
    stringStack conexionesEntrantes {::u_computers[d_ip].getConexionesEntrantes()};
    conexionesEntrantes.push(s_ip);
    ::u_computers[d_ip].setConexionesEntrantes(conexionesEntrantes);
}

// solamente retorna un conjunto que contiene los valores contenidos en un stack
stringSet stackToSet(stringStack stack) 
{
    stringSet setFromStack {};
    while (!stack.empty()) 
    {
        setFromStack.insert(stack.top());
        stack.pop();
    }
    return setFromStack;
}

stringSet mergingIps(std::vector<stringSet>& vectorOfSets) 
{
    stringSet merged;

    for (const auto& ips : vectorOfSets) 
    {
        stringSet tempSet;
        std::set_intersection(merged.begin(), merged.end(), 
        ips.begin(), ips.end(), std::inserter(tempSet, tempSet.begin()));

        merged = tempSet;
    }

    return merged;
}

int main() 
{
    const std::string filePath {"equipo11.csv"};

    // los prefijos "s_" y "d_" significan source y destination
    // respectivamente
    const unsigned s_ipsCol {2u};    // col_1
    const unsigned s_sitesCol {4u};  // col_2
    const unsigned d_ipsCol {5u};    // col_3
    const unsigned d_sitesCol {7u};  // col_4

    // Se rellenan las dos estructuras (set y unordered_map)
    // que representan a dominios que no contienen .reto.com
    // y a un conjunto de key value pairs (IP, conexionesComputadora)
    // a partir de un CSV
    fillingFromCSV(filePath);

    const std::string_view innerNetwork {"192.168.47."};
    const std::string unusualDomain_1 {"bnmp2i91bh6ed8pbje47.com"};
    const std::string unusualDomain_2 {"cbw3em4msntuaun155km.ru"};

    std::cout << "Sitios con dominios inusuales:\t" << unusualDomain_1 << '\t' 
    << unusualDomain_2 << '\n' << std::endl;

    std::cout << "IPs de los sitios con dominios inusuales:\t" << 
    ::u_computers[unusualDomain_1].getIp() << '\t';

    for (const auto& kv_pair : ::u_computers) 
    {
        if (kv_pair.second.getNombre() == unusualDomain_2) 
        {
            std::cout << kv_pair.second.getIp() << std::endl;
            break;
        }
    }

    std::cout << '\n';

    std::cout << "Sitios pertenecientes al dominio reto.com con al menos 1 conexion" 
    << " saliente: ";
    for (const auto& kv_pair : ::u_computers) 
    {
        if ((kv_pair.first.rfind(innerNetwork) != std::string::npos) && 
            (kv_pair.second.getConexionesEntrantes().size() > 0)) 
        {
            std::cout << ' ' << kv_pair.second.getIp() << '\t';
        }
    }

    std::cout << '\n';

    // Toma algunas computadoras que no sean server.reto.com o el servidor DHCP. 
    // Pueden ser entre 5 y 10. Obtén las IPs únicas de las conexiones entrantes. 
    stringSet toGmail {stackToSet(::u_computers["110.228.219.9"].getConexionesEntrantes())};
    stringSet toLinkedIn {stackToSet(::u_computers["202.241.66.149"].getConexionesEntrantes())};
    stringSet toGenius {stackToSet(::u_computers["176.16.82.142"].getConexionesEntrantes())};
    stringSet toRottenTomatoes {stackToSet(::u_computers["103.35.6.248"].getConexionesEntrantes())};
    stringSet toSpotify {stackToSet(::u_computers["175.43.209.100"].getConexionesEntrantes())};

    /* Para las IPs encontradas en el paso anterior, determina si se han comunicado 
    con los dominios inusuales. */
    std::cout << std::boolalpha;

    stringSet toUnusualDomain {stackToSet(::u_computers[unusualDomain_1].getConexionesEntrantes())};
    std::vector<stringSet> sets {toUnusualDomain, toGmail, toLinkedIn, toGenius, toRottenTomatoes, toSpotify};
    stringSet mergedIps {mergingIps(sets)};

    std::cout << "\nAlguna IP del paso anterior se han comunicado con "<< unusualDomain_1 << ": " << mergedIps.empty();

    toUnusualDomain = stackToSet(::u_computers[unusualDomain_2].getConexionesEntrantes());
    sets = {toUnusualDomain, toGmail, toLinkedIn, toGenius, toRottenTomatoes, toSpotify};
    mergedIps  = {mergingIps(sets)};

    std::cout << "\nAlguna IP del paso anterior se han comunicado con " << unusualDomain_2 << ": " << mergedIps.empty();

    /*No se podría determinar la fecha de cuando ocurre ya que la implementación 
    de conexionesComputadora no la incluye como uno de sus datos miembro, ni qué 
    protocolo se usa ya que el puerto en el que sucede la conexión tampoco es 
    incluido como dato miembro de conexionesComputadora */
    return 0;
}