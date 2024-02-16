/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** debug
*/

#ifndef DEBUG_H_
    #define DEBUG_H_

    #define NOTHING /* Nothing */

    #ifdef DEBUG_MODE
        #define FORMAT(s) "%s:%d: " s "\n"
        #define HEAD __FILE_NAME__, __LINE__

        #define DEBUG(fmt, ...) (printf(FORMAT(fmt), HEAD, __VA_ARGS__))
        #define DEBUGV(fmt, va) (vprintf(FORMAT(fmt), HEAD, (va)))
        #define DEBUG_MSG(string) (printf(FORMAT(string), HEAD))
        #define DEBUG_DO(code) (code)
    #else
        #define DEBUG_DO(code) NOTHING
        #define DEBUG(...) NOTHING
        #define DEBUGV(...) NOTHING
        #define DEBUG_MSG(...) NOTHING
    #endif

#endif /* DEBUG_H_ */
