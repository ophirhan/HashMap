#include <boost/tokenizer.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include "HashMap.hpp"

#define INVALID_INPUT "Invalid input"

#define EXPECTED_NUM_OF_ARGS 4

#define THRESHOLD_ARG_NUM 3

#define DATABASE_ARG_NUM 1

#define MESSAGE_ARG_NUM 2

#define EXACTLY_TWO_COLS "Only exactly two columns allowed"

/**
 * @brief Turns all letter in a string to lowercase, in place.
 * @param str the string to change.
 */
void toLowerCase(std::string &str)
{
    for (char &c : str)
    {
        c = (char) tolower(c);
    }
}

/**
 * @brief Checks if a given string represents a none-negative integer.
 * @param basicString string to check.
 * @return true if the string is a none-negative integer, false otherwies.
 */
bool isNoneNegativeInteger(const std::string &basicString)
{
    if (basicString.length() == 0)
    {
        return false;
    }
    if (basicString.length() > 1 && basicString[0] == '0')
    {
        return false;
    }
    for (char c:basicString)
    {
        if (!isdigit(c))
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
        while (std::getline(database, line))
        {
            if (line.find_first_of(',') != line.find_last_of(','))
            {
                delete (_map);
                throw std::invalid_argument(EXACTLY_TWO_COLS);
            }
            tokenizer tok{line, sep};
            int counter = 0, weight = 0;
            std::string expression;
            auto current = tok.begin();
            while (current != tok.end())
            {
                counter++;
                if (counter == 1)
                {
                    expression = *current;
                    toLowerCase(expression);
                    if (expression.empty())
                    {
                        delete (_map);
                        throw std::invalid_argument("Expression must not be empty");
                    }
                }
                else if (counter == 2)
                {
                    if (!isNoneNegativeInteger(*current))
                    {
                        delete (_map);
                        throw std::invalid_argument(
                                "Only none negative integers allowed as weights");
                    }
                    weight = stoi(*current);
                }
                else
                {
                    break;
                }
                ++current;
            }
            if (counter != 2)
            {
                delete (_map);
                throw std::invalid_argument(EXACTLY_TWO_COLS);
            }
            _map->insert(expression, weight);
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
        while (std::getline(messageFile, line))
        {
            message += line + "\n";
        }

        toLowerCase(message);
        for (const auto &i: *_map)
        {
            size_t index = message.find(i.first);
            while (index != std::string::npos)
            {
                score += i.second;
                index = message.find(i.first, index + 1);
            }
        }
        if (score >= threshold)
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
        delete (_map);
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
    if (argc != EXPECTED_NUM_OF_ARGS)//todo including SpamDetector or not?
    {
        std::cerr << "Usage: SpamDetector <database path> <message path> <threshold>" << std::endl;
        return EXIT_FAILURE;
    }
    int threshold = 0;
    try
    {
        threshold = std::stoi(argv[THRESHOLD_ARG_NUM]);
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << INVALID_INPUT << std::endl;
        return EXIT_FAILURE;
    }
    if (threshold < 1 || !isNoneNegativeInteger(argv[THRESHOLD_ARG_NUM]))
    {
        std::cerr << INVALID_INPUT << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream databaseFile(argv[DATABASE_ARG_NUM]);
    if (!databaseFile.is_open()) // File doesn't exist
    {
        std::cerr << INVALID_INPUT << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream messageFile(argv[MESSAGE_ARG_NUM]);
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
    catch (std::invalid_argument &e)
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