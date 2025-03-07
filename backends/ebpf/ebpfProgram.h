/*
Copyright 2013-present Barefoot Networks, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef BACKENDS_EBPF_EBPFPROGRAM_H_
#define BACKENDS_EBPF_EBPFPROGRAM_H_

#include "codeGen.h"
#include "ebpfModel.h"
#include "ebpfObject.h"
#include "ebpfOptions.h"
#include "frontends/common/options.h"
#include "frontends/p4/evaluator/evaluator.h"
#include "frontends/p4/typeMap.h"
#include "ir/ir.h"
#include "target.h"

namespace EBPF {

class EBPFProgram;
class EBPFParser;
class EBPFControl;
class EBPFDeparser;
class EBPFTable;
class EBPFType;

class EBPFProgram : public EBPFObject {
 public:
    // The builder->target defines either TC or XDP target,
    // while for PSA-eBPF we may use both of them interchangeably.
    // This field stores the Target object that is unique per eBPF program (pipeline).
    const Target *progTarget;
    const EbpfOptions &options;
    const IR::P4Program *program;
    const IR::ToplevelBlock *toplevel;
    P4::ReferenceMap *refMap;
    P4::TypeMap *typeMap;
    EBPFParser *parser;
    EBPFControl *control;
    EBPFModel &model;
    /// Deparser may be NULL if not supported (e.g. ebpfFilter package).
    EBPFDeparser *deparser;

    cstring endLabel, offsetVar, lengthVar, headerStartVar;
    cstring zeroKey, functionName, errorVar;
    cstring packetStartVar, packetEndVar, byteVar;
    cstring errorEnum;
    cstring license = "GPL";  /// TODO: this should be a compiler option probably
    cstring arrayIndexType = "u32";

    virtual bool build();  /// return 'true' on success

    EBPFProgram(const EbpfOptions &options, const IR::P4Program *program, P4::ReferenceMap *refMap,
                P4::TypeMap *typeMap, const IR::ToplevelBlock *toplevel)
        : progTarget(nullptr),
          options(options),
          program(program),
          toplevel(toplevel),
          refMap(refMap),
          typeMap(typeMap),
          parser(nullptr),
          control(nullptr),
          model(EBPFModel::instance),
          deparser(nullptr) {
        // NB: offsetVar not used in eBPF backend - uBPF and TC only
        offsetVar = EBPFModel::reserved("packetOffsetInBits");
        zeroKey = EBPFModel::reserved("zero");
        functionName = EBPFModel::reserved("filter");
        errorVar = EBPFModel::reserved("errorCode");
        packetStartVar = EBPFModel::reserved("packetStart");
        packetEndVar = EBPFModel::reserved("packetEnd");
        headerStartVar = EBPFModel::reserved("headerStart");
        lengthVar = EBPFModel::reserved("pkt_len");
        byteVar = EBPFModel::reserved("byte");
        endLabel = EBPFModel::reserved("end");
        errorEnum = EBPFModel::reserved("errorCodes");
    }

 protected:
    virtual void emitPreamble(CodeBuilder *builder);
    virtual void emitTypes(CodeBuilder *builder);
    virtual void emitHeaderInstances(CodeBuilder *builder);
    virtual void emitLocalVariables(CodeBuilder *builder);
    virtual void emitPipeline(CodeBuilder *builder);

    /// Checks whether a method name is considered to be part of the standard library, e.g., defined
    /// in core.p4 or ebpf_model.p4.
    /// TODO: Should we also distinguish overloaded methods?
    virtual bool isLibraryMethod(cstring methodName);

 public:
    virtual void emitCommonPreamble(CodeBuilder *builder);
    virtual void emitGeneratedComment(CodeBuilder *builder);
    virtual void emitH(CodeBuilder *builder, cstring headerFile);  // emits C headers
    virtual void emitC(CodeBuilder *builder, cstring headerFile);  // emits C program

    DECLARE_TYPEINFO(EBPFProgram, EBPFObject);
};

}  // namespace EBPF

#endif /* BACKENDS_EBPF_EBPFPROGRAM_H_ */
