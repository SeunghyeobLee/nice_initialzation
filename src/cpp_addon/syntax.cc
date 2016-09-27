#include <node.h>
#include "engine.hpp"

using namespace std;

namespace nodejs {
	
	inline string v8Tostring(v8::Local<v8::Value> value){
		v8::String::Utf8Value utf8_value(v8::Local<v8::String>::Cast(value));
		return string(*utf8_value);
	}

	inline v8::Local<v8::String> stringToString(v8::Isolate* isolate, string input){
		return v8::String::NewFromUtf8(isolate, input.c_str());
	}
	
	using namespace v8;
	using namespace engine;

	void Standard(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();

		if (args.Length() > 1) {
			// Throw an Error that is passed back to JavaScript
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong number of arguments")));
			return;
		}
		if(!args[0] -> IsString()){
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments")));
			return;
		}

		args.GetReturnValue().Set(
			stringToString(isolate, 
				engine::change_syntax(
					v8Tostring(args[0]))));
	}

	void init(Local<Object> exports) {
		NODE_SET_METHOD(exports, "standard", Standard);
	}

	NODE_MODULE(addon, init)
} 

