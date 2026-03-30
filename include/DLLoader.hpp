/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** DLLoader
*/

#ifndef DLLOADER_HPP
    #define DLLOADER_HPP

    #include <dlfcn.h>
    #include <memory>
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
        std::unique_ptr<T> getInstance(const std::string &symbol) { // it's the gate between main prog and dynamic lib
            char *error = nullptr;
            void *symb = nullptr;
            using create_t = T *(*)();
            
            dlerror();
            symb = dlsym(mHandle, symbol.c_str());
            error = dlerror();
            if (error != nullptr)
                throw std::runtime_error(error);
            create_t createFunction = reinterpret_cast<create_t>(symb);
            if (!createFunction)
                throw std::runtime_error("Invalid symbol cast");
            return std::unique_ptr<T>(createFunction());
        }
    private:
        void *mHandle;
};

// Evans and Marin, to expose a C function you have to use : 
// extern "C" IGame* create();
// or
// extern "C" IDisplay* create();

// You also have to :
// return new MyGame();
// because i do :
// std::unique_ptr<T>(createFunction());
// and you don't have to delete since it's a unique so it's done automatically


#endif /* DLLOADER_HPP */
