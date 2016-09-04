#include <iostream>

#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>

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

  ctx->Release();
  engine->ShutDownAndRelease();

  return 0;
}