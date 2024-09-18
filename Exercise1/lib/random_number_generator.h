#ifndef ALGORITHM_ENGINEERING_RANDOM_NUMBER_GENERATOR_H
#define ALGORITHM_ENGINEERING_RANDOM_NUMBER_GENERATOR_H

#include <fstream>
#include <vector>

void generateRandomIntegersRAM(const std::string& filename, std::vector<double> ram_percentages);
void generateRandomIntegersFile(const std::string& filename, long long fileSizeInBytes);
long long getSystemRamSize();

std::vector<int> readIntegersFromFile(const std::string& filename);

std::vector<int> gen_random_integers(long long n_integers, int min);


#endif //ALGORITHM_ENGINEERING_RANDOM_NUMBER_GENERATOR_H
