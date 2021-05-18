#pragma once

// Compile Time String Encryption
// Prevent strings from being stored in rdata/data
// Inline strings in .text and store these on the stack when decrypting
// After the string is decreypted and used, it disappears right away
// How to use: XorString("String")
// Purpose: Prevent generic scan to trigger in case hooks fail
// Initally wrote by LeFF (for malwares)

#define CompileTimeAsSeed       ((__TIME__[7] - '0') * 1  + (__TIME__[6] - '0') * 10  + \
                                (__TIME__[4] - '0') * 60   + (__TIME__[3] - '0') * 600 + \
                                (__TIME__[1] - '0') * 3600 + (__TIME__[0] - '0') * 36000)

template <int X> struct EnsureCompileTime {
    enum : int {
        Value = X
    };
};

constexpr int LinearCongruentGenerator(int Rounds) {
    return 1013904223 + 1664525 * ((Rounds > 0) ? LinearCongruentGenerator(Rounds - 1) : CompileTimeAsSeed & 0xFFFFFFFF);
}

#define Random() EnsureCompileTime<LinearCongruentGenerator(10)>::Value
#define RandomNumber(Min, Max) (Min + (Random() % (Max - Min + 1)))

template <int... Pack> struct IndexList {};

template <typename IndexList, int Right> struct Append;
template <int... Left, int Right> struct Append<IndexList<Left...>, Right> {
    typedef IndexList<Left..., Right> Result;
};

template <int N> struct ConstructIndexList {
    typedef typename Append<typename ConstructIndexList<N - 1>::Result, N - 1>::Result Result;
};
template <> struct ConstructIndexList<0> {
    typedef IndexList<> Result;
};

const char XORKEY = static_cast<char>(RandomNumber(0, 0xFF));
constexpr char EncryptCharacter(const char Character, int Index) {
    return Character ^ (XORKEY + Index);
}

template <typename IndexList> class CXorString;
template <int... Index> class CXorString<IndexList<Index...> > {
private:
    char Value[sizeof...(Index) + 1];
public:
    constexpr CXorString(const char* const String)
        : Value{ EncryptCharacter(String[Index], Index)... } {}

    char* decrypt() {
        for (int t = 0; t < sizeof...(Index); t++) {
            Value[t] = Value[t] ^ (XORKEY + t);
        }
        Value[sizeof...(Index)] = '\0';
        return Value;
    }

    char* get() {
        return Value;
    }
};

#define XorS(X, String) CXorString<ConstructIndexList<sizeof(String)-1>::Result> X(String)
#define XorString( String ) ( CXorString<ConstructIndexList<sizeof( String ) - 1>::Result>( String ).decrypt() )