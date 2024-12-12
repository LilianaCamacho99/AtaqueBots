#ifndef CONEXIONESCOMPUTADORA_H
#define CONEXIONESCOMPUTADORA_H

#include <queue>
#include <stack>
#include <string>
#include <vector>

namespace conexiones
{
    class ConexionesComputadora
    {
        private:
        std::string ip;
        std::string nombre;
        std::stack<std::string> conexionesEntrantes;
        /* Las conexiones entrantes son aquellas donde la dirección IP de este 
        equipo se encuentra como dirección de destino */
        std::vector<std::string> conexionesSalientes;
        /* Son las conexiones hechas por esta computadora, las conexiones salientes 
        son aquellas donde la dirección IP de este equipo se encuentra 
        como dirección IP fuente. */
        public:
        ConexionesComputadora();

        ConexionesComputadora(std::string_view ip, std::string_view nombre, 
        std::stack<std::string> conexionesEntrantes, 
        std::vector<std::string> conexionesSalientes);

        void setIp(std::string_view ip);
        std::string getIp() const;

        void setNombre(std::string_view nombre);
        std::string getNombre() const;

        void setConexionesEntrantes(std::stack<std::string> conexionesEntrantes);
        std::stack<std::string> getConexionesEntrantes() const;

        void setConexionesSalientes(std::vector<std::string> conexionesSalientes);
        std::vector<std::string> getConexionesSalientes() const;       

        bool consecutiveInstances(unsigned quantity); 
    };
}

#endif