/*
**
** Copyright 2015, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <string.h>

#include <android-base/logging.h>

#include "config.h"
#include "perfprofd_binder.h"

extern int perfprofd_main(int argc, char** argv, Config* config);

static int MainBinder() {
  android::status_t ret;
  if ((ret = android::perfprofd::PerfProfdNativeService::start()) != android::OK) {
    LOG(ERROR) << "Unable to start InstalldNativeService: %d" << ret;
    exit(1);
  }

  android::IPCThreadState::self()->joinThreadPool();

  LOG(INFO) << "Exiting perfprofd";
  return 0;
}

int main(int argc, char** argv)
{
  if (argc > 1 && strcmp(argv[1], "--binder") == 0) {
    return MainBinder();
  }

  struct PosixSleepConfig : public Config {
    void Sleep(size_t seconds) override {
      sleep(seconds);
    }
  };
  PosixSleepConfig config;
  return perfprofd_main(argc, argv, &config);
}
