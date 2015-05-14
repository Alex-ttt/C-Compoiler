#include "scan.h"
#include "parser.h"
#include <fstream>
#include <io.h>

static string BaseName(string in){
	return in.erase(in.find('.'));
}

void ExecuteProgram(string path, string name){
	system(string("\\masm32\\bin\\ml.exe /c /coff " + name + ".asm > Assembling.log").c_str());
	string link = "\\masm32\\bin\\link.exe /SUBSYSTEM:CONSOLE ";
	int pos = name.rfind("\\");
	if(pos == -1)
		pos = 0;
	string obj = path + name.substr(pos, name.length()) + ".obj";
	string exe = path + name.substr(pos, name.length()) + ".exe";
	system(string(link + obj + " > Compilation.log").c_str());
	system(string("del " + obj).c_str());				
	system(exe.c_str());
	system(string("del " + exe).c_str());
	system("del Compilation.log");
}

int main(int argc, char *argv[]){
	ofstream *output = 0;
	argv[2] = "04.in";
	argv[1] = "-gen";

	try{
	//if(argc == 1)
	//	throw MyException("Running without parameters\nRun the compiler with \"-help\" to get some help");
	//else

	if(!strcmp(argv[1], "-help")){
		cout << "-f - write to the file + output to the screen (lexer)" << endl << 
			"-s - output to the screen (lexel)" << endl << 
			"-t - write to the file (lexer)" << endl <<
			"-pe - parse expression" << endl <<
			"-pd - parse declaration" << endl <<
			"-po - parse operators" << endl <<
			"-bp - build program" << endl << 
			"-opt - build program with high-level optimization" << endl <<
			"-gen - generate asm code" << endl <<
			"-gen_opt - generate asm code with low level optimiation" << endl << 
			"-inline_opt - build program with inline functions" << endl;
	} else 
		if(_access(argv[2], 0) == -1)
			throw MyException("File not found");
		if(!strcmp(argv[1], "-gen_opt")){
			ifstream *f = new ifstream(argv[2], ios::in);
			Scan lexer(f);
			string name(BaseName(argv[2]));
			Parser parser(lexer, new CodeGenerator(name + string(".asm")));
			parser.ParseProgram();
			parser.GenerateCode();
			parser.LowLevelOptimization();
			string path(argv[0]);
			path = path.substr(0, path.rfind("\\") + 1) + "..\\";
			parser.fflush();
			ExecuteProgram(path, name);
		} else
		if(!strcmp(argv[1], "-gen")){
			ifstream *f = new ifstream(argv[2], ios::in);
			Scan lexer(f);
			string name(BaseName(argv[2]));
			Parser parser(lexer, new CodeGenerator(name + string(".asm")));
			parser.TurnOnInline();

			parser.ParseProgram();
			//parser.HighLevelOptimization();
			//parser.SubExpressionReduce();
			parser.print();
			parser.GenerateCode();
			//parser.LowLevelOptimization();
			string path(argv[0]);
			path = path.substr(0, path.rfind("\\") + 1) + "..\\";
			parser.fflush();
			//ExecuteProgram(path, name);
		} else
		if(!strcmp(argv[1], "-inline_opt")){
			ifstream *f = new ifstream(argv[2], ios::in);
			Scan lexer(f);
			Parser parser(lexer);
			parser.TurnOnInline();
			parser.ParseProgram();
			parser.print();
			f->close();
			delete f;
		}
		else
		if(!strcmp(argv[1], "-opt")){
			ifstream *f = new ifstream(argv[2], ios::in);
			Scan lexer(f);
			Parser parser(lexer);
			parser.ParseProgram();
			parser.HighLevelOptimization();
			parser.SubExpressionReduce();
			parser.print();
			f->close();
			delete f;
		} else
		if(!strcmp(argv[1], "-bp")){
			ifstream *f = new ifstream(argv[2], ios::in);
			Scan lexer(f);
			Parser parser(lexer);
			parser.ParseProgram();
			parser.print();
			f->close();
			delete f;
		} 
		else
		if(!strcmp(argv[1], "-po")){
			ifstream *f = new ifstream;
			f->open(argv[2], ios::in);
			Scan lexer(f);
			Parser parser(lexer);
			parser.ParseProgram();
			parser.print();
		}
		else
	if(!strcmp(argv[1], "-pd")){
		ifstream *f = new ifstream;
		f->open(argv[2], ios::in);
		Scan lexer(f);
		Parser parser(lexer);
		do	
			parser.ParseDeclaration();	
		while(*lexer.Get() == T_KEYWORD);
		parser.print_declaration();
		f->close();
		delete f;
	} else

	if(!strcmp(argv[1], "-pe")){
		ifstream *f = new ifstream;
		f->open(argv[2], ios::in);
		Scan a(f);
		Parser p(a);
		p.ParseExpression()->print();
		f->close();
		delete f;
	}
	 else
		if(!strcmp(argv[1], "-s")){	
			ifstream *f = new ifstream; 
			f->open(argv[2], ios::in);
			Scan a(f);
			while(!a.isEnd()){
				a.Next();
				a.Get()->Print();
			}		
			f->close();
			delete f;

		} else 
			cout << "Invalid mode" <<endl;
	}
	catch(MyException &e){
		e.Print();
		if(output){
			e.Print(output);
			output->close();
		}
	}
	cin.get();
	return 0;
}