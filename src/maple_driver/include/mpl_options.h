/*
 * Copyright (c) [2019] Huawei Technologies Co.,Ltd.All rights reserved.
 *
 * OpenArkCompiler is licensed under the Mulan PSL v1.
 * You can use this software according to the terms and conditions of the Mulan PSL v1.
 * You may obtain a copy of Mulan PSL v1 at:
 *
 *     http://license.coscl.org.cn/MulanPSL
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v1 for more details.
 */
#ifndef MAPLE_DRIVER_INCLUDE_MPL_OPTIONS_H
#define MAPLE_DRIVER_INCLUDE_MPL_OPTIONS_H
#include <map>
#include <set>
#include <stdio.h>
#include <string>
#include <tuple>
#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "file_utils.h"
#include "option_parser.h"
#include "mpl_logging.h"

namespace maple {
enum InputFileType {
  kNone,
  kClass,
  kJar,
  kMpl,
  kVtableImplMpl,
  kS,
};

enum OptimizationLevel { kO0, kO1, kO2 };
enum RunMode { kAutoRun, kCustomRun, kUnkownRun };

class MplOption {
 public:
  MplOption(const std::string &key, const std::string &value, const std::string &connectSymbol, bool isAppend,
            const std::string &appendSplit, bool needRootPath = false)
      : key(key),
        value(value),
        connectSymbol(connectSymbol),
        isAppend(isAppend),
        appendSplit(appendSplit),
        needRootPath(needRootPath) {
    CHECK_FATAL(!key.empty(), "MplOption got an empty key.");
  }

  const std::string &GetKey() const {
    return key;
  }

  const std::string &GetValue() const {
    return value;
  }

  void SetValue(std::string val) {
    value = val;
  }

  const std::string &GetconnectSymbol() const {
    return connectSymbol;
  }

  bool GetIsAppend() const {
    return isAppend;
  }

  const std::string &GetAppendSplit() const {
    return appendSplit;
  }

  bool GetNeedRootPath() const {
    return needRootPath;
  }

 private:
  // option key
  std::string key;
  // option value
  std::string value;
  std::string connectSymbol;
  // true --- append user option to default option with appendSplit.
  // false --- override default option with user option.
  bool isAppend;
  std::string appendSplit;
  bool needRootPath;
};

struct DefaultOption {
  MplOption *mplOptions;
  uint32_t length;
};

class MplOptions {
 public:
  MplOptions()
      : optionParser(nullptr),
        options({}),
        exeOptions({}),
        inputFiles(""),
        inputFolder(""),
        outputFolder(""),
        outputName("maple"),
        exeFolder(""),
        inputFileType(InputFileType::kNone),
        optimizationLevel(OptimizationLevel::kO0),
        runMode(RunMode::kUnkownRun),
        setDefaultLevel(false),
        isSaveTmps(false),
        saveFiles({}),
        splitsInputFiles({}),
        extras({}),
        runningExes({}),
        printCommandStr(""),
        debugFlag(false),
        timePhases(false),
        genMemPl(false),
        genVtableImpl(false),
        verify(false) {}
  ~MplOptions() = default;

  int Parse(int argc, char **argv);
  const std::string OptimizationLevelStr() const;

  const std::map<std::string, std::vector<mapleOption::Option>> &GetOptions() const {
    return options;
  }

  const std::map<std::string, std::vector<mapleOption::Option>> &GetExeOptions() const {
    return exeOptions;
  }

  const std::string &GetInputFiles() const {
    return inputFiles;
  }

  const std::string &GetOutputFolder() const {
    return outputFolder;
  }

  const std::string &GetOutputName() const {
    return outputName;
  }

  const std::string &GetExeFolder() const {
    return exeFolder;
  }

  const InputFileType &GetInputFileType() const {
    return inputFileType;
  }

  const OptimizationLevel &GetOptimizationLevel() const {
    return optimizationLevel;
  }

  const bool GetSetDefaultLevel() const {
    return setDefaultLevel;
  }

  const bool GetIsSaveTmps() const {
    return isSaveTmps;
  }

  const std::vector<std::string> &GetSaveFiles() const {
    return saveFiles;
  }

  const std::vector<std::string> &GetSplitsInputFiles() const {
    return splitsInputFiles;
  }

  const std::map<std::string, std::vector<MplOption>> &GetExtras() const {
    return extras;
  }

  const std::vector<std::string> &GetRunningExes() const {
    return runningExes;
  }

  const std::string &GetPrintCommandStr() const {
    return printCommandStr;
  }

  const bool GetDebugFlag() const {
    return debugFlag;
  }

  const bool GetTimePhases() const {
    return timePhases;
  }

  const bool GetGenMemPl() const {
    return genMemPl;
  }

  const bool GetGenVtableImpl() const {
    return genVtableImpl;
  }

  const bool GetVerify() const {
    return verify;
  }

 private:
  std::unique_ptr<mapleOption::OptionParser> optionParser;
  std::map<std::string, std::vector<mapleOption::Option>> options;
  std::map<std::string, std::vector<mapleOption::Option>> exeOptions;
  std::string inputFiles;
  std::string inputFolder;
  std::string outputFolder;
  std::string outputName;
  std::string exeFolder;
  InputFileType inputFileType;
  OptimizationLevel optimizationLevel;
  RunMode runMode;
  bool setDefaultLevel;
  bool isSaveTmps;
  std::vector<std::string> saveFiles;
  std::vector<std::string> splitsInputFiles;
  std::map<std::string, std::vector<MplOption>> extras;
  std::vector<std::string> runningExes;
  std::string printCommandStr;
  bool debugFlag;
  bool timePhases;
  bool genMemPl;
  bool genVtableImpl;
  bool verify;
  bool Init(const std::string &inputFile);
  ErrorCode HandleGeneralOptions();
  ErrorCode DecideRunType();
  ErrorCode DecideRunningPhases();
  ErrorCode CheckInputFileValidity();
  ErrorCode CheckRunMode(RunMode mode);
  ErrorCode CheckFileExits();
  void AddOption(const mapleOption::Option &option);
  ErrorCode UpdatePhaseOption(const std::string &args, const std::string &exeName);
  ErrorCode UpdateExtraOptionOpt(const std::string &args);
  ErrorCode AppendDefaultCombOptions();
  ErrorCode AppendDefaultCgOptions();
  ErrorCode AppendDefaultOptions(const std::string &exeName, MplOption mplOptions[], unsigned int length);
  void UpdateRunningExe(const std::string &args);
};
}  // namespace maple
#endif  // MAPLE_DRIVER_INCLUDE_MPL_OPTIONS_H
