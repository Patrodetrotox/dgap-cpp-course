
#include <iostream>
#include <string>
#include <vector>
#include "index.h"
#include <ctime>
#include <iomanip>
#include <algorithm>

#ifdef __linux__
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#elif defined(_WIN32)
#include <Windows.h>
#endif

#define MAX_LENGTH_OF_NAME 50
#define MAX_LENGTH_OF_PATH 100

using namespace std;

#ifdef __linux__

vector<file_info*> read_directory(string path)
{
    DIR *dirp = opendir(path.c_str());
    if (!dirp)
        return vector<file_info*>();

    vector<file_info*> vec;
    dirent *entry;
    while (entry = readdir(dirp)) {
        string name = entry->d_name;
        if (name == "." || name == "..")
            continue;
        struct stat st;
        string fullpath = path + "/" + name;
        if (stat(fullpath.c_str(), &st))
            continue; // err

        file_info *inf = new file_info;
        inf->name = name;
        inf->type = entry->d_type == DT_DIR ? ft_dir : ft_reg;
        inf->size = st.st_size;
        inf->mtime = st.st_mtim.tv_sec;
        vec.push_back(inf);
    }
    return vec;
}

#elif defined(_WIN32)

vector<file_info*> read_directory(string path)
{
	path += "/";
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile((path + '*').c_str(), &fd);
	if (hFind == INVALID_HANDLE_VALUE)
		return vector<file_info*>();
    
	vector<file_info*> vec;
	do {
		file_info *info = new file_info;
		string name = fd.cFileName;
        if (name == "." || name == "..")
            continue;

        info->name = name;
		info->mtime = fd.ftLastWriteTime.dwLowDateTime;
		info->size = fd.nFileSizeLow | fd.nFileSizeHigh << 32;
		info->type = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? ft_dir : ft_reg;
		info->path = path + name;
		vec.push_back(info);
	} while (FindNextFile(hFind, &fd));

	FindClose(hFind);
	return vec;
}


#endif

std::pair<unsigned, unsigned> MaxLength(const std::map<std::string, std::shared_ptr<file_info>>& index) {
	unsigned maxP = 0;
	unsigned maxN = 0;
	for(const auto& i: index) {
		unsigned sizeP = i.first.length();
		unsigned sizeN = i.second->name.length();
		if( maxP < sizeP) {
			maxP = sizeP;
		}
		if( maxN < sizeN) {
			maxN = sizeN;
		}
	}
	if(maxN > MAX_LENGTH_OF_NAME) {
		maxN = MAX_LENGTH_OF_NAME;
	}

	if(maxP > MAX_LENGTH_OF_PATH) {
		maxP = MAX_LENGTH_OF_PATH;
	}

	return std::make_pair(maxP, maxN);
}

void InfoOutput(shared_ptr<file_info> info, const int& max_name, const int& max_path) {
	time_t t = info->mtime;
	string strTime = ctime(&t);
	strTime.erase(strTime.find('\n'));
	cout.setf(ios::left);
	cout << setw(max_name);
	if(info->name.length() > MAX_LENGTH_OF_NAME) {
		cout << info->name.substr(0, MAX_LENGTH_OF_NAME - 3) + "...";
	} else {
		cout << info->name;
	}
	cout << "  " << setw(max_path);
	if(info->path.length() > MAX_LENGTH_OF_PATH) {
		cout << info->path.substr(0, MAX_LENGTH_OF_PATH - 3) + "...";
	} else {
		cout << info->path;
	}
	cout << "  " << setw(26) << strTime << "  " << setw(9);  //9 = max{size("directory"), size("regular")}
	info->type == file_type::ft_dir ? cout << "directory" : cout << "regular";
	cout << "  " << setw(20) << info->size << endl;  // name -- path -- time -- type -- size  // 20 - length of max uint64_t (18446744073709551615)
}

bool NameCmp(const shared_ptr<file_info>& lhs, const shared_ptr<file_info>& rhs) {
	return lhs->name < rhs->name;
}

bool SizeCmp(const shared_ptr<file_info>& lhs, const shared_ptr<file_info>& rhs) {
	return lhs->size < rhs->size;
}

bool TimeCmp(const shared_ptr<file_info>& lhs, const shared_ptr<file_info>& rhs) {
	return lhs->mtime < rhs->mtime;
}

void BuildIndex(map<string, shared_ptr<file_info>>& index, const vector<file_info*>& v, unsigned& counter) {
	for(file_info* i: v) {
		index[i->path] = shared_ptr<file_info>(i);
		if(i->type == file_type::ft_dir) {
			BuildIndex(index, read_directory(i->path), counter);
		}
		counter++;
	}
}

unsigned FileIndexer::Build() {
	unsigned counter = 0;
	BuildIndex(m_index, read_directory(m_root), counter);
	pair<unsigned, unsigned> max = MaxLength(m_index);
	m_pathLength = max.first;
	m_nameLength = max.second;
	return counter;
}

void FileIndexer::PrintFiles() {
	for(const auto& i: m_index) {
		InfoOutput(i.second, m_nameLength, m_pathLength);
	}
}

void FileIndexer::PrintFilesSorted(SortingType type) {
	vector<shared_ptr<file_info>> buf;
	for(const auto& i: m_index) {
		buf.push_back(i.second);
	}
	if(type == SortingType::Name) {
		sort(buf.begin(), buf.end(), NameCmp);
	} else if(type == SortingType::Time) {
		sort(buf.begin(), buf.end(), TimeCmp);
	} else if(type == SortingType::Size) {
		sort(buf.begin(), buf.end(), SizeCmp);
	}
	for(const shared_ptr<file_info>& i: buf) {
		InfoOutput(i, m_nameLength, m_pathLength);
	}
}

bool FileIndexer::DeleteFile_(const std::string &path) {
	if(m_index.count(path)) {
		if(m_index[path]->type == file_type::ft_dir) {
			auto it1 = m_index.lower_bound(path);
			string path2 = path;
			path2[path.length() - 1] += 1;
			auto it2 = m_index.lower_bound(path2);
			m_index.erase(it1, it2);
		} else {
			m_index.erase(path);
		}
		pair<unsigned, unsigned> max = MaxLength(m_index);
		m_pathLength = max.first;
		m_nameLength = max.second;
		return true;
	} else {
		return false;
	}
}

bool FileIndexer::MoveFile_(const std::string &path, const std::string &new_path) {
	if(m_index.count(path) > 0 && m_index.count(new_path) == 0) {
		m_index[new_path] = m_index[path];
		m_index.erase(path);
		return true;
	} else {
		return false;
	}
}

unsigned FileIndexer::FindFiles(const std::string &pattern) {
	unsigned count = 0;
	for( auto i: m_index) {
		if(i.second->path.find(pattern) != string::npos) {
			InfoOutput(i.second, m_nameLength, m_pathLength);
		}
	}
	return count;
}
