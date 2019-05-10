
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
	if(maxN > 50) {
		maxN = 50;
	}

	if(maxP > 100) {
		maxP = 100;
	}

	return std::make_pair(maxP, maxN);
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
		time_t t = i.second->mtime;
		string strTime = ctime(&t);
		strTime.erase(strTime.find('\n'));
		cout.setf(ios::left);
		cout << setw(m_nameLength);
		if(i.second->name.size() > 47) {
			cout << i.second->name.substr(0, 47) + "...";
		} else {
			cout << i.second->name;
		}
		cout << "  " << setw(m_pathLength);
		if(i.first.size() > 97) {
			cout << i.first.substr(0, 97) + "...";
		} else {
			cout << i.first;
		}
		cout << "  " << setw(26) << strTime << "  " << setw(9);  //9 = max{size("directory"), size("regular")}
		i.second->type == file_type::ft_dir ? cout << "directory" : cout << "regular";
		cout << "  " << setw(20) << i.second->size << endl;  // name -- path -- time -- type -- size  // 20 - length of max uint64_t (18446744073709551615)
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
		time_t t = i->mtime;
		string strTime = ctime(&t);
		strTime.erase(strTime.find('\n'));
		cout.setf(ios::left);
		cout << setw(m_nameLength);
		if(i->name.size() > 47) {
			cout << i->name.substr(0, 47) + "...";
		} else {
			cout << i->name;
		}
		cout << "  " << setw(m_pathLength);
		if(i->path.size() > 97) {
			cout << i->path.substr(0, 97) + "...";
		} else {
			cout << i->path;
		}
		cout << "  " << setw(26) << strTime << "  " << setw(9);
		i->type == file_type::ft_dir ? cout << "directory" : cout << "regular";
		cout << "  " << setw(20) << i->size << endl;
	}
}

bool FileIndexer::DeleteFile_(const std::string &path) {
	unsigned size = m_index.size();
	m_index.erase(path);
	if(m_index.size() != size) {
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
			time_t t = i.second->mtime;
			string strTime = ctime(&t);
			strTime.erase(strTime.find('\n'));
			cout.setf(ios::left);
			cout << setw(m_nameLength);
			if(i.second->name.size() > 47) {
				cout << i.second->name.substr(0, 47) + "...";
			} else {
				cout << i.second->name;
			}
			cout << "  " << setw(m_pathLength);
			if(i.first.size() > 97) {
				cout << i.first.substr(0, 97) + "...";
			} else {
				cout << i.first;
			}
			cout << "  " << setw(26) << strTime << "  " << setw(9);
			i.second->type == file_type::ft_dir ? cout << "directory" : cout << "regular";
			cout << "  " << setw(20) << i.second->size << endl;
			count++;
		}
	}
	return count;
}
