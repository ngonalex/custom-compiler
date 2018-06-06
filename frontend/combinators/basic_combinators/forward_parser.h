#ifndef FORWARD_PARSER_H_
#define FORWARD_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"
#include "frontend/combinators/basic_combinators/parsestatus.h"

#include <string>

#define super NullParser

using namespace cs160::abstract_syntax::frontend;

namespace cs160 {
namespace frontend {

class ForwardParser : NullParser {
 public:
  virtual ParseStatus parse(std::string inputProgram,
      int startCharacter,
        std::string errorType = "");
        
  void setParser(NullParser* parser) {
    parser_ = std::make_unique<NullParser>(parser);
  }

 private:
  std::unique_ptr<NullParser> parser_;

};

}  // namespace frontend
}  // namespace cs160

#endif  // FORWARD_PARSER_H_

// Nemanja's Comments
// auto mul = new ForwardParser()

// mul.setParser(mulConcrete)
