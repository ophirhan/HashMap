#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
//#include <string>
#include <iostream>
//#include <fstream>
#include "HashMap.hpp"

#define INVALID_INPUT "Invalid input"

class SpamDetector
{
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
public:
    explicit SpamDetector(std::ifstream &database)
    {
        _map = new HashMap<std::string, int>();boost::char_separator<char> sep{","};
        std::string line;
        while(std::getline(database, line))
        {
            tokenizer tok{line, sep};
            int counter = 0, weight = 0;
            std::string expression;
            for (auto current = tok.begin(); current != tok.end(); current++, counter++)
            {
                switch (counter)
                {
                    case 0:
                    {
                        expression = *current;
                        break;
                    }
                    case 1:
                    {
                        weight = stoi(*current);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
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
        for(auto &i: *_map)
        {
            int index = 0;
            message.find(i.first);
            while(index != std::string::npos)
            {
                score += i.second;
                index = message.find(i.first, index);
            }
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
    if (threshold < 1)
    {
        throw std::exception();
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