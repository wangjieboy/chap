// Copyright (c) 2017 VMware, Inc. All Rights Reserved.
// SPDX-License-Identifier: GPL-2.0

#pragma once
#include "../Commands/Runner.h"
#include "../Commands/Subcommand.h"
#include "../ThreadMap.h"
namespace chap {
namespace ThreadMapCommands {
template <class Offset>
class CountStacks : public Commands::Subcommand {
 public:
  CountStacks() : Commands::Subcommand("count", "stacks"), _processImage(0) {}

  void SetProcessImage(const ProcessImage<Offset>* processImage) {
    _processImage = processImage;
    if (processImage != NULL) {
      _threadMap = &processImage->GetThreadMap();
    } else {
      _threadMap = (const ThreadMap<Offset>*)(0);
    }
  }

  void ShowHelpMessage(Commands::Context& context) {
    context.GetOutput()
        << "This command provides "
           "totals of the number of threads and the space they occupy.\n";
  }

  void Run(Commands::Context& context) {
    Commands::Output& output = context.GetOutput();
    Commands::Error& error = context.GetError();
    bool isRedirected = context.IsRedirected();
    if (_processImage == 0) {
      error << "This command is currently disabled.\n";
      error << "There is no process image.\n";
      if (isRedirected) {
        output << "This command is currently disabled.\n";
        output << "There is no process image.\n";
      }
      return;
    }
    Offset totalBytes = 0;
    typename ThreadMap<Offset>::const_iterator itEnd = _threadMap->end();
    for (typename ThreadMap<Offset>::const_iterator it = _threadMap->begin();
         it != itEnd; ++it) {
      totalBytes += (it->_stackLimit - it->_stackBase);
    }
    output << std::dec << _threadMap->NumThreads() << " threads use 0x"
           << std::hex << totalBytes << " bytes.\n";
  }

 private:
  const ProcessImage<Offset>* _processImage;
  const ThreadMap<Offset>* _threadMap;
};
}  // namespace ThreadMapCommands
}  // namespace chap
