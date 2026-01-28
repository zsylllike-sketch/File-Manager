/*
    Author: Shuyun Zheng
    Date: Jan 26, 2026
    Description: Implementation of file operations used by wxWidgets file manager.
*/
#include <filesystem>
#include "FileOp.h"
/* Check if a path exists.
* @param p The path to check.
* @param ec Error code to capture any filesystem errors.
* @return true if the path exists, false otherwise.
*/
bool FileOp::Exists(const std::filesystem::path& p, std::error_code& ec){
    return std::filesystem::exists(p, ec);
}
/* Check if a path is a directory.
* @param p The path to check.
* @param ec Error code to capture any filesystem errors.
* @return true if the path is a directory, false otherwise.
*/
bool FileOp::IsDir(const std::filesystem::path& p, std::error_code& ec){
    return std::filesystem::is_directory(p, ec);
}
/* Create a directory at the specified path.
* @param p The path where the directory should be created.
* @param ec Error code to capture any filesystem errors.
* @return true if the directory was created successfully, false otherwise.
*/
bool FileOp::CreateDir(const std::filesystem::path& p, std::error_code& ec){
    std::filesystem::create_directory(p, ec);
    return !ec;
}
/* Rename a file or directory from oldPath to newPath.
* @param oldPath The current path of the file or directory.
* @param newPath The new path for the file or directory.
* @param overwrite If true, overwrite the destination if it exists.
* @param ec Error code to capture any filesystem errors.
* @return true if the rename was successful, false otherwise.
*/
bool FileOp::RenamePath(const std::filesystem::path& oldPath,
                        const std::filesystem::path& newPath,
                        bool overwrite,
                        std::error_code& ec){
    ec.clear();
    if (overwrite && std::filesystem::exists(newPath, ec)) {
        std::filesystem::remove_all(newPath, ec);
        if (ec) return false;
    }
    std::filesystem::rename(oldPath, newPath, ec);
    return !ec;
}
/* Delete a file or directory at the specified path.
* @param p The path to delete.
* @param ec Error code to capture any filesystem errors.
* @return true if the deletion was successful, false otherwise.
*/
bool FileOp::DeletePath(const std::filesystem::path& p, std::error_code& ec){
    ec.clear();
    std::filesystem::remove_all(p, ec);
    return !ec;
}
/* Copy a file or directory from source to destination.
* @param src The source path.
* @param dest The destination path.
* @param overwrite If true, overwrite the destination if it exists.
* @param ec Error code to capture any filesystem errors.
* @return true if the copy was successful, false otherwise.
*/
bool FileOp::CopyPath(const std::filesystem::path& src,
                      const std::filesystem::path& dest,
                      bool overwrite,
                      std::error_code& ec) {
    ec.clear();
    // If overwrite is true and destination exists, remove it first
    if (overwrite && std::filesystem::exists(dest, ec)) {
        std::filesystem::remove_all(dest, ec);
        if (ec) return false;
    }
    // Dir or file copy
    if (std::filesystem::is_directory(src, ec)) {
        std::filesystem::copy(src, dest, std::filesystem::copy_options::recursive, ec);
    } else {
        auto option = overwrite ? std::filesystem::copy_options::overwrite_existing
                     : std::filesystem::copy_options::none;
        std::filesystem::copy_file(src, dest, option, ec);
    }
    return !ec;
}
/* Move a file or directory from source to destination.
* @param src The source path.
* @param dest The destination path.
* @param overwrite If true, overwrite the destination if it exists.
* @param ec Error code to capture any filesystem errors.
* @return true if the move was successful, false otherwise.
*/
bool FileOp::MovePath(const std::filesystem::path& src,
                      const std::filesystem::path& dest,
                      bool overwrite,
                      std::error_code& ec) {
    if (overwrite && std::filesystem::exists(dest, ec)) {
        std::filesystem::remove_all(dest, ec);
        if (ec) return false;
    }
    std::filesystem::rename(src, dest, ec);
    return !ec;
}
/* Remove a file or directory if it exists.
* @param p The path to remove.
* @param ec Error code to capture any filesystem errors.  
*/
bool FileOp::RemoveIfExists(const std::filesystem::path& p, std::error_code& ec)
{
    ec.clear();
    if (!std::filesystem::exists(p, ec) || ec) return !ec;
    std::filesystem::remove_all(p, ec);
    return !ec;
}