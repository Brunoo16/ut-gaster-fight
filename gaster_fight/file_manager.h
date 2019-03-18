#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_

#include "main.h"

class FileManager
{
	private:
		FileManager() {}

		static std::vector<std::string> missing_files;

	public:
		template<typename T>
		static void LoadFromFile(T &t, const std::string &file_name);
		static bool GetMissingFilesList(std::string &file_list);

		inline static bool IsAnyFileMissing()
		{
			return !missing_files.empty();
		}
};

#endif