//
// Created by Leo Li on 2019-10-29.
//

#include "MacroExpand.h"
#include "KernelFuseTool.h"

namespace kernel_fusion {

void MacroExpand::run(const ast_matchers::MatchFinder::MatchResult &Result) {

  const auto *D = Result.Nodes.getNodeAs<FunctionDecl>(KernelFuseBindId);
  if (D->isTemplateInstantiation()) return;
  std::string Impl;
  llvm::raw_string_ostream ImplStream(Impl);
  const auto TD = D->getDescribedTemplate();
  if (TD) {
    TD->print(ImplStream);
  } else {
    D->print(ImplStream);
  }
  ImplStream.flush();
  const auto Replacement = tooling::Replacement(Result.Context->getSourceManager(),
                                                TD ? dyn_cast<NamedDecl>(TD) :  D, Impl);
  if (auto Err = Replacements[Replacement.getFilePath().str()].add(Replacement)) {
    llvm::outs() << "error";
  }
}
}