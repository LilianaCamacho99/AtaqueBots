#include "conexionesComputadora.h"

namespace conexiones
{
    ConexionesComputadora::ConexionesComputadora()
    {
        ip = "";
        nombre = "";
    }

    ConexionesComputadora::ConexionesComputadora(std::string_view ip, 
    std::string_view nombre, std::stack<std::string> conexionesEntrantes, 
    std::vector<std::string> conexionesSalientes)
    {
        this -> ip = ip;
        this -> nombre = nombre;
        this -> conexionesEntrantes = conexionesEntrantes;
        this -> conexionesSalientes = conexionesSalientes;
    }

    void ConexionesComputadora::setIp(std::string_view ip)
    {
        this -> ip = ip;
    }

    std::string ConexionesComputadora::getIp() const
    {
        return ip;
    }

    void ConexionesComputadora::setNombre(std::string_view nombre)
    {
        this -> nombre = nombre;
    }

    std::string ConexionesComputadora::getNombre() const
    {
        return nombre;
    }

    void ConexionesComputadora::setConexionesEntrantes
    (std::stack<std::string> conexionesEntrantes)
    {
        this -> conexionesEntrantes = conexionesEntrantes;
    }

    std::stack<std::string> ConexionesComputadora::getConexionesEntrantes() const
    {
        return conexionesEntrantes;
    }

    void ConexionesComputadora::setConexionesSalientes
    (std::vector<std::string> conexionesSalientes)
    {
        this -> conexionesSalientes = conexionesSalientes;
    }

    std::vector<std::string> ConexionesComputadora::getConexionesSalientes() const
    {
        return conexionesSalientes;
    }

    bool ConexionesComputadora::consecutiveInstances(unsigned instances)
    {
        int counter {1};
        std::string momentaryIp {conexionesSalientes[0]};

        for (std::string ip : conexionesSalientes)
        {
            if (momentaryIp == ip)
            {
                ++counter;
                if (counter == instances)
                {
                    break;
                }
            }
            else
            {
                momentaryIp = ip;
                counter = 1;
            }
        }

        return (counter == instances);
    }
}