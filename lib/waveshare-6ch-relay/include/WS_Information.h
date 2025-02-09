#ifndef _WS_INFORMATION_H_
#define _WS_INFORMATION_H_

#define PROJECT_CONFIGURATION_HEADER_FILE "../../../project_config.h"

#if __has_include(PROJECT_CONFIGURATION_HEADER_FILE)
#include PROJECT_CONFIGURATION_HEADER_FILE
#else
#error "You need to create a configuration file (e.g., " PROJECT_CONFIGURATION_HEADER_FILE ")"
#endif

#endif // _WS_INFORMATION_H_