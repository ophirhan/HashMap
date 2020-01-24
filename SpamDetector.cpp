#include <boost/tokenizer.hpp>
//#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>#
#include <string>
#include <iostream>
#include <fstream>
#include "HashMap.hpp"

#define INVALID_INPUT "Invalid input"


bool isWholeNumber(const std::string &basicString)
{
    if(basicString.length()==0)
    {
        return false;
    }
    if(basicString.length() > 1 && basicString[0] == '0')
    {
        return false;
    }
    for(char c:basicString)
    {
        if(!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

class SpamDetector
{
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
public:
    explicit SpamDetector(std::ifstream &database)
    {
        _map = new HashMap<std::string, int>();
        boost::char_separator<char> sep{","};
        std::string line;
        while(std::getline(database, line))
        {
            if(line.find_first_of(',') != line.find_last_of(','))
            {
                throw std::exception();
            }
            tokenizer tok{line, sep};
            int counter = 0, weight = 0;
            std::string expression;
            auto current = tok.begin();
            while (current != tok.end())
            {
                if(counter == 0)
                {
                    expression = *current;
                    boost::algorithm::to_lower(expression);
                }
                else
                {
                    if(!isWholeNumber(*current))
                    {
                        throw std::exception();
                    }
                    weight = stoi(*current);
                }
                ++current;
                counter++;
            }
            if (counter != 2 || expression.empty() || weight < 0)
            {
                throw std::exception();
            }
            _map->insert(expression,weight);
        }
    }

    void detect(std::ifstream &messageFile, int threshold)
    {
        int score = 0;
        std::string message;
        std::string line;
        while(std::getline(messageFile, line))
        {
            message += line + "\n";
        }
        boost::algorithm::to_lower(message);
        for(const auto &i: *_map)
        {
            size_t index = message.find(i.first);
            while(index != std::string::npos)
            {
                score += i.second;
                index = message.find(i.first, index + 1);
            }
        }
        if(score >= threshold)
        {
            std::cout << "SPAM" << std::endl;
        }
        else
        {
            std::cout << "NOT_SPAM" << std::endl;
        }
    }

private:
    HashMap<std::string, int> *_map{};


};
int main(const int argc, const char **argv)
{
    if(argc != 4)//todo including SpamDetector or not?
    {
        std::cerr << "Usage: SpamDetector <databaseFile path> <messageFile path> <threshold>" << std::endl;
        return EXIT_FAILURE;
    }
    int threshold = 0;
    try
    {
        threshold = std::stoi(argv[3]);
    }
    catch(std::exception &e)
    {
        std::cerr << INVALID_INPUT << std::endl;
        return EXIT_FAILURE;
    }
    if (threshold < 1 || !isWholeNumber(argv[3]))
    {
        std::cerr << INVALID_INPUT << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream databaseFile(argv[1]);
    if (!databaseFile.is_open()) // File doesn't exist
    {
        std::cerr << INVALID_INPUT << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream messageFile(argv[2]);
    if (!messageFile.is_open()) // File doesn't exist
    {
        databaseFile.close();
        std::cerr << INVALID_INPUT << std::endl;
        return EXIT_FAILURE;
    }
    try
    {
        SpamDetector detector(databaseFile);
        detector.detect(messageFile, threshold);
    }
    catch (std::exception &e)
    {
        databaseFile.close();
        messageFile.close();
        std::cerr << INVALID_INPUT << std::endl;
        return EXIT_FAILURE;
    }
    databaseFile.close();
    messageFile.close();
    return EXIT_SUCCESS;
}