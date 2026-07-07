#include <coreinit/dynload.h>

extern "C"
int rpl_entry(OSDynLoad_Module module, OSDynLoad_EntryReason reason)
{
    return 0;
}