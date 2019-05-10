
#include <string>
#include <map>
#include <memory>

enum file_type
{
    ft_dir,
    ft_reg,
};

struct file_info {
    std::string name;
    std::string path;
    file_type type;
    uint64_t size;
    uint64_t mtime;
};

class FileIndexer
{
public:
    FileIndexer(const std::string &rootPath): m_root(rootPath),
	m_pathLength(0),
	m_nameLength(0){

    }

    ~FileIndexer() {

    }

    /*
     * Build deep file index starting from root dir.
     * Put info about files into map: "Full path" -> file_info.
     * Store file_info as shared_ptr. Do not use pure pointers inside this class.
     * Return number of files found.
     */
    unsigned Build();

    enum SortingType {
        Name,
        Size,
        Time,
    };

    /*
     * Print file list to cout unsorted/sorted.
     * Use table-view to print all info about files.
     */
    void PrintFiles();
    void PrintFilesSorted(SortingType type);

    /*
     * Find file paths by pattern and print them unsorted.
     * Return number of found files.
     */
    unsigned FindFiles(const std::string &pattern);

    /* 
     * Move file selected by path.
     * Return status: success or fail.
     */
    bool MoveFile_(const std::string &path, const std::string &new_path);

    /* 
     * Delete file selected by path.
     * Return status: success or fail.
     */
    bool DeleteFile_(const std::string &path);

private:
    std::string m_root;
    std::map<std::string, std::shared_ptr<file_info>> m_index;
    unsigned m_pathLength;
    unsigned m_nameLength;
    /* ... */
};

