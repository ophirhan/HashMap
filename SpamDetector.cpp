#include <boost/tokenizer.hpp>
//#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include "HashMap.hpp"

#define INVALID_INPUT "Invalid input"

/**
 * @brief Checks if a given string represents a none-negative integer.
 * @param basicString string to check.
 * @return true if the string is a none-negative integer, false otherwies.
 */
bool isNoneNegativeInteger(const std::string &basicString)
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

/**
 * @brief SpamDetector object for SpamDetector project.
 */
class SpamDetector
{
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
public:

    /**
     * @brief SpamDetector constructor.
     * @param database input fileStream where each line contains
     * a spam expression and its weight separated by a comma
     */
    explicit SpamDetector(std::ifstream &database)
    {
        _map = new HashMap<std::string, int>();
        boost::char_separator<char> sep{","};
        std::string line;
        while(std::getline(database, line))
        {
            if(line.find_first_of(',') != line.find_last_of(','))
            {
                delete(_map);
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
                        delete(_map);
                        throw std::exception();
                    }
                    weight = stoi(*current);
                }
                ++current;
                counter++;
            }
            if (counter != 2 || expression.empty() || weight < 0)
            {
                delete(_map);
                throw std::exception();
            }
            _map->insert(expression,weight);
        }
    }

    /**
     * @brief checks if a given message is considered as spam
     * according to the object database and a given threshold.
     * prints SPAM if it is, prints NOT_SPAM otherwise.
     * @param messageFile inputFileStream to check if it is spam.
     * @param threshold positive number indicating minimum value to be considered as spam.
     */
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

    /**
     * @brief SpamDetector destructor.
     */
    ~SpamDetector()
    {
        delete(_map);
    }

private:
    HashMap<std::string, int> *_map{};


};

/**
 * @brief Main function of SpamDetector object for SpamDetector project.
 * parses database filename, message filename and a threshold value and prints to the screen
 * a message indicating if the given message was spam or not.
 * @param argc number of given arguments.
 * @param argv array of arguments.
 * @return EXIT_SUCCESS if input was valid, EXIT_FAILURE otherwise.
 */
int main(const int argc, const char **argv)
{
    if(argc != 4)//todo including SpamDetector or not?
    {
        std::cerr << "Usage: SpamDetector <database path> <message path> <threshold>" << std::endl;
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

/**
 * @brief Exception for SpamDetector project.
 */
class SpamDetectorException: std::exception
{

};