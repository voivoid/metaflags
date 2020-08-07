#include <windows.h>
#include "metaflags/metaflags.hpp"

namespace mf = meta_flags;

namespace MsgBoxFlags
{
  using Buttons = 
    mf::and_t_<
      mf::one_of_<MB_OK, MB_OKCANCEL, MB_ABORTRETRYIGNORE, MB_YESNOCANCEL, MB_YESNO, MB_RETRYCANCEL, MB_CANCELTRYCONTINUE>,
      mf::any_of_<MB_HELP>
    >;

  using DefButtons =
    mf::one_of_<MB_DEFBUTTON1, MB_DEFBUTTON2, MB_DEFBUTTON3, MB_DEFBUTTON4>;

  using Modality =
    mf::one_of_<MB_APPLMODAL, MB_SYSTEMMODAL, MB_TASKMODAL>;

  using Misc =
    mf::any_of_<MB_DEFAULT_DESKTOP_ONLY, MB_RIGHT, MB_RTLREADING, MB_SETFOREGROUND, MB_TOPMOST, MB_SERVICE_NOTIFICATION>;

  using Rules = mf::and_t_<Buttons,
                           mf::optional_t_<DefButtons>,
                           mf::optional_t_<Modality>,
                           mf::optional_t_<Misc>>;
}



void test()
{
  mf::make<MsgBoxFlags::Rules, MB_OKCANCEL, MB_HELP, MB_DEFBUTTON2, MB_SYSTEMMODAL, MB_SETFOREGROUND, MB_TOPMOST>();
}
