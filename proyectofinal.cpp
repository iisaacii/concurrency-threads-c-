/* Martinez Garcia Isaac
UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
FACULTAD INGENIERIA 
SISTEMAS OPERATIVOS
PROYECTO FINAL : Pequeño programa donde podemos apreciar la concurrencia de los hilos al comunicarse entre sí, usamos
mutex como un semaforo para probar la correcta sincronización y el clásico sleep para poder diferenciar de mejor manera
la concurrencia*/
//COMPILAR CON: g++ -std=c++11 proyectofinal.cpp -o proyectofinal
//CORRER CON:./proyectofinal

#include <iostream>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <list>
#include <iterator>
#include <vector>
#include <mutex>   

using namespace std;
using namespace std::this_thread;
std::mutex mtx; 

int num_clientes=0;
int num_cocineros=0;
int num_meseros=0;
int num_platillos=5;
int mesa_asignada=0;
int orden=0;
int restante=0;
int charola=0;

list<int> mesa;
list<int> platillo;
list<int> total;

void inserta(){
    for (int i = 1; i < 11; ++i){ mesa.push_back(i);}
}

void fifoplatillos(int p){
    for (int i = 1; i < p+1; ++i){ platillo.push_back(i);}
}

void showlist(list <int> g)
{
    list <int> :: iterator it;
    for(it = g.begin(); it != g.end(); ++it)
        cout << '\t' << *it;
    cout << '\n';
}

void recorrer(list <int> g)
{
    list <int> :: iterator it;
    for(it = g.begin(); it != g.end(); ++it)
        cout << '\t';
    cout << '\n';
}

void sumlist(list <int> g){
    int sum=0;
    list <int> :: iterator it;
        for(it = g.begin(); it != g.end(); ++it)
        sum+=*it;
        cout << "\t TOTAL RESTAURANTE:" << sum;
}

void cliente(){
    
    while (true)
    {
        if (mesa_asignada!=0)
        {   mtx.lock();
            cout<<"Cliente: Se me asignó la mesa: "<<mesa_asignada<<endl;
            mtx.unlock();
            //sleep(1);
            cout<<"Cliente: Procederé a pedir "<<num_platillos<<" platillos"<<endl;
            fifoplatillos(num_platillos);
            orden=1;
            sleep(1);
            mesa_asignada=0;
        }
        
        if (restante!=0 & platillo.size()==0)
        {   
            cout<<"Cliente: CONSUMIENDO, ESPERANDO 5 SEGUNDOS..."<<endl;
            sleep(5);
            charola=num_platillos*10;
            cout<<"Total charola: "<<charola<<endl;
            total.push_back(charola);
            sleep(1);
            recorrer(total);
            sumlist(total);
            sleep(5);
            restante=0;
            
            
        }
        
    }
}

void mesero(){
    mtx.lock();
    cout<<"Mesero: En breve te asignaré una mesa..."<<endl;
    mesa_asignada=mesa.front();
    mesa.pop_front();
    //sleep(1);
    cout<<"Mesero: Se asignó la mesa: "<<mesa_asignada<<endl;
    mtx.unlock();
    while (true)
    {
        if (orden!=0)
        {   sleep(1);
            cout<<"Mesero: Orden recabada, gracias"<<endl;
            orden=0;
        }
        if (restante!=0 & platillo.size()>0)
        {   
            cout<<"Mesero: Entregando comida"<<endl;
            platillo.pop_back();
            sleep(3);
        }
        
    }

}

void cocinero(){
    while (true)
    {
        if (orden!=0)
        {   sleep(2);
            cout<<"Cocinero: Elaborando la orden solicitada"<<endl;
            cout<<"ESPERANDO 5 SEGUNDOS EN LA ELABORACIÓN..."<<endl;
            sleep(5);
        }
        else if (platillo.size()>0)
        {
            
            cout<<"Platos por entregar:  ";
            showlist(platillo);
            restante=1;
            
        }
        
        sleep(2);
        
    }   
    
}

void hilos(){
    mtx.lock();
    inserta();
    thread m1(mesero);
    mtx.unlock();
    //sleep(1);
    mtx.lock();
    thread c1(cliente);
    mtx.unlock();
    //sleep(1);
    mtx.lock();
    thread co1(cocinero);
    mtx.unlock();
    m1.join();
    c1.join();
    co1.join();
    
}


int main(){

  cout << "Bienvenido al restaurante, administración recabará un poco de información\n";
    sleep(1);
    
    cout << "Ingrese número de clientes\n";
    cin >> num_clientes;
    cout << "Ingrese número de meseros disponibles\n";
    cin >> num_meseros;
    cout << "Ingrese número de cocineros disponibles\n";
    cin >> num_cocineros;
    cout << "El número de platillos máximo por persona son 5...Reglas de la casa.\n\n";
    
    std::vector<std::thread>threads;

    for (int i = 0; i < num_clientes; i++)
    {   
        threads.push_back(std::thread(hilos));
        //sleep(1);
    }

    for (auto &th:threads)
    {
        th.join();
    }
    
}