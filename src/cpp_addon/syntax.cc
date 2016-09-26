#include <node.h>
#include <iostream>
#include <vector>
#include <stack>
#include <typeinfo>
#include <sstream>
#include <cstdlib>
#include <new>
#include <exception>
#include <cstring>
using namespace std;
namespace engine{
	string change_syntax(string input);
}
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


namespace constant{
	const string adt = "ADT";
	const string string = "STRING";
}

class Tree{
public:
	typedef pair<string, void *> value_type;
	typedef vector<value_type> vect;
	typedef vect::const_iterator const_iterator;
	
	template <class T>
		string typepredict(T& value){
			if(typeid(T) == typeid(Tree *))
				return constant::adt;
			else if(typeid(T) == typeid(string)){
				// String to Integer, Double, String, Charactor 
				return constant::string;
			}
			else
				return "Others";
		}

	inline void add(string* value){
		data.push_back(value_type( constant::string, (void *)value));
	}

	inline void add(Tree* value){
		data.push_back(value_type( constant::adt, (void *)value));
	}

	
	const_iterator begin() const{
		return data.begin();
	}
	const_iterator end() const{
		return data.end();
	}

private:
	vect data;
};

namespace bracket{
	
	inline bool is_bracket(char first){
		if(first == '(' || first == ')' || 
			first == '[' || first == ']' || 
			first == '{' || first == '}'){
				return true;
			}
		return false;
	}
	
	inline bool is_open(char first){
		if(first == '(' || first == '[' || first == '{')
			return true;
		return false;
	}
	
	inline bool is_close(char first){
		if(first == ')' || first == ']' || first == '}')
			return true;
		return false;
	}
	
	inline bool is_matched(char first, char second){
		if(is_open(first) && is_close(second))
			return true;
		return false;
	}

	inline char get_counterpart(char first){
		assert(is_bracket(first));
		switch (first)
		{
		case '(' : return ')';
		case '[' : return ']';
		case '{' : return '}';
		case ')' : return '(';
		case ']' : return '[';
		case '}' : return '{';
		}

		return 0;
	}

}
namespace engine {

	bool is_matching_parenthesis(string &input){
		stack<int> st;
		
		for(auto &it : input){
			if(bracket::is_open(it)){
				st.push(it);
			}else if(bracket::is_close(it)){
				if(st.empty())
					return false;
				
				char ch = st.top();
				st.pop();

				if(!bracket::is_matched(ch, it))
					return false;
			}
		}

		if(!st.empty())
			return false;
		
		return true;
	}

	Tree *head;

	void add_tree(Tree *parent, const string &input, int start, int pos){
		if(start > pos)
			return;

		string *substr = new string(input.substr(start, pos - start + 1));
		Tree *tree = new Tree;
		tree->add(substr);
		parent->add(tree);
	}
	void stringtotree(string input){

		head = new Tree;	
		stack<Tree *> level;
		level.push(head);

		
		int start = 0;
		int pos = 0;		
		
		for(auto &it : input){
			if(bracket::is_open(it)){
				Tree *tree = new Tree;
				level.top()->add(tree);
				level.push(tree);

				start = pos + 1;
			}
			if(bracket::is_close(it)){
				add_tree(level.top(), input, start, pos - 1);
				level.pop();

				start = pos + 1;
			}

			if(it == ','){
				add_tree(level.top(), input, start, pos);
				start = pos + 1;
			}
			pos++;
		}
		add_tree(level.top(), input, start, pos - 1);
	}

	void print_tree(Tree * tree){

		for(auto it = tree->begin(); it != tree->end(); ++it){
		
			if(it->first == constant::adt){
				print_tree((Tree *)(it->second));
			}else {
				string *str = (string *)(it -> second);
				cout << *str << endl;

				delete str;
			}
		}

		delete tree;
	}
	string change_syntax(string input){
		if(!is_matching_parenthesis(input)){
			cout << "parenthesis is not matched" << endl;
			return input;
		}

		stringtotree(input);

		print_tree(head);

		return input;
	}
}