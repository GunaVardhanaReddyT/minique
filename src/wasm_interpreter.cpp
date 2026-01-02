#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <cctype>
#include <cmath>

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
            if (!line.empty()) {
                parseLine(line);
            }
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
        
        // Otherwise evaluate as expression or variable
        std::string rest;
        std::getline(iss, rest);
        
        // Remove semicolon and whitespace
        size_t semicolon = rest.find(';');
        if (semicolon != std::string::npos) {
            rest = rest.substr(0, semicolon);
        }
        
        // Trim whitespace
        rest.erase(0, rest.find_first_not_of(" \t"));
        rest.erase(rest.find_last_not_of(" \t") + 1);
        
        if (rest.empty()) return;
        
        // Check if it's just a variable
        if (variables.count(rest)) {
            output << variables[rest] << "\n";
        } else {
            // Try to evaluate as expression
            std::istringstream exprStream(rest);
            double value = evaluateExpression(exprStream);
            output << value << "\n";
        }
    }
    
    double evaluateExpression(std::istringstream& iss) {
        std::vector<double> values;
        std::vector<char> operators;
        std::string token;
        
        while (iss >> token) {
            if (token == ";") break;
            
            if (token.size() == 1 && (token[0] == '+' || token[0] == '-' || 
                                      token[0] == '*' || token[0] == '/')) {
                operators.push_back(token[0]);
            } else if (variables.count(token)) {
                values.push_back(variables[token]);
            } else {
                try {
                    values.push_back(std::stod(token));
                } catch (...) {
                    values.push_back(0);
                }
            }
        }
        
        if (values.empty()) return 0;
        if (operators.empty()) return values[0];
        
        // Handle multiplication and division first
        for (size_t i = 0; i < operators.size(); ) {
            if (operators[i] == '*' || operators[i] == '/') {
                if (operators[i] == '*') {
                    values[i] = values[i] * values[i + 1];
                } else {
                    values[i] = values[i] / values[i + 1];
                }
                values.erase(values.begin() + i + 1);
                operators.erase(operators.begin() + i);
            } else {
                i++;
            }
        }
        
        // Handle addition and subtraction
        double result = values[0];
        for (size_t i = 0; i < operators.size(); i++) {
            if (operators[i] == '+') {
                result += values[i + 1];
            } else if (operators[i] == '-') {
                result -= values[i + 1];
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
