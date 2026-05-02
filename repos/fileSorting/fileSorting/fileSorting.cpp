#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>
#include <ctime>
#include <random>

struct SortData
{
	int countFiles;
	std::vector<int> ms;
	std::vector<int> ip;
	int level;
	int countSeparator;
	std::string originName;
};

bool createFileWithRandomNumbers(const std::string& fileName, const int numbersCount, const int maxNumberValue) {
	std::ofstream file(fileName);
	if (!file.is_open()) {
		std::cerr << "createFileWithRandomNumbers: Error - fail is not openning" << std::endl;
		return false;
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(-maxNumberValue, maxNumberValue);

	for (int i = 0; i < numbersCount; i++) {
		file << dist(gen);
		if (i < numbersCount - 1) file << '\n';
	}
	return true;
}

int MinIndex(std::vector<int> arr)
{
	int minIndex, min, f;
	minIndex = 0;
	min = arr[0];
	for (int i = 1; i <= arr.size() - 2; ++i)
	{
		f = arr[i];
		if (f < min)
		{
			min = f;
			minIndex = i;
		}
	}
	return minIndex;
}

bool IsFileContainsSortedArray(const std::string& fileName)
{
	std::ifstream originFile(fileName);
	if (!originFile.is_open())
	{
		std::cerr << "Error! File don't open!\n";
		return false;
	}
	int current, next;
	if (!(originFile >> current))
	{
		return false;
	}

	while (originFile >> next)
	{
		if (current > next)
		{
			originFile.close();
			return false;
		}
		current = next;
	}
	originFile.close();
	return true;
}

void OpenStream(std::vector<std::fstream*>& files, std::ios::openmode mode)
{
	for (int i = 0; i < files.size(); ++i)
	{
		files[i]->open("file" + std::to_string(i) + ".txt", mode);
	}
}

void CloseStream(std::vector<std::fstream*>& files)
{
	for (int i = 0; i < files.size(); ++i)
	{
		files[i]->close();
	}
}

void Splitting(SortData& data, std::ifstream& originFile, std::vector<std::fstream*>& files)
{
	data.level = 1;
	data.countSeparator = 0;
	originFile.open(data.originName);
	if (!originFile.is_open())
		return;
	for (int i = 0; i <= data.countFiles - 2; ++i)
	{
		data.ms.push_back(1);
		data.ip.push_back(1);
	}
	data.ms.push_back(0);
	data.ip.push_back(0);
	OpenStream(files, std::ios::out);
	int current, next, j = 0;
	originFile >> current;
	while (!originFile.eof())
	{
		if (current != INT_MAX)
		{
			*files[j] << current << " ";
		}
		else
			data.countSeparator += 1;
		originFile >> next;
		while (!originFile.eof() && (current <= next))
		{
			current = next;
			if (current != INT_MAX)
				*files[j] << current << " ";
			else
				data.countSeparator += 1;
			originFile >> next;
		}
		data.ms[j]--;
		*files[j] << INT_MAX << " ";
		if (data.ms[j] < data.ms[j + 1])
			++j;
		else if (data.ms[j] == 0)
		{
			++data.level;
			int ip0 = data.ip[0];
			j = 0;
			for (int k = 0; k <= data.countFiles - 2; ++k)
			{
				data.ms[k] = data.ip[k + 1] - data.ip[k] + ip0;
				data.ip[k] = data.ip[k + 1] + ip0;
			}
		}
		else
			j = 0;
		current = next;
		if (originFile.eof() && current != INT_MAX)
		{
			*files[j] << current << " " << INT_MAX;
			--data.ms[j];
		}
	}
	CloseStream(files);
	originFile.close();
}

std::vector<std::fstream*> CreateFilesVector(std::vector<std::string>& names)
{
	std::fstream* file = nullptr;
	std::vector<std::fstream*> filesContainer;
	for (int i = 0; i < names.size(); ++i)
	{
		std::string fileName = "file" + std::to_string(i) + ".txt";
		names[i] = fileName;
		file = new std::fstream(fileName);
		file->open(fileName, std::ios::out);
		filesContainer.push_back(file);
		file->close();
	}
	return filesContainer;
}

void Merging(SortData& data, std::vector<std::fstream*>& files, std::vector<std::string> namesFiles)
{
	OpenStream(files, std::ios::in);
	files[data.countFiles - 1]->close();
	files[data.countFiles - 1]->open(namesFiles[data.countFiles - 1], std::ios::out);
	while (data.level != 0)
	{
		while (*files[data.countFiles - 2] >> data.ip[data.countFiles - 2])
		{
			int haveFictitious = 1;
			for (int i = 0; i <= data.countFiles - 2; ++i)
				haveFictitious *= data.ms[i];
			while (haveFictitious)
			{
				for (int i = 0; i <= data.countFiles - 2; ++i)
				{
					--data.ms[i];
					haveFictitious *= data.ms[i];
				}
				++data.ms[data.countFiles - 1];
			}
			for (int i = 0; i <= data.countFiles - 2; ++i)
			{
				if (data.ms[i] == 0)
				{
					if (i == data.countFiles - 2)
						continue;
					*files[i] >> data.ip[i];
				}
				else if (data.ms[i] > 0)
				{
					if (i == data.countFiles - 2)
					{
						files[data.countFiles - 2]->close();
						files[data.countFiles - 2]->open(namesFiles[data.countFiles - 2], std::ios::in);
					}
					--data.ms[i];
					data.ip[i] = INT_MAX;
				}
			}
			for (; ;)
			{
				int indexMin = MinIndex(data.ip);
				if (data.ip[indexMin] == INT_MAX)
					break;
				*files[data.countFiles - 1] << data.ip[indexMin] << " ";
				*files[indexMin] >> data.ip[indexMin];
			}
			if (data.level != 1)
			{
				*files[data.countFiles - 1] << INT_MAX << " ";
			}
			else
			{
				for (int i = data.countSeparator; i > 0; --i)
				{
					*files[data.countFiles - 1] << INT_MAX << " ";
				}
			}
		}
		--data.level;
		files[data.countFiles - 1]->close();
		files[data.countFiles - 2]->close();
		files[data.countFiles - 2]->open(namesFiles[data.countFiles - 2], std::ios::out);
		if (!files[data.countFiles - 2]->is_open())
			return;
		files[data.countFiles - 1]->open(namesFiles[data.countFiles - 1], std::ios::in);
		if (!files[data.countFiles - 1]->is_open())
			return;
		for (int i = data.countFiles - 1; i > 0; --i)
		{
			std::swap(namesFiles[i], namesFiles[i - 1]);
			std::swap(files[i], files[i - 1]);
			std::swap(data.ms[i], data.ms[i - 1]);
		}
	}
	const char* sort = namesFiles[0].c_str();
	CloseStream(files);
	if (std::rename(sort, "sorted.txt"))
	{
		std::perror("Error renaming");
	}
}

void MultiphaseSort(const std::string& fileName, const int countFiles = 3)
{
	SortData data{};
	std::vector<std::string> namesFiles(countFiles);
	std::vector<std::fstream*> files = CreateFilesVector(namesFiles);
	std::ifstream originFile;
	data.countFiles = countFiles;
	data.originName = fileName;
	Splitting(data, originFile, files);
	Merging(data, files, namesFiles);
	for (int i = 0; i < countFiles; ++i)
	{
		delete files[i];
	}
}


int main()
{
	srand(time(0));
	std::string file = "originFile.txt";
	createFileWithRandomNumbers(file, 100000, 100000);
	std::clock_t start = clock();
	MultiphaseSort(file, 7);
	std::clock_t stop = clock();
	double time = double(stop - start) / double(CLOCKS_PER_SEC);
	switch (IsFileContainsSortedArray("sorted.txt"))
	{
	case true:
		std::cout << "Sorted!" << std::endl;
		std::cout << "Sorting time:" << time << std::endl;
		break;
	case false:
		std::cout << "File is not sorted!\n";
		break;
	}
	return 0;
}

