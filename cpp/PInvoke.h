#include <cstdint>
#if defined(_MSC_VER)
    //  Microsoft 
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    //  GCC
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    //  do nothing and hope for the best?
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif
// typedef void (*CopyMonsterCallback)(uint8_t*, int);
using CopyMonsterCallback = void (*)(uint8_t*, int);

extern "C"
{
    EXPORT int Add(int a, int b);
    EXPORT void CreateTestMonster(CopyMonsterCallback cb);
    EXPORT void PrintMonster(uint8_t* buf);
}