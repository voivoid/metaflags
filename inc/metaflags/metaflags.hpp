#pragma once

#include "boost/hana/set.hpp"
#include "boost/hana/integral_constant.hpp"
#include "boost/hana/intersection.hpp"
#include "boost/hana/length.hpp"
#include "boost/hana/greater_equal.hpp"
#include "boost/hana/union.hpp"

#include <optional>

namespace meta_flags
{
  namespace details
  {
    template <auto... flags>
    constexpr auto make_flags_set()
    {
      return boost::hana::make_set( boost::hana::integral_c<decltype( flags ), flags>... );
    }

    template <typename T, auto... values>
    struct base
    {
      static_assert( sizeof...(values) > 0 );

      template <auto... flags>
      static constexpr auto matched_opt()
      {
        constexpr auto flags_set = details::make_flags_set<flags...>();

        constexpr auto values_set = boost::hana::make_set( boost::hana::integral_c<decltype(values), values>... );
        constexpr auto intersection = boost::hana::intersection( values_set, flags_set );
        constexpr bool matched = T::template is_matched( intersection );

        using opt_match = std::optional< decltype( intersection ) >;
        return matched ? opt_match{ intersection } : opt_match{};;
      }
    };

    struct one_of
    {
      template <typename M>
      static constexpr bool is_matched( const M match_set )
      {
        return boost::hana::length( match_set ) == boost::hana::size_c<1>;
      }
    };

    struct one_or_more
    {
      template <typename M>
      static constexpr bool is_matched( const M match_set )
      {
        return boost::hana::length( match_set ) >= boost::hana::size_c<1>;
      }
    };

    struct zero_or_more
    {
      template <typename M>
      static constexpr bool is_matched( const M match_set )
      {
        return true;
      }
    };

  }

  template <typename rule, typename... rules>
  struct and_t_
  {
    template <auto... flags>
    static constexpr auto matched_opt()
    {
      constexpr auto matched_set_opt = rule::template matched_opt<flags...>();
      constexpr auto rest_matched = and_t_<rules...>::template matched_opt<flags...>();
      if constexpr ( matched_set_opt.has_value() && rest_matched.has_value() )
      {
        constexpr auto matched = boost::hana::union_( matched_set_opt.value(), rest_matched.value() );
        return std::optional{ matched };
      }
      else
      {
        return std::optional<decltype(boost::hana::make_set())>{};
      }
    }
  };

  template <typename rule>
  struct and_t_<rule>
  {
    template <auto... flags>
    static constexpr auto matched_opt()
    {
      return rule:: template matched_opt<flags...>();
    }
  };

  template <typename rule, typename... rules>
  struct or_t_
  {
    template <auto... flags>
    static constexpr auto matched_opt()
    {
      constexpr auto matched_set_opt = rule::template matched_opt<flags...>();
      if constexpr ( matched_set_opt.has_value() )
      {
        return matched_set_opt;
      }
      else
      {
        return or_t_<rules...>::template matched_opt<flags...>();
      }      
    }
  };

  template <typename rule>
  struct or_t_<rule>
  {
    template <auto... flags>
    static constexpr auto matched_opt()
    {
      return rule:: template matched_opt<flags...>();
    }
  };

  template <typename rule>
  struct optional_t_
  {
    template <auto... flags>
    static constexpr auto matched_opt()
    {
      constexpr auto matched_set_opt = rule::template matched_opt<flags...>();
      if constexpr ( matched_set_opt.has_value() )
      {
        return matched_set_opt;
      }
      else
      {
        return std::optional<decltype( boost::hana::make_set() )>{ {} };
      }
    }
  };

  template <auto... values> using one_of_ = details::base<details::one_of, values...>;
  template <auto... values> using any_of_ = details::base<details::zero_or_more, values...>;

  template <typename meta_enum, auto... flags>
  constexpr bool check()
  {
    constexpr auto flags_set = details::make_flags_set<flags...>();

    constexpr auto matched_set_opt = meta_enum::template matched_opt<flags...>();
    if constexpr ( matched_set_opt.has_value() )
    {
      return boost::hana::length( flags_set ) == boost::hana::length( matched_set_opt.value() );
    }
    else
    {
      return false;
    }
  }

  template <typename meta_enum, auto... flags>
  constexpr auto make()
  {
    static_assert( check<meta_enum, flags...>() );
    return ( flags | ... );
  }

}