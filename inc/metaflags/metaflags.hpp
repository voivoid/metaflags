#pragma once

#include "boost/hana/ap.hpp"
#include "boost/hana/chain.hpp"
#include "boost/hana/greater_equal.hpp"
#include "boost/hana/if.hpp"
#include "boost/hana/integral_constant.hpp"
#include "boost/hana/intersection.hpp"
#include "boost/hana/length.hpp"
#include "boost/hana/optional.hpp"
#include "boost/hana/set.hpp"
#include "boost/hana/union.hpp"

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
  static_assert( sizeof...( values ) > 0 );

  template <auto... flags>
  static constexpr auto matched_opt()
  {
    const auto flags_set = details::make_flags_set<flags...>();

    const auto values_set   = boost::hana::make_set( boost::hana::integral_c<decltype( values ), values>... );
    const auto intersection = boost::hana::intersection( values_set, flags_set );
    const auto matched      = T::template is_matched( intersection );

    return boost::hana::if_( matched, boost::hana::just( intersection ), boost::hana::nothing );
  }
};

struct one_of
{
  template <typename M>
  static constexpr auto is_matched( const M match_set )
  {
    return boost::hana::length( match_set ) == boost::hana::size_c<1>;
  }
};

struct one_or_more
{
  template <typename M>
  static constexpr auto is_matched( const M match_set )
  {
    return boost::hana::length( match_set ) >= boost::hana::size_c<1>;
  }
};

struct zero_or_more
{
  template <typename M>
  static constexpr auto is_matched( const M match_set )
  {
    return boost::hana::true_c;
  }
};

}  // namespace details

template <typename rule, typename... rules>
struct and_t_
{
  template <auto... flags>
  static constexpr auto matched_opt()
  {
    const auto matched_set_opt = rule::template matched_opt<flags...>();
    const auto rest_matched    = and_t_<rules...>::template matched_opt<flags...>();

    const auto union_matches = [matched_set_opt, rest_matched]( const auto s1, const auto s2 ) {
      return boost::hana::union_( s1, s2 );
    };

    return boost::hana::ap( boost::hana::just( union_matches ), matched_set_opt, rest_matched );
  }
};

template <typename rule>
struct and_t_<rule>
{
  template <auto... flags>
  static constexpr auto matched_opt()
  {
    return rule::template matched_opt<flags...>();
  }
};

template <typename rule, typename... rules>
struct or_t_
{
  template <auto... flags>
  static constexpr auto matched_opt()
  {
    const auto matched_set_opt = rule::template matched_opt<flags...>();
    return boost::hana::just( matched_set_opt.value_or( or_t_<rules...>::template matched_opt<flags...>() ) );
  }
};

template <typename rule>
struct or_t_<rule>
{
  template <auto... flags>
  static constexpr auto matched_opt()
  {
    return rule::template matched_opt<flags...>();
  }
};

template <typename rule>
struct optional_t_
{
  template <auto... flags>
  static constexpr auto matched_opt()
  {
    const auto matched_set_opt = rule::template matched_opt<flags...>();

    return boost::hana::just( matched_set_opt.value_or( boost::hana::make_set() ) );
  }
};

template <auto... values>
using one_of_ = details::base<details::one_of, values...>;

template <auto... values>
using any_of_ = details::base<details::zero_or_more, values...>;

template <typename meta_enum, auto... flags>
constexpr bool check()
{
  const auto flags_set = details::make_flags_set<flags...>();
  const auto matched_set_opt = meta_enum::template matched_opt<flags...>();
  const auto are_all_flags_matched = [flags_set]( const auto matched_set ) { return boost::hana::length( flags_set ) == boost::hana::length( matched_set ); };

  return boost::hana::maybe( false, are_all_flags_matched, matched_set_opt );
}

template <typename meta_enum, auto... flags>
constexpr auto make()
{
  static_assert( check<meta_enum, flags...>() );
  return ( flags | ... );
}

}  // namespace meta_flags
