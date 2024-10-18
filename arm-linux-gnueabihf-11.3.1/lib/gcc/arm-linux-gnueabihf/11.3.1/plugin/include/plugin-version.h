#include "configargs.h"

#define GCCPLUGIN_VERSION_MAJOR   11
#define GCCPLUGIN_VERSION_MINOR   3
#define GCCPLUGIN_VERSION_PATCHLEVEL   1
#define GCCPLUGIN_VERSION  (GCCPLUGIN_VERSION_MAJOR*1000 + GCCPLUGIN_VERSION_MINOR)

static char basever[] = "11.3.1";
static char datestamp[] = "20220604";
static char devphase[] = "";
static char revision[] = "[releases/gcc-11 revision 591c0f4b92548e3ae2e8173f4f93984b1c7f62bb]";

/* FIXME plugins: We should make the version information more precise.
   One way to do is to add a checksum. */

static struct plugin_gcc_version gcc_version = {basever, datestamp,
						devphase, revision,
						configuration_arguments};
