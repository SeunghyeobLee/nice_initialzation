#include <iostream>
#include <vector>
#include <stack>
#include <cctype>

using namespace std;

namespace constant{
	const string adt = "ADT";
	const string string = "STRING";
}

class Tree{
public:
	typedef pair<string, void *> value_type;
	typedef vector<value_type> vect;
	typedef vect::const_iterator const_iterator;
	

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

	Tree *head;

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

	string trim(string input){
		int len = input.length();
		int i, j;
		for(i = 0; i < len;i ++)
			if(!isspace(input.at(i)))
				break;
		for(j = len-1; j >= 0;j --)
			if(!isspace(input.at(j)))
				break;
		
		return input.substr(i, j - i + 1);
	}

	void add_tree(Tree *parent, const string &input, int start, int pos){
		if(start > pos)
			return;

		string content = input.substr(start, pos - start + 1);//trim(input.substr(start, pos - start + 1));

		if(content.length() == 0)
			return;

		string *substr = new string("\"" + content + "\"");

		parent->add(substr);
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
			}else if(bracket::is_close(it)){
				add_tree(level.top(), input, start, pos - 1);
				level.pop();

				start = pos + 1;
			}else if(it == ',' || it == ' '){
				add_tree(level.top(), input, start, pos - 1);
				start = pos + 1;
			}
			else if(it == '\n' || it == '\t'){
				add_tree(level.top(), input, start, pos - 1);

				string *str = new string(1, it);
				level.top()->add(str);
				start = pos + 1;
			}

			pos++;
		}
		add_tree(level.top(), input, start, pos - 1);
	}

	string print_tree(const Tree * tree){

		string output = "{ ";
		for(auto it = tree->begin(); it != tree->end(); ++it){
			
			if(it != tree->begin())
				output += ", ";

			if(it->first == constant::adt){
				output += print_tree((Tree *)(it->second));
			}else {
				string *str = (string *)(it -> second);
				output += *str;

				delete str;
			}
		}

		delete tree;

		return output + " }";
	}
	string change_syntax(string input){
		if(!is_matching_parenthesis(input)){
			cout << "parenthesis is not matched" << endl;
			return input;
		}

		stringtotree(input);

		return print_tree(head);
	}
}