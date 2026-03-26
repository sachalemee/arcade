/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** DLLoader
*/

#ifndef DLLOADER_H
    #define DLLOADER_H

#include <string>

template<typename T>
class DLLoader {
    public:
        DLLoader(const std::string &path);
        ~DLLoader();
    private:
        void *m_handle;
};

#endif /* DLLOADER_H */
