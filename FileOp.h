/*
    Author: Shuyun Zheng
    Date: Jan 26, 2026
    Description: Declare FileOp
*/
#ifndef FILEOP_H
#define FILEOP_H

#include <filesystem>
#include <system_error>

class FileOp {
public:
    // Check if a path exists.
    static bool Exists(const std::filesystem::path& p, std::error_code& ec);
    // Check whether the path is a dir
    static bool IsDir(const std::filesystem::path& p, std::error_code& ec);
    // Create a dir at the specified path
    static bool CreateDir(const std::filesystem::path& p, std::error_code& ec);
    // Rename/move a file or dir from oldpath to newpath
    static bool RenamePath(const std::filesystem::path& oldPath,
                           const std::filesystem::path& newPath,
                           bool overwrite,
                           std::error_code& ec);
    // Delete a file or dir by recursion
    static bool DeletePath(const std::filesystem::path& p, std::error_code& ec);
    // Copy a file from src to dest, dir by recursion
    static bool CopyPath(const std::filesystem::path& src,
                         const std::filesystem::path& dest,
                         bool overwrite,
                         std::error_code& ec);
    // Move a file or dir from src to dest  
    static bool MovePath(const std::filesystem::path& src,
                         const std::filesystem::path& dest,
                         bool overwrite,
                         std::error_code& ec);

private:
    // Remove a path if exists
    static bool RemoveIfExists(const std::filesystem::path& p, std::error_code& ec);
};

#endif