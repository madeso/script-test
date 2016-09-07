#include <iostream>

#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>
#include "shared.h"

// Print the script string to the standard output stream
void print(std::string &msg)
{
  std::cout << msg;
}

void MessageCallback(const AngelScript::asSMessageInfo *msg, void *param)
{
  const char *type = "ERR ";
  if( msg->type == AngelScript::asMSGTYPE_WARNING )
    type = "WARN";
  else if( msg->type == AngelScript::asMSGTYPE_INFORMATION )
    type = "INFO";
  std::cerr << msg->section <<" ("<<msg->row<<", "<<msg->col<<") : "<<type<<" : "<<msg->message<<"\n";
}

void Die(const std::string& err) {
  std::cerr << err << "\n";
  throw err;
}

struct AsTest : LanguageTest<AsTest> {
  AsTest(AngelScript::asIScriptContext* ctx, AngelScript::asIScriptModule *mod) : ctx_(ctx) {
    rec = mod->GetFunctionByDecl("int fib_rec(int)");
    if( rec == NULL ) Die("failed to get rec");

    loop = mod->GetFunctionByDecl("int fib_loop(int)");
    if( loop == NULL ) Die("failed to get loop");

    str = mod->GetFunctionByDecl("int string_test(int)");
    if( str == NULL ) Die("failed to get str");
  }

  int RunRec(int i) {
    int p = ctx_->Prepare(rec);
    if( p < 0 ) {
      std::cerr << "Failed to prepare " << p << "\n";
      return -42;
    }
    ctx_->SetArgDWord(0, i);
    int r = ctx_->Execute();
    if( r == AngelScript::asEXECUTION_FINISHED) {
      return ctx_->GetReturnDWord();
    }

    return -42;
  }

  int RunLoop(int i) {
    int p = ctx_->Prepare(loop);
    if( p < 0 ) {
      std::cerr << "Failed to prepare " << p << "\n";
      return -42;
    }
    ctx_->SetArgDWord(0, i);
    int r = ctx_->Execute();
    if( r == AngelScript::asEXECUTION_FINISHED) {
      return ctx_->GetReturnDWord();
    }

    return -42;
  }

  int RunStr(int i) {
    int p = ctx_->Prepare(str);
    if( p < 0 ) {
      std::cerr << "Failed to prepare " << p << "\n";
      return -42;
    }
    ctx_->SetArgDWord(0, i);
    int r = ctx_->Execute();
    if( r == AngelScript::asEXECUTION_FINISHED) {
      return ctx_->GetReturnDWord();
    }

    return -42;
  }

  AngelScript::asIScriptFunction* rec;
  AngelScript::asIScriptFunction* loop;
  AngelScript::asIScriptFunction* str;
  AngelScript::asIScriptContext *ctx_;
};

int main() {
  using namespace AngelScript;

  int r = 0;

  AngelScript::asIScriptEngine *engine = AngelScript::asCreateScriptEngine();

  r = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
  if( r < 0 ) {
    std::cerr << "Failed to setup as\n";
    return 0;
  }

  AngelScript::RegisterStdString(engine);
  r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
  if( r < 0 ) {
    std::cerr << "Failed to setup as " << r << "\n";
    return 0;
  }

  r = engine->RegisterGlobalFunction("string build_string(string)", asFUNCTION(BuildString), asCALL_CDECL);
  if( r < 0 ) {
    std::cerr << "Failed to setup build_string " << r << "\n";
    return 0;
  }

  CScriptBuilder builder;
  r = builder.StartNewModule(engine, "MyModule");
  if( r < 0 )
  {
    std::cerr << "Unrecoverable error while starting a new module.\n";
    return 0;
  }

  r = builder.AddSectionFromFile("test.as");
  if( r < 0 )
  {
    std::cerr << "Please correct the errors in the script and try again.\n";
    return 0;
  }
  r = builder.BuildModule();
  if( r < 0 )
  {
    std::cerr << "Please correct the errors in the script and try again.\n";
    return 0;
  }

  asIScriptModule *mod = engine->GetModule("MyModule");
  asIScriptFunction *func = mod->GetFunctionByDecl("void main()");
  if( func == 0 )
  {
    std::cerr << "The script must have the function 'void main()'. Please add it and try again.\n";
    return 0;
  }

  asIScriptContext *ctx = engine->CreateContext();
  ctx->Prepare(func);
  r = ctx->Execute();
  if( r != asEXECUTION_FINISHED )
  {
    if( r == asEXECUTION_EXCEPTION )
    {
      std::cerr << "An exception '" << ctx->GetExceptionString() <<"' occurred. Please correct the code and try again.\n";
    }
  }

  AsTest test(ctx, mod);
  test.Run();

  ctx->Release();
  engine->ShutDownAndRelease();

  return 0;
}