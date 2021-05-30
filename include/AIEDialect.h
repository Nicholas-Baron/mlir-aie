// (c) Copyright 2019 Xilinx Inc. All Rights Reserved.
//===- AIEDialect.h - Dialect definition for the AIE IR ----------------===//
//
// Copyright 2019 Xilinx
//
//===---------------------------------------------------------------------===//

#ifndef MLIR_AIE_DIALECT_H
#define MLIR_AIE_DIALECT_H

#include "mlir/IR/Dialect.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/IR/OpImplementation.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/TypeSupport.h"
#include "mlir/IR/Types.h"
#include "mlir/Pass/Pass.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/Debug.h"
#include "mlir/Dialect/StandardOps/IR/Ops.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include <map>

using namespace mlir;

#include "AIEEnums.h.inc"

namespace mlir {
namespace OpTrait {
template <typename ConcreteType>
class FlowEndPoint : public OpTrait::TraitBase<ConcreteType, FlowEndPoint> {
};
}
}

/// Include the generated interface declarations.
#include "AIEInterfaces.h.inc"

namespace xilinx {
namespace AIE {

void registerAIEAssignBufferAddressesPass();
void registerAIETranslations();
void registerAIEFindFlowsPass();
void registerAIECreateFlowsPass();
void registerAIECreateCoresPass();
void registerAIECreateLocksPass();
void registerAIEHerdRoutingPass();
void registerAIECreatePacketFlowsPass();
void registerAIELowerMemcpyPass();

// FIXME: use this
//#include "AIEDialect.h.inc"

// The Dialect
class AIEDialect : public mlir::Dialect {
public:
  explicit AIEDialect(mlir::MLIRContext *ctx);
  static StringRef getDialectNamespace() { return "AIE"; }


  // /// Parse a type registered to this dialect. Overridding this method is
  // /// required for dialects that have custom types.
  // /// Technically this is only needed to be able to round-trip to textual IR.
  // mlir::Type parseType(DialectAsmParser &parser) const override;

  // /// Print a type registered to this dialect. Overridding this method is
  // /// only required for dialects that have custom types.
  // /// Technically this is only needed to be able to round-trip to textual IR.
  // void printType(mlir::Type type, DialectAsmPrinter &os) const override;
};

////////////////////////////////////////////////////////////////////////////////
/////////////////////// Custom Types for the Dialect ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

// namespace detail {
// struct AIEListTypeStorage;
// }

// /// LLVM-style RTTI: one entry per subclass to allow dyn_cast/isa.
// enum AIETypeKind {
//   // The enum starts at the range reserved for this dialect.
//   AIE_TYPE = mlir::Type::FIRST_PRIVATE_EXPERIMENTAL_0_TYPE,
//   AIE_LIST,
// };

// /// Type for Toy arrays.
// /// In MLIR Types are reference to immutable and uniqued objects owned by the
// /// MLIRContext. As such `AIEListType` only wraps a pointer to an uniqued
// /// instance of `AIEListTypeStorage` (defined in our implementation file) and
// /// provides the public facade API to interact with the type.
// class AIEListType : public mlir::Type::TypeBase<AIEListType, mlir::Type,
//                                                  detail::AIEListTypeStorage> {
// public:
//   using Base::Base;

//   /// Return the type of individual elements in the array.
//   mlir::Type getElementType();

//   /// Get the unique instance of this Type from the context.
//   static AIEListType get(mlir::Type elementType);

//   /// Support method to enable LLVM-style RTTI type casting.
//   static bool kindof(unsigned kind) { return kind == AIETypeKind::AIE_LIST; }
// };


////////////////////////////////////////////////////////////////////////////////
//////////////////// Custom Operations for the Dialect /////////////////////////
////////////////////////////////////////////////////////////////////////////////

//#include "AIEOpInterfaces.h.inc"

typedef std::pair<WireBundle, int> Port;
typedef std::pair<Port, Port> Connect;
typedef std::pair<int, int> TileID;

bool isValidTile(TileID src);

// Return the tile ID of the memory to the west of the given tile, if it exists.
Optional<TileID> getMemWest(TileID src);
// Return the tile ID of the memory to the east of the given tile, if it exists.
Optional<TileID> getMemEast(TileID src);
// Return the tile ID of the memory to the north of the given tile, if it exists.
Optional<TileID> getMemNorth(TileID src);
// Return the tile ID of the memory to the south of the given tile, if it exists.
Optional<TileID> getMemSouth(TileID src);

bool isInternal(int srcCol, int srcRow, int dstCol, int dstRow);
bool isWest(int srcCol, int srcRow, int dstCol, int dstRow);
bool isMemWest(int srcCol, int srcRow, int dstCol, int dstRow);
bool isEast(int srcCol, int srcRow, int dstCol, int dstRow);

bool isMemEast(int srcCol, int srcRow, int dstCol, int dstRow);
bool isNorth(int srcCol, int srcRow, int dstCol, int dstRow);
bool isMemNorth(int srcCol, int srcRow, int dstCol, int dstRow);
bool isSouth(int srcCol, int srcRow, int dstCol, int dstRow);
bool isMemSouth(int srcCol, int srcRow, int dstCol, int dstRow);

bool isLegalMemAffinity(int coreCol, int coreRow, int memCol, int memRow);
}

// include TableGen generated Op definitions
#define GET_OP_CLASSES
#include "AIE.h.inc"

namespace AIE {

#define GEN_PASS_CLASSES
#include "AIEPasses.h.inc"

std::unique_ptr<OperationPass<ModuleOp>> createAIECoreToLLVMPass();
std::unique_ptr<OperationPass<ModuleOp>> createAIECoreToStandardPass();
std::unique_ptr<OperationPass<ModuleOp>> createAIENormalizeAddressSpacesPass();
std::unique_ptr<OperationPass<FuncOp>> createAIEVectorOptPass();

/// Generate the code for registering passes.
#define GEN_PASS_REGISTRATION
#include "AIEPasses.h.inc"

} // AIE
} // xilinx

#endif
