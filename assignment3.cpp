#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include<climits>


void bubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j+1]
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void mergeFiles(const std::string& outputFile, const std::string& file1, const std::string& file2) {
    // Open input sort files
    std::ifstream inputFile1(file1);
    std::ifstream inputFile2(file2);

    if (!inputFile1 || !inputFile2) {
        std::cerr << "Error opening input sort files." << std::endl;
        return;
    }

    // Open the output finalsort file
    std::ofstream finalSortFile(outputFile);
    if (!finalSortFile) {
        std::cerr << "Error opening output finalsort file: " << outputFile << std::endl;
        return;
    }

    int number1, number2;

    // Read the first number from each file
    if (inputFile1 >> number1 && inputFile2 >> number2) {
        while (true) {
            // Compare the numbers and write the smaller one to the finalsort file
            if (number1 < number2) {
                finalSortFile << number1 << std::endl;
                if (inputFile1 >> number1) {
                    continue;
                } else {
                    // If no more numbers in file1, write the remaining numbers from file2
                    finalSortFile << number2 << std::endl;
                    while (inputFile2 >> number2) {
                        finalSortFile << number2 << std::endl;
                    }
                    break;
                }
            } else {
                finalSortFile << number2 << std::endl;
                if (inputFile2 >> number2) {
                    continue;
                } else {
                    // If no more numbers in file2, write the remaining numbers from file1
                    finalSortFile << number1 << std::endl;
                    while (inputFile1 >> number1) {
                        finalSortFile << number1 << std::endl;
                    }
                    break;
                }
            }
        }
    }

    // Close input sort files
    inputFile1.close();
    inputFile2.close();

    // Close the output finalsort file
    finalSortFile.close();

    std::cout << "Files merged successfully into " << outputFile << std::endl;
}

int main() {
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Generate and write 100 random numbers between 100 and 999 to the file
    std::ofstream randomNumbersFile("random.txt");
    if (!randomNumbersFile) {
        std::cerr << "Error opening the random numbers file." << std::endl;
        return 1;
    }

    for (int i = 0; i < 100; ++i) {
        int randomNumber = std::rand() % 90 + 10; // Generate a random number between 10 and 99
        randomNumbersFile << randomNumber << std::endl;
    }

    randomNumbersFile.close();

    const int numberOfFiles = 10;
    const int numbersPerFile = 10;
    const int totalNumbers = 100;

    // Read all random numbers into an array
    std::ifstream randomInputFile("random.txt");
    if (!randomInputFile) {
        std::cerr << "Error opening the random numbers file." << std::endl;
        return 1;
    }

    int allNumbers[totalNumbers];
    for (int i = 0; i < totalNumbers; ++i) {
        if (!(randomInputFile >> allNumbers[i])) {
            std::cerr << "Error reading from random numbers file." << std::endl;
            return 1;
        }
    }

    randomInputFile.close();

    // Create output files and corresponding sort files
    for (int fileCounter = 1; fileCounter <= numberOfFiles; ++fileCounter) {
        // Create the output file name
        std::string outputFileName = "output_file_" + std::to_string(fileCounter) + ".txt";
        std::ofstream outputFile(outputFileName);

        if (!outputFile) {
            std::cerr << "Error opening the output file: " << outputFileName << std::endl;
            return 1;
        }

        // Create an array to store the numbers for this output file
        int numbers[numbersPerFile];

        // Copy 10 numbers sequentially from the array to the output file
        for (int i = 0; i < numbersPerFile; ++i) {
            numbers[i] = allNumbers[(fileCounter - 1) * numbersPerFile + i];
            outputFile << numbers[i] << std::endl;
        }

        outputFile.close();

        // Sort the numbers using bubble sort
        bubbleSort(numbers, numbersPerFile);

        // Create the sort file name
        std::string sortFileName = "sort_file_" + std::to_string(fileCounter) + ".txt";
        std::ofstream sortFile(sortFileName);

        if (!sortFile) {
            std::cerr << "Error opening the sort file: " << sortFileName << std::endl;
            return 1;
        }

        // Write the sorted numbers to the sort file
        for (int i = 0; i < numbersPerFile; ++i) {
            sortFile << numbers[i] << std::endl; 
        }

        sortFile.close();

        std::cout << "Numbers in " << outputFileName << " sorted and written to " << sortFileName << std::endl;
    }


    // Merge sort files sequentially into finalsort.txt
    for (int i = 1; i <= numberOfFiles - 1; ++i) {
        std::string outputFile = "finalsort_temp.txt";
        std::string inputFile1 = (i == 1) ? "sort_file_" + std::to_string(i) + ".txt" : "finalsort.txt";
        std::string inputFile2 = "sort_file_" + std::to_string(i + 1) + ".txt";

        mergeFiles(outputFile, inputFile1, inputFile2);

        // Rename the temporary finalsort file to finalsort.txt
        std::rename(outputFile.c_str(), "finalsort.txt");
    }
   
    
    std::cin.ignore();
    return 0;
}
