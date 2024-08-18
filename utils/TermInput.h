#ifndef TERMINPUT_H
#define TERMINPUT_H

#include <stdio.h>
#include <string>

#ifdef _WIN32
#else
    void initTermios(int echo);
    void resetTermios(void);

    char getch_(int echo);
    char getch(void);
    char getche(void);
#endif

std::string ingresarCadena(bool o = false);
std::string ingresarNumero();
std::string ingresarCedula();
int ingresarEntero();
float ingresarFlotante();

#endif
