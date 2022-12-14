#include "UtilsHelper.h"

using namespace std;

vector<string> UtilsHelper::explode(string& string_to_explode,
                              const char& separator)
{
  string next;
  vector<string> result;
  for (string::const_iterator it = string_to_explode.begin(); it != string_to_explode.end(); it++) {
    if (*it == separator) {
      if (!next.empty()) {
        result.push_back(next);
        next.clear();
      }
    }
    else {
      next += *it;
    }
  }
  if (!next.empty())
    result.push_back(next);
  return result;
}

void UtilsHelper::capture_signals()
{
  signal(SIGTERM,UtilsHelper::shutdown_application);
  signal(SIGKILL,UtilsHelper::shutdown_application);
  signal(SIGSEGV,UtilsHelper::shutdown_application);
  signal(SIGABRT,UtilsHelper::shutdown_application);
  signal(SIGQUIT,UtilsHelper::shutdown_application);
  signal(SIGTSTP,UtilsHelper::shutdown_application);
  signal(SIGINT,UtilsHelper::shutdown_application);
}
void UtilsHelper::shutdown_application(int signum)
{
  spdlog::info("[main::shutdown_application] Shutting down application....");
  EvDevHelper::shutdown_buttons_leds();
  spdlog::info("[main::shutdown_application] Exiting application with code: {0}", signum);
  spdlog::info("[main::shutdown_application] Good bye!");
  exit(signum);
}
