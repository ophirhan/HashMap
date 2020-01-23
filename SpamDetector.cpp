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
        readDatabase(database);
    }

    void readDatabase(std::ifstream &database)
    {
        _map = new HashMap<std::string, int>();boost::char_separator<char> sep{","};
        std::string line;
        while (std::getline(database, line))
        {
            tokenizer tok{line, sep};
            int counter = 0, fractalType = 0, fractalDimension = 0;
            for (auto current = tok.begin(); current != tok.end(); current++, counter++)
            {
                switch (counter)
                {
                    case 0:
                    {
                        fractalType = checkString(*current, MAX_TYPE);
                        break;
                    }
                    case 1:
                    {
                        fractalDimension = checkString(*current, MAX_DIMENSION);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            if (counter != 2 || fractalType == -1 || fractalDimension == -1)
            {
                return false;
            }
            results.insert(results.begin(),
                           FractalFactory::createFractal(fractalType, fractalDimension));
        }
    }

    void detect(const std::ifstream &messageFile, int threshold)
    {

    }

private:
    HashMap<std::string, int> *_map;


};
int main(const int argc, const char **argv)
{
    if(argc != 4)//todo including SpamDetector or not?
    {
        std::cerr << "Usage: SpamDetector <databaseFile path> <messageFile path> <threshold>" << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream databaseFile(argv[1]);
    std::ifstream messageFile(argv[2]);
    if (!databaseFile.is_open() || !messageFile.is_open()) // File doesn't exist
    {
        std::cerr << INVALID_INPUT << std::endl;
    }
    int threshold = std::stoi(argv[3]);
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
    return EXIT_SUCCESS;
}

typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
class Database
{
    Database(std::ifstream &fileStream, const std::string &filename)
    {
        boost::char_separator<char> sep{","};
        std::string line;
        while (std::getline(fileStream, line))
        {
            tokenizer tok{line, sep};
            int counter = 0, fractalType = 0, fractalDimension = 0;
            for (auto current = tok.begin(); current != tok.end(); current++, counter++)
            {
                switch (counter)
                {
                    case 0:
                    {
                        fractalType = checkString(*current, MAX_TYPE);
                        break;
                    }
                    case 1:
                    {
                        fractalDimension = checkString(*current, MAX_DIMENSION);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            if (counter != 2 || fractalType == -1 || fractalDimension == -1)
            {
                return false;
            }
            results.insert(results.begin(),
                           FractalFactory::createFractal(fractalType, fractalDimension));
        }
        return true;
    }
};