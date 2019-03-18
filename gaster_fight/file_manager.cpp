#include "file_manager.h"

std::vector<std::string> FileManager::missing_files;

template <typename T>
void FileManager::LoadFromFile(T &t, const std::string &file_name)
{
	if (!t.loadFromFile(file_name))
	{
		missing_files.push_back(file_name);
	}
}

template void FileManager::LoadFromFile<sf::Image>(sf::Image &t, const std::string &file_name);
template void FileManager::LoadFromFile<sf::Texture>(sf::Texture &t, const std::string &file_name);
template void FileManager::LoadFromFile<sf::Font>(sf::Font &t, const std::string &file_name);
template void FileManager::LoadFromFile<sf::SoundBuffer>(sf::SoundBuffer &t, const std::string &file_name);

template<> void FileManager::LoadFromFile<sf::Music>(sf::Music &t, const std::string &file_name)
{
	if (!t.openFromFile(file_name))
	{
		missing_files.push_back(file_name);
	}
}

bool FileManager::GetMissingFilesList(std::string &file_list)
{
	if (missing_files.empty())
	{
		return false;
	}
	else
	{
		for (auto it = begin(missing_files); it != end(missing_files); ++it)
		{
			file_list += *it + '\n';
		}
		return true;
	}
}