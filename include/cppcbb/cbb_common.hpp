//Copyright(C) 2020 Henry Bullingham
//This file is subject to the license terms in the LICENSE file
//found in the top - level directory of this distribution.

#pragma once

#if !defined (CPPCBB_INCLUDE_CBB_COMMON_H)
#define CPPCBB_INCLUDE_CBB_COMMON_H


/*

Some error handling configuration

*/

#define CPPCBB_STRINGIFY_IMPL(x_) #x_
#define CPPCBB_STRINGIFY(x_) CPPCBB_STRINGIFY_IMPL(x_)
#define CPPCBB_LINE_STRING CPPCBB_STRINGIFY(__LINE__)
#define CPPCBB_ASSERTION_STRING(str_) "Assertion Failed in File " __FILE__ " at line " CPPCBB_LINE_STRING "" str_ 


#if defined(CPPCBB_USE_EXCEPTIONS)

#include <exception>

#define CPPCBB_ASSERT(expr_, msg_) if (expr_) {} else { throw std::runtime_error(CPPCBB_ASSERTION_STRING(msg_)); }

#elif defined(CPPCBB_USE_ASSERTS)

#include <cassert>

#define CPPCBB_ASSERT(expr_, msg_) assert((expr_) && CPPCBB_ASSERTION_STRING(msg_))

#else

#define CPPCBB_ASSERT(expr_, msg_) if(expr_){} else {}

#endif

/*

    Useful types in this file

*/

namespace cppcbb
{
    /// <summary>
    /// Traits for the specific element type
    /// </summary>
    /// <typeparam name="Elem"></typeparam>
    template<typename Elem>
    class default_traits;

    
}

/* 

    Implementation section

*/

namespace cppcbb
{
    template<typename Elem>
    class default_traits
    {
    public:
        using iterator = Elem*;
        using const_iterator = Elem const*;
    };
}

#endif //CPPCBB_INCLUDE_CBB_COMMON_H