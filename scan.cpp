#include "scan.h"
#include <fstream>
#include <map>

enum _state{
	BEGIN,
	NUMBER,
	SYMBOL, 			
	STRING, 
	OPERATION,
	SEPARATOR,
	WHITESPACE,
	CHAR,
	SUCCESS,
	COMMENT,
	END
};  

char m[][13] = { "int number", "float_number", "identifier", "keyword", "separator", "operation", "string", "char"};

static bool isNumber(char s){
	return s >= '0' && s <= '9';
}

static bool isSymbol(char s){
	return s >= 'A' && s <= 'Z' 
		|| s >= 'a' && s <= 'z'
		|| s == '_';
}

static bool isOperation(char s){
	return  s == '<' ||
			s == '>' ||
			s == '=' ||
			s == '+' ||
			s == '-' ||
			s == '*' ||
			s == '/' ||
			s == '&' ||
			s == '%' ||
			s == '|' ||
			s == '^' ||
			s == '?' ||
			s == '~' ||
			s == '!';
}

static bool isSeparation(char s){
	return  s == '.' ||	
			s == ',' ||
			s == ':' ||
			s == ';' ||
			s == '(' ||
			s == ')' ||
			s == '{' ||
			s == '}' ||
			s == '[' ||
			s == ']' ||
			s == '}';
}

static bool isKeyWord(string str){
	static string arr[] = {"bool", "break", "case",
	"char", "const",
	"continue", "default", "delete",
	"do", "else", "enum", "float",
	"for", "friend", "if", "int", 
	"long", "struct", "this", 
	"true", "typedef", "return", 
	"union", "void", "while", 
	"printf", "scanf", "inline"};
	int i = 0;
	for each(string s in arr){
		if(str == s)
			return true;
		i++;
	}
	return false;
}

static bool isWhitespace(char s){
	return  (s == ' ') || (s == '\n') || (s == '\t');
}

Scan::Scan(ifstream *ff){
	f = ff;
	col = line = 1;
	buf = 1;
	t = 0;
	last_token = end_of_file = false;
}

Scan::~Scan(void){
	delete t;
}

bool Scan::isEnd(){
	return end_of_file;
}

void Token::Print() const {
		cout << line << "\t" << start << "\t" << str << string(str.length(), ' ') << "\t" << m[type] << endl ;
}

void Token::Print(ofstream *t) const {
		*t << line << "\t" << start << "\t" << str << string(str.length(), ' ') << "\t" << m[type] << endl;
}

void MyException::Print(ofstream *f) const {
	if(line != -1)
		*f << massage << "\t line " << line << ", col " << col << endl;
	else
		*f << massage << endl;
}

MyException::MyException(const string &m, Token *token) : massage(m) {
	line = token ? token->line : -1;
	col = token ? token->start : -1;
}

void MyException::Print() const {
	cout << massage;
	if(line != -1){
			cout << "\t line " << line;
		if(col != -1)
			cout << ", col " << col << endl;
	}
	cout << endl;
}

Token* Scan::Get(){
	return t;
}

Operation::Operation(Values v) : Token(0, 0, "", T_OPERATION){
	value = v;
}

Operation::Operation(int l, int s, string st) : Token(l, s, st , T_OPERATION) {
	static map<string, Values> m;
	static bool ok = false;
	if(!ok){
		m["+"] = Values::plus;
		m["-"] = Values::minus;
		m["*"] = factor;
		m["/"] = divis;
		m["++"] = incr;
		m["--"] = decr;
		m["%"] = percent;
		m[">"] = more;
		m["<"] = Values::less;
		m["=="] = equally;
		m[">="] = more_or_eql;
		m["<="] = less_or_eql;
		m["!="] = not_eql;
		m["&&"] = l_and;
		m["||"] = l_or;
		m["&"] = b_and;
		m["|"] = b_or;
		m[">>"] = sr;
		m["<<"] = sl;
		m["="] = assign;
		m["+="] = pl_eql;
		m["-="] = mn_eql;
		m["*="] = fct_eql;
		m["/="] = div_eql;
		m["%="] = per_eql;
		m["&="] = and_eql;
		m["|="] = or_eql;
		m["!"] = l_not;
		m["~"] = b_not;
		m["^"] = xor;
		m["^="] = xor_eql;
		m["~="] = not_b_eql;
		m["?"] = quest;
		m["["] = op_sq_bracket;
		m["]"] = cl_sq_bracket;
		m["("] = op_bracket;
		m[")"] = cl_bracket;
		m["."] = point;
		m["->"] = arrow;
		m[">>="] = sr_eql;
		m["<<="] = sl_eql;
	}
	ok = true;
	value = m[str];
 }

