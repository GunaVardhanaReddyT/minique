#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>

// Simple interpreter that doesn't need LLVM
class MiniqueInterpreter {
private:
    std::map<std::string, double> variables;
    std::stringstream output;
    
public:
    std::string execute(const std::string& code) {
        output.str("");
        output.clear();
        
        std::istringstream iss(code);
        std::string line;
        
        while (std::getline(iss, line)) {
            parseLine(line);
        }
        
        return output.str();
    }
    
private:
    void parseLine(const std::string& line) {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;
        
        if (keyword == "set") {
            parseSet(iss);
        } else if (keyword == "show") {
            parseShow(iss, line);
        }
    }
    
    void parseSet(std::istringstream& iss) {
        std::string varName;
        std::string equals;
        iss >> varName >> equals;
        
        double value = evaluateExpression(iss);
        variables[varName] = value;
    }
    
    void parseShow(std::istringstream& iss, const std::string& line) {
        // Check if it's a string literal
        size_t firstQuote = line.find('"');
        if (firstQuote != std::string::npos) {
            size_t secondQuote = line.find('"', firstQuote + 1);
            if (secondQuote != std::string::npos) {
                std::string str = line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                output << str << "\n";
                return;
            }
        }
        
        // Otherwise evaluate as expression
        double value = evaluateExpression(iss);
        output << value << "\n";
    }
    
    double evaluateExpression(std::istringstream& iss) {
        std::string token;
        std::vector<double> values;
        std::vector<char> operators;
        
        while (iss >> token) {
            if (token == ";") break;
            
            if (token == "+" || token == "-" || token == "*" || token == "/") {
                operators.push_back(token[0]);
            } else if (variables.count(token)) {
                values.push_back(variables[token]);
            } else {
                values.push_back(std::stod(token));
            }
        }
        
        if (values.empty()) return 0;
        if (operators.empty()) return values[0];
        
        double result = values[0];
        for (size_t i = 0; i < operators.size(); i++) {
            switch (operators[i]) {
                case '+': result += values[i + 1]; break;
                case '-': result -= values[i + 1]; break;
                case '*': result *= values[i + 1]; break;
                case '/': result /= values[i + 1]; break;
            }
        }
        
        return result;
    }
};

extern "C" {
    const char* compile_minique(const char* source_code) {
        try {
            MiniqueInterpreter interpreter;
            std::string result = interpreter.execute(source_code);
            
            static std::string output;
            output = result;
            return output.c_str();
        }
        catch (const std::exception& e) {
            static std::string error;
            error = std::string("Error: ") + e.what();
            return error.c_str();
        }
    }
}
