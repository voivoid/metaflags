#include "metaflags/metaflags.hpp"

#include <cassert>

namespace {

enum Flags : int
{
    Read, Write, Cached, NonCached
};

namespace mf = meta_flags;

using and_flags = mf::and_t_<
                            mf::one_of_< Read, Write >,
                            mf::one_of_< Cached, NonCached >
                        >;

using or_flags = mf::or_t_<
                            mf::one_of_< Read, Write >,
                            mf::one_of_< Cached >
                        >;

using optional_flags = mf::and_t_<
  mf::one_of_< Read, Write >,
  mf::optional_t_<mf::one_of_< Cached, NonCached >>
>;

  static_assert( mf::check< optional_flags, Read >() );
  static_assert( mf::check< optional_flags, Read, Cached >() );
  static_assert( mf::check< optional_flags, Read, NonCached >() );
  static_assert( !mf::check< optional_flags, Read, Cached, NonCached >() );

  static_assert( mf::check< and_flags, Read, Cached >() );
  static_assert( mf::check< and_flags, Read, NonCached >() );
  static_assert( mf::check< and_flags, Write, Cached >() );
  static_assert( mf::check< and_flags, Write, NonCached >() );  
  static_assert( !mf::check< and_flags, Read, Cached, NonCached >() );
  static_assert( !mf::check< and_flags, Read, Cached, Write >() );
  static_assert( !mf::check< and_flags, Read, Cached, Write, NonCached >() );

  static_assert( mf::check< or_flags, Read >() );
  static_assert( mf::check< or_flags, Write >() );
  static_assert( mf::check< or_flags, Cached >() );
  static_assert( !mf::check< or_flags, NonCached >() );

  static_assert( mf::check< mf::any_of_<Read, Write, Cached>, Read>() );
  static_assert( mf::check< mf::any_of_<Read, Write, Cached>, Write>() );
  static_assert( mf::check< mf::any_of_<Read, Write, Cached>, Cached>() );  
  static_assert( mf::check< mf::any_of_<Read, Write, Cached>, Read, Write, Cached>() );
  static_assert( !mf::check< mf::any_of_<Read, Write, Cached>, NonCached>() );

  
  static_assert( mf::check< mf::one_of_<Read, Write, Cached>, Read>() );
  static_assert( mf::check< mf::one_of_<Read, Write, Cached>, Write>() );
  static_assert( mf::check< mf::one_of_<Read, Write, Cached>, Cached>() );  

  static_assert( !mf::check< mf::one_of_<Read, Write, Cached>, Read, Write>() );
  static_assert( !mf::check< mf::one_of_<Read, Write, Cached>, Read, Write, Cached>() );
  static_assert( !mf::check< mf::one_of_<Read, Write, Cached>, NonCached>() );
  static_assert( !mf::check< mf::one_of_<Read, Write, Cached>, Read, NonCached>() );
}

int main() {

    //check<flags_meta_enum, Read, Cached>();
    //check<flags_meta_enum, Read, NonCached>();
    //check<flags_meta_enum, Write, Cached>();
    //check<flags_meta_enum, Write, NonCached>();
}