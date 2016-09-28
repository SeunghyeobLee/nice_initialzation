#include <iostream>
#include <vector>
#include <stack>
#include <cctype>
#include <cstdlib>

using namespace std;

// 타입의 이름을 string으로 표현 
namespace constant{
	const string adt = "ADT";
	const string string = "STRING";
    const std::string integer = "INTEGER";
}

// 동적인 트리
// 타입을 string으로, 값을 void pointer로 저장
class Tree{
public:
	typedef pair<string, void *> value_type;
	typedef vector<value_type> vect;
	typedef vect::const_iterator const_iterator;
	
    inline void add(int* value){
        data.push_back(value_type( constant::integer, (void *)value));
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

// 괄호 연산
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

// 계산 파트
namespace engine {

	Tree *head;

    // 괄호가 잘 닫혀 있는지 확인
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

    // string의 앞뒤 space 제거
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

    bool is_integer(string input){

        auto it = input.begin();
        char ch = *it;
        
        if(!((ch >= '0' && ch <= '9') || ch == '-' || ch == '+'))
            return false;
        
        
        for(it++;it != input.end(); it++){
            if(!(*it >= '0' && *it <= '9')){
                return false;
            }
        }
        return true;
    }
    // tree에 string 저장
	void add_tree(Tree *parent, const string &input, int start, int pos){
		if(start > pos)
			return;

		string content = input.substr(start, pos - start + 1);

		if(content.length() == 0)
			return;

        if(is_integer(content)){
            int p = std::atoi(content.c_str());
            int *value = new int(p);

            parent->add(value);
        }else {
		    string *substr = new string("\"" + content + "\"");

		    parent->add(substr);
        }
	}

    // parsing해서 tree에 저장
    // stack을 이용하여 다중 괄호 처리
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
			}else if(it == ',' || it == ' ' || it == '\t'){
				add_tree(level.top(), input, start, pos - 1);
				start = pos + 1;
			}
			else if(it == '\n'){
				add_tree(level.top(), input, start, pos - 1);

				string *str = new string(1, it);
				level.top()->add(str);
				start = pos + 1;
			}

			pos++;
		}
		add_tree(level.top(), input, start, pos - 1);
	}

    // tree 내용 출력 및 제거
	string print_tree(const Tree * tree){

		string output = "{ ";
		for(auto it = tree->begin(); it != tree->end(); ++it){
			
			if(it != tree->begin())
				output += ", ";

			if(it->first == constant::adt){
				output += print_tree((Tree *)(it->second));
			}else if(it->first == constant::string){
				string *str = (string *)(it -> second);
				output += *str;

				delete str;
			}else if(it->first == constant::integer){
                int *integer = (int *)(it -> second);

                output += to_string(*integer);

                delete integer;
            }
		}

		delete tree;

		return output + " }";
	}
    // main
	string change_syntax(string input){
		if(!is_matching_parenthesis(input)){
			cout << "parenthesis is not matched" << endl;
			return input;
		}

		stringtotree(input);

		return print_tree(head);
	}
}