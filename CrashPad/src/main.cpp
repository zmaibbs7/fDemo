#include <client/crash_report_database.h>
#include <client/settings.h>
#include <client/crashpad_client.h>

bool InitializeCrashpad()
{
  // Cache directory that will store crashpad information and minidumps
  base::FilePath database("InitializeCrashpad");
  // Path to the out-of-process handler executable
  base::FilePath handler("/home/zmaibbs/Codes/fDemo/CrashPad/build/crashpad_handler");
  // URL used to submit minidumps to
  std::string url("https://error-handler.local");
  // Optional annotations passed via --annotations to the handler
  std::map<std::string, std::string> annotations;
  // Optional arguments to pass to the handler
  std::vector<std::string> arguments;

  crashpad::CrashpadClient client;
  bool success = client.StartHandler(
    handler,
    database,
    database,
    url,
    annotations,
    arguments,
    /* restartable */ true,
    /* asynchronous_start */ false);
  
  return success;
}

#include "AAA.h"
int main()
{
	InitializeCrashpad();


	A a;
	a.test();

	return 0;
}