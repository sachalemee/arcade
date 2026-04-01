/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** main
*/

#include <cstring>
#include <dlfcn.h>
#include <memory>
#include "Core.hpp"
#include "LibManager.hpp"
#include "arcade.hpp"

int main(int ac, char **av)
{
    LibManager libManager;
    std::unique_ptr<Core> core;

    if (error_handling(ac, av, libManager) == EXITFAILURE)
        return EXITFAILURE;
    core = std::make_unique<Core>(av[1]);
    return core->run();
}
