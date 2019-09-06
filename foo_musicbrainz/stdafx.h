#pragma once

#define _WIN32_WINNT 0x0501

#define COMPONENT_TITLE "MusicBrainz Tagger"
#define COMPONENT_DLL_NAME "foo_musicbrainz.dll"
#define COMPONENT_VERSION "0.3.11"

#include <regex>

#include "json.hpp"
using json = nlohmann::json;

#include "resource.h"
#include "../foobar2000-SDK/foobar2000/helpers/foobar2000+atl.h"
#include "../foobar2000-SDK/foobar2000/helpers/atl-misc.h"
#include "../foobar2000-SDK/libPPUI/CListControlOwnerData.h"
#include "../foobar2000-SDK/libPPUI/listview_helper.h"

using namespace pfc::stringcvt;
using str8 = pfc::string8;

#include "mb_helpers.h"
#include "mb_preferences.h"