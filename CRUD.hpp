#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <windows.h>

class CRUD
{
public:
    bool fileExists(const std::string &filePath)
    {
        std::ifstream inFile(filePath);
        return inFile.good();
    }

    std::string getDocumentsPath()
    {
        char *userProfile = std::getenv("USERPROFILE");
        if (userProfile)
        {
            std::ostringstream oss;
            oss << userProfile << "\\Documents\\highScore.txt";
            return oss.str();
        }
        else
        {
            throw std::runtime_error("Unable to detect USERPROFILE environment variable.");
        }
    }

    void createFile(const std::string &filePath, int highScore)
    {
        std::ofstream outFile(filePath);
        if (outFile)
        {
            outFile << "HIGHSCORE: " << highScore << "\n";
        }
        else
        {
            std::cerr << "Error creating file." << std::endl;
        }
        outFile.close();
    }

    int readFileAndExtractIntegers(const std::string &filePath)
    {
        std::ifstream inFile(filePath);
        if (!inFile)
        {
            std::cerr << "Error opening file for reading." << std::endl;
            return -1; // Return an error value
        }

        std::string line;
        int highScore = -1; // Initialize with an error value
        while (std::getline(inFile, line))
        {
            std::istringstream iss(line);
            std::string key;
            if (iss >> key && key == "HIGHSCORE:")
            {
                iss >> highScore;
                break;
            }
        }
        inFile.close();
        return highScore;
    }

    void deleteFile(const std::string &filePath)
    {
        std::remove(filePath.c_str()) == 0;
    }
};