KeyWord::KeyWord (int l, int s, string st , Types t = T_KEYWORD) : Token(l, s, st , t){
	static map<string, Values> m;
	static bool ok = false;
	if(!ok){
		m["bool"] = _bool;
		m["break"] = _break;
		m["case"] = _case;
		m["catch"] = _catch;
		m["char"] = _char;
		m["const"] = _const;
		m["continue"] = _continue;
		m["default"] = _default;
		m["delete"] = _delete;
		m["do"] = _do;
		m["double"] = _double;
		m["else"] = _else;
		m["enum"] = _enum;
		m["false"] = _false;
		m["float"] = _float;
		m["for"] = _for;
		m["friend"] = _friend;
		m["goto"] = _goto;
		m["int"] = _int;
		m["if"] = _if;
		m["long"] = _long;
		m["this"] = _this;
		m["true"] = _true;
		m["typedef"] = _typedef;
		m["typename"] = _typename;
		m["return"] = _return;
		m["union"] = _union;
		m["void"] = _void;
		m["while"] = _while;
		m["struct"] = _struct;
		m["scanf"] = _scanf;
		m["printf"] = _printf;
		m["inline"] = _Inline;
	}
	ok = true;
	value = m[str];
 }

Token::Token(int l, int s, int number, Types tp) : line(l), start(s), type(tp){
	str = to_string(number);
}

Token::Token(int l, int s, float number, Types tp) : line(l), start(s), type(tp){
	ostringstream buff;
    buff << number;
    str = buff.str();
}

Identifier::Identifier(int l, int s, string st) : Token(l, s, st, T_IDENTIFIER){
	value = val_ident;
}

IntNumber::IntNumber(int l, int s, string st) : Token(l, s, st, T_INTEGER){
	value = val_int; 
	v2 = atoi(str.c_str());
}

IntNumber::IntNumber(int l, int s, int number) : Token(l, s, number, T_INTEGER){
	value = val_int;
	v2 = number;
}

 FloatNumber::FloatNumber(int l, int s, string st) : Token(l, s, st, T_FLOAT){
	value = val_float;
	v2 = atof(str.c_str());
 }

 FloatNumber::FloatNumber(int l, int s, float number): Token(l, s, number, T_FLOAT){
	 value = val_float;
	 v2 = number;
 }

 Separator::Separator(int l, int s, string st , Types t = T_SEPARATOR) : Token(l, s, st , t){
	static map<string, Values> m;
	static bool ok = false;
	if(!ok){
		m["."] = point;
		m[","] = comma;
		m[";"] = semicolon;
		m[":"] = colon;
		m["{"] = op_brace;
		m["}"] = cl_brace;
		m["["] = op_sq_bracket;
		m["]"] = cl_sq_bracket;
		m["("] = op_bracket;
		m[")"] = cl_bracket;
	}
	ok = true;
	value = m[str];
 }

