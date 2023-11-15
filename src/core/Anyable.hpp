
#pragma once

#include <cassert>
#include <functional>
#include <map>
#include <sstream>

#include "core/Macros.hpp"
// #include "core/std_any.hpp"
// #include "core/Input.hpp"
// #include "core/Output.hpp"
// #include "core/io/input/InputBase.hpp"
// #include "core/io/output/OutputBase.hpp"
//#include "core/Input.hpp"
//#include "core/Output.hpp"
#include "core/Serializer.hpp"
//#include "core/serializer/SerializerI.hpp"

#if CPLUSPLUS_VERSION <= 14
#    include "std_any.hpp"
#else
#    include <any>
#endif

namespace hub {

class Anyable
{

  private:
    class AnyHelper
    {
        //  private:
      public:
//        AnyHelper() = delete;
//        AnyHelper(AnyHelper &&) = delete;
//        AnyHelper(const AnyHelper &) = delete;

        //    template <typename T>
        //    Anyable( const T* t )
        //    {
        //        getTypeName = [](  ) {
        ////            return std::string("const ") + TYPE_NAME(T) + " *";
        //            return std::string(TYPE_NAME(T)) + "*";
        //        };

        //        getValueStr = []( const std::any& any ) {
        //            assert( typeid( const T* ) == any.type() );
        //            const T* val = std::any_cast<const T*>( any );
        //            std::stringstream sstream;
        //            sstream << val;
        //            return sstream.str();
        //        };
        //        compare = []( const std::any& any, const std::any& any2 ) {
        //            return !strcmp( std::any_cast<const T*>( any ), std::any_cast<const T*>( any2
        //            ) );
        //        };
        //    }

        template <class T>
        void setup() {
            //            std::cout << "[Anyable] setup type : " << TYPE_NAME(T) << std::endl;

            getTypeName = []() { return TYPE_NAME( T ); };

            // void
            if constexpr ( std::is_same_v<T, void> ) {
                getValueStr = []( const std::any& ) { return "nil"; };
                write       = []( Serializer&, const std::any& ) {};
                read        = []( Serializer&, std::any& ) {};
                compare     = []( const std::any& any, const std::any& any2 ) {
                    if ( any.type() == typeid( void ) || any2.type() == typeid( void ) ) {
                        return any.type() == any2.type();
                    }
                    return false;
                };
            }
            else {
                getValueStr = []( const std::any& any ) {
                    assert( typeid( T ) == any.type() );
                    const T& val = std::any_cast<const T&>( any );
                    std::stringstream sstream;
                    sstream << val;
                    return sstream.str();
                };
                // const char *
                if constexpr ( std::is_same_v<T, const char*> ) {
                    write = []( Serializer& output, const std::any& any ) {
                        const char* val = std::any_cast<const char*>( any );
                        output.write( val );
                    };
                    read = []( Serializer& input, std::any& any ) {
                        char* val =
                            new char[80]; // leak, please do not use char *, use std::string instead
                        input.read( val );
                        any = (const char*)val;
                    };
                    compare = []( const std::any& any, const std::any& any2 ) {
                        return strcmp( std::any_cast<const char*>( any ),
                                       std::any_cast<const char*>( any2 ) ) == 0;
                    };
                }
                // others
                else {
                    write = []( Serializer& output, const std::any& any ) {
                        const T& val = std::any_cast<const T&>( any );
                        output.write( val );
                    };
                    read = []( Serializer& input, std::any& any ) {
                        T t;
                        input.read( t );
                        any = t;
                    };
                    compare = []( const std::any& any, const std::any& any2 ) {
                        if ( any.type() == typeid( void ) || any2.type() == typeid( void ) ) {
                            return any.type() == any2.type();
                        }
                        return std::any_cast<const T&>( any ) == std::any_cast<const T&>( any2 );
                    };
                }
            }
        }

        friend class Anyable;
        friend class Any;
      private:
        std::function<std::string()> getTypeName;
        std::function<std::string( const std::any& )> getValueStr;
        std::function<bool( const std::any&, const std::any& )> compare;
        //        std::function<void( io::output::OutputBase& output, const std::any& )> write;
//        std::function<void( Output&, const std::any& )> write;
        std::function<void( Serializer&, const std::any& )> write;
        std::function<void( Serializer&, std::any& )> read;
//        std::function<void( Input&, std::any& )> read;
    };

    template <class T>
    static AnyHelper make_anyHelper() {
        AnyHelper ret;
        ret.setup<T>();
        return ret;
    }

    template <class T>
    static std::pair<size_t, AnyHelper> makeAnyHelperRow() {
        return std::make_pair( typeid( T ).hash_code(), make_anyHelper<T>() );
    }

    // static std::map<size_t, AnyHelper> s_anyables = makeAnyHelperMap<double>();

    //    static std::map<size_t, AnyHelper> s_anyables;
    template <class... T>
    static std::map<size_t, AnyHelper> makeAnyHelperMap() {
        return { makeAnyHelperRow<T>()... };
        //    return std::map<size_t, AnyHelper>{ makeRow<T>()};
    }

    static std::map<size_t, AnyHelper> s_anyables;

    //    static std::vector<std::string> supportedTypes();

    template <class T>
    static void insertSupportedType() {
        assert( s_anyables.find( typeid( T ).hash_code() ) == s_anyables.end() );
        s_anyables.insert( makeAnyHelperRow<T>() );
        assert( s_anyables.find( typeid( T ).hash_code() ) != s_anyables.end() );
    }
    template <class T, class... Ts>
    static void insertSupportedTypes() {
        insertSupportedType<T>();
        if constexpr ( sizeof...( Ts ) > 0 ) { insertSupportedTypes<Ts...>(); }
        //        assert( s_anyables.find( typeid( T ).hash_code() ) == s_anyables.end() );
        //        s_anyables.insert( makeAnyHelperRow<T>() );
        //        assert( s_anyables.find( typeid( T ).hash_code() ) != s_anyables.end() );
    }

  public:
    static std::vector<std::string> supportedTypes() {
        std::vector<std::string> ret;
        for ( const auto& pair : s_anyables ) {
            ret.push_back( "<" + pair.second.getTypeName() + ">" );
            //        std::cout << pair.first << " -> " << pair.second.getTypeName() << std::endl;
        }
        return ret;
    }

    template <class... Ts>
    static void registerTypes() {
        insertSupportedTypes<Ts...>();
        //        insertSupportedType<Ts...>();
        //        if constexpr ( sizeof...( Ts ) > 0 ) { addSupportedType<Ts...>(); }
        //        assert( s_anyables.find( typeid( T ).hash_code() ) == s_anyables.end() );
        //        s_anyables.insert( makeAnyHelperRow<T>() );
        //        assert( s_anyables.find( typeid( T ).hash_code() ) != s_anyables.end() );
    }

    friend class Any;
    //    friend class std::ostream;

}; // class Anyable

} // namespace hub
