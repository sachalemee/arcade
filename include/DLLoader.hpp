/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** DLLoader
*/

#ifndef DLLOADER_H
    #define DLLOADER_H

#include <dlfcn.h>
#include <stdexcept>
#include <string>

template<typename T>
class DLLoader {
    public:
        DLLoader(const std::string &path) { // load the .so 
            mHandle = dlopen(path.c_str(), RTLD_NOW); // resolve symbols
            if (!mHandle) {
                throw std::runtime_error(dlerror());
            }
        }
        ~DLLoader() { // close the .so
            if (mHandle) {
                dlclose(mHandle);
            }
        }
    private:
        void *mHandle;
};

#endif /* DLLOADER_H */
