// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "google/protobuf/compiler/objectivec/oneof.h"

#include <string>

#include "absl/strings/str_cat.h"
#include "google/protobuf/compiler/objectivec/helpers.h"
#include "google/protobuf/compiler/objectivec/names.h"
#include "google/protobuf/compiler/objectivec/options.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/io/printer.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace objectivec {

OneofGenerator::OneofGenerator(const OneofDescriptor* descriptor,
                               const GenerationOptions& generation_options)
    : descriptor_(descriptor), generation_options_(generation_options) {
  variables_["enum_name"] = OneofEnumName(descriptor_);
  variables_["name"] = OneofName(descriptor_);
  variables_["capitalized_name"] = OneofNameCapitalized(descriptor_);
  variables_["raw_index"] = absl::StrCat(descriptor_->index());
  const Descriptor* msg_descriptor = descriptor_->containing_type();
  variables_["owning_message_class"] = ClassName(msg_descriptor);
}

void OneofGenerator::SetOneofIndexBase(int index_base) {
  int index = descriptor_->index() + index_base;
  // Flip the sign to mark it as a oneof.
  variables_["index"] = absl::StrCat(-index);
}

void OneofGenerator::GenerateCaseEnum(io::Printer* printer) const {
  auto vars = printer->WithVars(variables_);
  printer->Emit({{"cases",
                  [&] {
                    for (int j = 0; j < descriptor_->field_count(); j++) {
                      const FieldDescriptor* field = descriptor_->field(j);
                      printer->Emit(
                          {{"field_name", FieldNameCapitalized(field)},
                           {"field_number", field->number()}},
                          R"objc(
                            $enum_name$_$field_name$ = $field_number$,
                          )objc");
                    }
                  }}},
                R"objc(
                  typedef GPB_ENUM($enum_name$) {
                    $enum_name$_GPBUnsetOneOfCase = 0,
                    $cases$
                  };
                )objc");
  printer->Emit("\n");
}

void OneofGenerator::GeneratePublicCasePropertyDeclaration(
    io::Printer* printer) const {
  auto vars = printer->WithVars(variables_);
  printer->Emit({{"comments",
                  [&] {
                    EmitCommentsString(printer, generation_options_,
                                       descriptor_);
                  }}},
                R"objc(
                  $comments$;
                  @property(nonatomic, readonly) $enum_name$ $name$OneOfCase;
                )objc");
  printer->Emit("\n");
}

void OneofGenerator::GenerateClearFunctionDeclaration(
    io::Printer* printer) const {
  auto vars = printer->WithVars(variables_);
  printer->Emit(R"objc(
      /**
       * Clears whatever value was set for the oneof '$name$'.
       **/
      void $owning_message_class$_Clear$capitalized_name$OneOfCase($owning_message_class$ *message);
    )objc");
}

void OneofGenerator::GeneratePropertyImplementation(
    io::Printer* printer) const {
  auto vars = printer->WithVars(variables_);
  printer->Emit(R"objc(
    @dynamic $name$OneOfCase;
  )objc");
}

void OneofGenerator::GenerateClearFunctionImplementation(
    io::Printer* printer) const {
  auto vars = printer->WithVars(variables_);
  printer->Emit(R"objc(
    void $owning_message_class$_Clear$capitalized_name$OneOfCase($owning_message_class$ *message) {
      GPBDescriptor *descriptor = [$owning_message_class$ descriptor];
      GPBOneofDescriptor *oneof = [descriptor.oneofs objectAtIndex:$raw_index$];
      GPBClearOneof(message, oneof);
    }
  )objc");
}

std::string OneofGenerator::DescriptorName() const {
  return variables_.find("name")->second;
}

std::string OneofGenerator::HasIndexAsString() const {
  return variables_.find("index")->second;
}

}  // namespace objectivec
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