bool Scan::Next(){
	if(last_token)
		return end_of_file = true;
	_state state = BEGIN;
	char ch;
	if(buf == 1)
		f->get(ch);
	else
		ch = buf;
	int i = col + 1;
	string str("");
	bool point = false;

	if(f->eof() && (buf == 1)){
		state = END;
		end_of_file = true; 
		throw MyException("File is empty");;
	}
	while (state != SUCCESS)
		switch (state) {
		case BEGIN:
	 		if (isNumber(ch)){
				state = NUMBER;
				break;
			}
			else if(ch == '\"'){			
				state = STRING;
				break;
			} else if(ch == '\''){
				state = CHAR;
				break;
			} else if(isOperation(ch)){
				state = OPERATION;
				break;
			}else if (isSeparation(ch)){
				if(ch == '.'){
					str += ch;
					f->get(ch);
					if(isNumber(ch)){
						state = NUMBER;
						point = true;
						break;
					}
				}
				state = SEPARATOR;
				break;
			}else if(isWhitespace(ch)){
				state = WHITESPACE;
				break;
			}else if(isSymbol(ch)){
				state = SYMBOL;
				break;
			}else if(ch == '-'){
				f->get(buf);
				if(buf == '>'){
					t = new Separator(line, col, "->");
					f->get(buf);
					state = SUCCESS;
					break;
				} else
					throw MyException("Unknown token", line, col);
			} else
				throw MyException("Unknown token", line, col);
		

		case NUMBER:
			if(!point)
			while(ch == '0')
				f->get(ch);
			else
				str = "0.";

			while((((isNumber(ch) || (ch == '.')) && !point) ||
				 (point && isNumber(ch))) && !f->eof()){

				if (ch == '.')
					point = true;			
				str += ch;
				f->get(ch);
				i++;
			}
			if(f->eof()){
				if(point)
					t = new FloatNumber(line, col, str);
				else
					t = new IntNumber(line, col, str);
				state = END;
				break;
			}
			if(point && (ch == '.')) 
				throw MyException( "\n Incorrect number", line, col);

			if(ch == 'e'){
				str += ch;
				f->get(ch);
				if((ch == '+') || (ch == '-')){
					str += ch;
					i++;
					f->get(ch);
				}
				if(isNumber(ch)){
					while(isNumber(ch) && !f->eof()){
						str += ch;
						f->get(ch);
						i++;
					}
					if(isSymbol(ch)) 
						throw MyException("\n Incorrect number", line, col);

				} else  
					throw MyException("\n Incorrect number", line, col);
			}

			if(f->eof()){
				if(point)
					t = new FloatNumber(line, col, str);
				else
					t = new IntNumber(line, col, str);
				state = END;
				break;
			}

			buf = ch;
			if (isOperation(ch) || isSeparation(ch) || isWhitespace(ch)){
				if(point)
					t = new FloatNumber(line, col, str);
				else
					t = new IntNumber(line, col, str);
				col = i;
				state = SUCCESS;
			} else	if (isSymbol(ch))
				throw MyException("\n Incorrect number", line, col);
			break;

		case SEPARATOR:
			if (str == "."){
				t = new Operation(line, col, str);
				buf = ch;
				col += 2;
			}
			else{
				if(ch == '[' || ch == ']' 
					|| ch == '(' || ch == ')'
					)
					t = new Operation(line, col, str += ch);
				else
					t = new Separator(line, col, str += ch);	
				col++;
				f->get(buf);
			}
			state = (f->eof()) ? END : SUCCESS;
			break;

		case OPERATION:
			str += ch;
			if(!f->eof()){
				f->get(buf);
				if((ch == '/') && (buf == '/') || (ch == '/') && (buf == '*')){
					state = COMMENT;
					break;
				}
			} else {
				t = new Operation(line, col, str);
				col = i;
				state = END;
				break;	
			}
			if(!f->eof()){
				if(ch == '-' && buf == '>'){
					str += buf;
					i++;
					f->get(buf);
				}
				if(buf == '=' || 
					buf == '+' && ch == '+' ||
					buf == '-' && ch == '-' ||
					buf == '>' && ch == '>' ||
					buf == '|' && ch == '|' ||
					buf == '&' && ch == '&' ||
					buf == '<' && ch == '<'  && buf != '?') {
						str += buf;	
						i++;
						f->get(buf);
						if((str == ">>" || str == "<<") && buf == '='){
							str += buf;
							i++;
							f->get(buf);
						}
				}
			}
			state = (f->eof()) ? END : SUCCESS;
			t = new Operation(line, col, str);
			col = i;
			break;

		case WHITESPACE:
			if(ch == '\n'){
				line++;
				col = 1;
			} else 
				col = i;

			f->get(ch);
			if(f->eof()){
				return end_of_file = true;
			} else
			state = BEGIN;
			break;

		case STRING:
			f->get(ch);
			f->get(buf);
			i++;

			while(!f->eof() && (buf != '\"' && buf != '\n' || 
				ch == 92 && buf == '\"' && buf != '\n')){
				str += ch;
				ch = buf;
				f->get(buf);
				i++;
			}
			if(buf == '\"'){
				str += ch;
				f->get(buf);
				i++;
			} else 
				throw MyException("\n There is no closing quote", line, col);
		
			state = f->eof() ? END : SUCCESS;
			t = new String(line, col, str);
			col = i;
			break;

		case CHAR:
			f->get(ch);
			if (ch == '\'') 
				throw MyException("\n There is no character in single quotes", line, col);
			if (ch == '\\'){
				state = SUCCESS;
				f->get(ch);	 
				if (ch  == 't')
					str += '\t';
				else if (ch == 'n')
					str += '\n';
				else
					str += ch;
				f->get(ch);
				if (ch != '\'' || f->eof()){
					if(f->eof())
						str = "\n Unexpected end of file";
					else
						str = "\n There is no closing quote"; 
					throw MyException(str, line, col);
				} else {
					t = new Char(line, col, str);
					col += 5;
					f->get(buf);
					if (f->eof())
						state= END;
					break;
				}
			}
			str += ch;
			f->get(ch);
			if (ch != '\'')
				throw MyException( "\n Too many characters in quotes", line, col);
			else {
				t = new Char(line, col, str);
				col += 5;
				f->get(buf);
				state = f->eof() ? END : SUCCESS;
				break;
			}
		

		case SYMBOL:
			while((isSymbol(ch) || isNumber(ch)) && !f->eof()){
				str += ch;
				f->get(ch);
				i++;
			}
			if(isKeyWord(str)){
				t = new KeyWord(line, col, str);
				col = i;
			} else {
				t = new Identifier(line, col, str);
				col = i;
			}
			buf = ch;
			state = (f->eof()) ? END : SUCCESS;
			break;

		case COMMENT:
			if (buf == '/')
				while((ch != '\n') && (!f->eof()))
					f->get(ch);
			else{
				while(((ch != '*') || (buf != '/')) && !f->eof()){
					col++;
					ch = buf;
					f->get(buf);
					if(buf == '\n'){
						line++;
						col = 1;
						ch = buf;
						f->get(buf);
					}
				}
				str = "";
				if(f->eof()){
					if((ch != '*') && (buf != '/')){
						state = END;
						break;
					} else 
						throw MyException("\n Unclosed multi-line comment", line, col);
				} else {
					f->get(ch);
					if(f->eof())
						return end_of_file = true;
					 else
						state = BEGIN;
					break;
					}
				
				}	

			if(f->eof())
				state = END;
			else {
				f->get(ch);
				str = "";
				line++;
				col = 1;
				state = f->eof() ? END : BEGIN;
			}
			break;

		case END:
			return last_token = true;	
		}
	return true;
}