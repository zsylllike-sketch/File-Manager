/*
    Author: Shuyun Zheng
    Date: Jan 26, 2026
    Description: Implement UI part
*/
#include <chrono>
#include <filesystem>

#include <wx/msgdlg.h>
#include <wx/utils.h>

#include "FileOp.h"
#include "MainFrame.h"
/* Convert a machine time to a readable time for Date Modified
* @param ftime: System time
* @return wxStirng: Formatted time
*/
static wxString FormatFileTime(const std::filesystem::file_time_type& ftime) {
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(ftime - std::filesystem::file_time_type::clock::now()
        + system_clock::now());
    std::time_t cftime = system_clock::to_time_t(sctp); 
    std::tm* timeinfo = std::localtime(&cftime);
    wxString formattedTime = wxString::Format("%04d-%02d-%02d %02d:%02d:%02d",
        timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
        timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    return formattedTime;
}
/* Create the main window and bind events
* @param title
*/
MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
    // Initialize and shortcuts
    CreateStatusBar();
    SetStatusText("Ready");
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(ID_NewDir, "&New...\tCtrl-N");
    fileMenu->Append(ID_Open,  "&Open...\tCtrl-O");
    fileMenu->Append(ID_Rename,"&Rename...\tCtrl-E");
    fileMenu->Append(ID_Delete,"&Delete...\tDEL");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT,"E&xit\tCtrl-Q");

    wxMenu* editMenu = new wxMenu();
    editMenu->Append(ID_Copy,  "&Copy\tCtrl-C");
    editMenu->Append(ID_Cut,   "Cu&t\tCtrl-X");
    editMenu->Append(ID_Paste, "&Paste\tCtrl-V");

    wxMenu* viewMenu = new wxMenu();
    viewMenu->Append(ID_Refresh, "&Refresh\tF5");

    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(ID_About, "&About");

    wxMenuBar* menubar = new wxMenuBar();
    menubar->Append(fileMenu, "&File");
    menubar->Append(editMenu, "&Edit");
    menubar->Append(viewMenu, "&View");
    menubar->Append(helpMenu, "&Help");
    SetMenuBar(menubar);
    // Create UI components
    wxPanel* panel = new wxPanel(this);
    m_pathBar = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_fileList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    SetupListColumns();
  
    m_pathBar->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnPathEnter, this);
    m_fileList->Bind(wxEVT_LIST_ITEM_ACTIVATED, &MainFrame::OnFileActivated, this);

    // Bind events
    Bind(wxEVT_MENU, &MainFrame::OnNewDir, this, ID_NewDir);
    Bind(wxEVT_MENU, &MainFrame::OnOpen,   this, ID_Open);
    Bind(wxEVT_MENU, &MainFrame::OnRename, this, ID_Rename);
    Bind(wxEVT_MENU, &MainFrame::OnDelete, this, ID_Delete);

    Bind(wxEVT_MENU, &MainFrame::OnCopy,   this, ID_Copy);
    Bind(wxEVT_MENU, &MainFrame::OnCut,    this, ID_Cut);
    Bind(wxEVT_MENU, &MainFrame::OnPaste,  this, ID_Paste);

    Bind(wxEVT_MENU, &MainFrame::OnRefresh,this, ID_Refresh);
    Bind(wxEVT_MENU, &MainFrame::OnAbout,  this, ID_About);
    Bind(wxEVT_MENU, &MainFrame::OnExit,   this, wxID_EXIT);
    
    // Sizer
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_pathBar, 0, wxEXPAND | wxALL, 5);
    sizer->Add(m_fileList, 1, wxEXPAND | wxALL, 5);
    panel->SetSizer(sizer);

    // Get current file info
    currentPath_ = std::filesystem::current_path();
    RefreshFileList(currentPath_);
}
/* Read selection and return the corresponding path
* @param outPath: ouput receiving the selected path
* @return true if the selection exists
*/
bool MainFrame::TryGetSelectedPath(std::filesystem::path& outPath) const {
    long item = m_fileList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item == -1 || item >=(long)(rowPaths_.size())) {
        return false;
    }
    outPath = rowPaths_[item];
    return true;
}
/* Prompting the user for confirmation before deletion
* @param dest: destination path
* @return true if user wants to overwrite/dest dne
*/
bool MainFrame::ConfirmOverwriteIfExists(const std::filesystem::path& dest) {
    std::error_code ec;
    if (std::filesystem::exists(dest, ec)) {
        int response = wxMessageBox("The file or directory \"" + wxString(dest.filename().wstring()) +
                                    "\" already exists. Do you want to overwrite it?",
                                    "Confirm Overwrite",
                                    wxYES_NO | wxICON_QUESTION,
                                    this);
        return response == wxYES;
    }
    return true; // No existing file, no need to confirm
}
/* The ability to display properties of files including name, type (file or directory), size, and date modified.
* @return void
*/
void MainFrame::SetupListColumns() {
    // Insert columns for file properties
    m_fileList->InsertColumn(0, _("Name"), wxLIST_FORMAT_LEFT, 250);
    m_fileList->InsertColumn(1, _("Type"), wxLIST_FORMAT_LEFT, 100);
    m_fileList->InsertColumn(2, _("Size"), wxLIST_FORMAT_RIGHT, 100);
    m_fileList->InsertColumn(3, _("Date Modified"), wxLIST_FORMAT_LEFT, 150);
}

// Menu
/* Handle exit menu
* @param event
* @return void
*/
void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}
/* Refresh the list
* @param event
* @return void
*/
void MainFrame::OnRefresh(wxCommandEvent& event) {
    RefreshFileList(currentPath_);
}
/* Display the program info
* @param event
* @return void
*/
void MainFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("File Manager\n"
                 "Using wxWidgets and C++17\n"  ); 
}
/* Path, status bar handle
* @return void
*/
void MainFrame::UpdatePathUI() {
    if (m_pathBar) {
        wxString p = wxString(currentPath_.string());
        m_pathBar->ChangeValue(p);
        SetStatusText(p);
        SetTitle("File Manager - " + p);
    }
}
/* Validates the dir, and add entry for parent navi
* @param path: Dir path
*/
void MainFrame::RefreshFileList(const std::filesystem::path& path) {
    // Error for filesystem
    std::error_code ec;
    if (!std::filesystem::exists(path, ec) || !std::filesystem::is_directory(path, ec)) {
        wxMessageBox("Invalid directory:\n" + wxString(path.string()),
                     "Error",
                     wxOK | wxICON_ERROR,
                     this);
        return;
    }
    currentPath_ = path;
    UpdatePathUI();
    m_fileList->DeleteAllItems();
    rowPaths_.clear(); // Clear previous paths

    // Go back to parent directory
    if (currentPath_ != currentPath_.root_path()) {
        long row = m_fileList->InsertItem(m_fileList->GetItemCount(), "..");
        m_fileList->SetItem(row, 1, "Dir");
        m_fileList->SetItem(row, 2, "");
        m_fileList->SetItem(row, 3, "");
        rowPaths_.push_back(currentPath_.parent_path()); // Store parent path
    }

    // Iterate through directory entries
    for (const auto& entry : std::filesystem::directory_iterator(path, ec)) {
        if(ec)break;
        // Get file path and name
        const auto p = entry.path();
        wxString name = p.filename().wstring();
        // Insert
        long row = m_fileList->InsertItem(m_fileList->GetItemCount(), name);
        // Store path to the row
        rowPaths_.push_back(p);
        std::error_code ec2;
        // Type
        const bool isDir = entry.is_directory(ec2);
        if (ec2) {
            // If cannot determine the type, mark unknown but still keep a row
            m_fileList->SetItem(row, 1, "N/A");
            m_fileList->SetItem(row, 2, "N/A");
            m_fileList->SetItem(row, 3, "N/A");
            continue;
        }
        m_fileList->SetItem(row, 1, isDir ? "Dir" : "File");
        // File Size
        if (!isDir) {
            const auto size = entry.file_size(ec2);
            if (!ec2) {
                m_fileList->SetItem(row, 2,
                    wxString::Format("%llu bytes", static_cast<unsigned long long>(size)));
            } else {
                m_fileList->SetItem(row, 2, "N/A");
            }
        } else {
            m_fileList->SetItem(row, 2, "");
        }
        // Update modified time
        auto mt = entry.last_write_time(ec2);
        if (!ec2) {
            m_fileList->SetItem(row, 3, FormatFileTime(mt));
        }else{
            m_fileList->SetItem(row, 3, "N/A");
        }
    }
    // Error handle
    if (ec){
        wxMessageBox("Failed to list directory:\n" + wxString(currentPath_.wstring()) +
                        "\n" + wxString(ec.message()),
                    "Error",
                    wxOK | wxICON_ERROR,
                    this);   
    }     
}
/* Handle path input
* @param event
*/
void MainFrame::OnPathEnter(wxCommandEvent& event) {
    wxString pathStr = m_pathBar->GetValue();
    std::filesystem::path newPath(pathStr.ToStdWstring());
    std::error_code ec;
    if (std::filesystem::exists(newPath, ec) && std::filesystem::is_directory(newPath, ec)) {
        RefreshFileList(newPath);
    } else {
        wxMessageBox("Invalid path:\n" + pathStr,
                     "Error",
                     wxOK | wxICON_ERROR,
                     this);
        UpdatePathUI(); // Revert to current path
    }
}
/* Handle file/directory activation
* @param event
*/
void MainFrame::OnFileActivated(wxListEvent& event) {
    long itemIndex = event.GetIndex();
    if (itemIndex < 0 || itemIndex >= (long)(rowPaths_.size())) {
        return; // Invalid index
    }
    std::filesystem::path selectedPath = rowPaths_[itemIndex];
    std::error_code ec;
    if (std::filesystem::is_directory(selectedPath, ec)) {
        RefreshFileList(selectedPath);
        return;
    } else {
        // Open file with default application
        wxString filePathStr = selectedPath.wstring();
        if (!wxLaunchDefaultApplication(filePathStr)) {
            wxMessageBox("Failed to open file:\n" + filePathStr,
                         "Error",
                         wxOK | wxICON_ERROR,
                         this);
        }
    }
}

/* Opens the selected item
* @param event 
* @return void
*/
void MainFrame::OnOpen(wxCommandEvent& event) {
    std::filesystem::path selectedPath;
    if (!TryGetSelectedPath(selectedPath)) {
        wxMessageBox("No file or directory selected to open.",
                     "Error",
                     wxOK | wxICON_ERROR,
                     this);
        return;
    }
    std::error_code ec;
    if (std::filesystem::is_directory(selectedPath, ec)) {
        RefreshFileList(selectedPath);
    } else {
        // Open file with default application
        wxString filePathStr = selectedPath.wstring();
        if (!wxLaunchDefaultApplication(filePathStr)) {
            wxMessageBox("Failed to open file:\n" + filePathStr,
                         "Error",
                         wxOK | wxICON_ERROR,
                         this);
        }
    }
}
/* Propmt user for a dir name and create a new one
* @param event
* @return void
*/
void MainFrame::OnNewDir(wxCommandEvent& event) {
    wxTextEntryDialog dialog(this, "Enter new directory name:", "New Directory");
    if (dialog.ShowModal() == wxID_OK) {
        wxString dirName = dialog.GetValue();
        std::filesystem::path newDirPath = currentPath_ / dirName.ToStdWstring();
        std::error_code ec;
        if (FileOp::CreateDir(newDirPath, ec)) {
            SetStatusText("Created directory: " + wxString(newDirPath.wstring()));
            RefreshFileList(currentPath_);
        } else {
            wxMessageBox("Failed to create directory:\n" + wxString(newDirPath.wstring()),
                         "Error",
                         wxOK | wxICON_ERROR,
                         this);
        }
    }
}
/* Prompt user for a new name and rename the selected file or dir. If already exists, ask for confirmation
* @param event
* @return void
*/
void MainFrame::OnRename(wxCommandEvent& event) {
    std::filesystem::path selectedPath;
    if (!TryGetSelectedPath(selectedPath)) {
        wxMessageBox("No file or directory selected to rename.",
                     "Error",
                     wxOK | wxICON_ERROR,
                     this);
        return;                 
    }
    wxTextEntryDialog dialog(this, "Enter new name:", "Rename",
                             wxString(selectedPath.filename().wstring()));
    if (dialog.ShowModal() == wxID_OK) {
        wxString newName = dialog.GetValue();
        std::filesystem::path newPath = selectedPath.parent_path() / newName.ToStdWstring();
        bool overwrite = false;
        std::error_code ec;
        if (FileOp::Exists(newPath, ec) && !ec) {
            if (!ConfirmOverwriteIfExists(newPath)) return;
            overwrite = true;
        }
        if (!FileOp::RenamePath(selectedPath, newPath, overwrite, ec)) {
            wxMessageBox("Rename failed:\n" + wxString(ec.message()),
                     "Error", wxOK | wxICON_ERROR, this);
            return;
        }
        SetStatusText("Renamed to: " + wxString(newPath.wstring()));
        RefreshFileList(currentPath_);
    }
    else return;
}
/* Ask for confirmation for the selected file or dir, then delete
* @param event
* @return void
*/
void MainFrame::OnDelete(wxCommandEvent& event) {
    std::filesystem::path selectedPath;
    if (!TryGetSelectedPath(selectedPath)) {
        wxMessageBox("No file or directory selected to delete.",
                     "Error",
                     wxOK | wxICON_ERROR,
                     this);
        return;
    }
    int answer = wxMessageBox("Are you sure you want to delete:\n" +
                            wxString(selectedPath.wstring()) + "?",
                            "Confirm Deletion",
                            wxYES_NO | wxICON_QUESTION,
                            this);
    if (answer != wxYES) return;
    std::error_code ec;
    if (!FileOp::DeletePath(selectedPath, ec)) {
        wxMessageBox("Delete failed:\n" + wxString(ec.message()),
                     "Error",
                     wxOK | wxICON_ERROR,
                     this);
        return;
    }
    SetStatusText("Deleted: " + wxString(selectedPath.wstring()));
    RefreshFileList(currentPath_);
}
/* Mark the selected file in virtual clipboard
* @param event
* @return void
*/
void MainFrame::OnCopy(wxCommandEvent& event) {
    std::filesystem::path selectedPath;
    if (!TryGetSelectedPath(selectedPath)) {
        wxMessageBox("No file or directory selected to copy.",
                     "Error",
                     wxOK | wxICON_ERROR,
                     this);
        return;
    }
    clipboardPath_ = selectedPath;
    clipMode_ = ClipMode::Copy;
    SetStatusText("Marked for copy: " + wxString(clipboardPath_.wstring()));
}
/* Cut a file, marking a file in a virtual clipboard to be moved with a later paste operation.  The status bar should display a message indicating the given file was marked for cutting.
* @param event
* @return void
*/
void MainFrame::OnCut(wxCommandEvent& event) {
    std::filesystem::path selectedPath;
    if (!TryGetSelectedPath(selectedPath)) {
        wxMessageBox("No file or directory selected to cut.",
                     "Error",
                     wxOK | wxICON_ERROR,
                     this);
        return;
    }
    clipboardPath_ = selectedPath;
    clipMode_ = ClipMode::Cut;
    SetStatusText("Cut: " + wxString(clipboardPath_.wstring()));
}
/* Paste a file, completing a copy or cut operation by copying or moving the marked file into the current directory depending on whether it was copied or cut in the first place.
* @param event
* @return void
*/
void MainFrame::OnPaste(wxCommandEvent& event) {
    if (clipMode_ == ClipMode::None || clipboardPath_.empty()) {
        wxMessageBox("Clipboard is empty.", "Error", wxOK | wxICON_ERROR, this);
        return;
    }

    std::filesystem::path destPath = currentPath_ / clipboardPath_.filename();

    bool overwrite = false;
    std::error_code ec;
    if (FileOp::Exists(destPath, ec) && !ec) {
        if (!ConfirmOverwriteIfExists(destPath)) return;
        overwrite = true;
    }
    bool success = false;
    if (clipMode_ == ClipMode::Copy) {
        success = FileOp::CopyPath(clipboardPath_, destPath, overwrite, ec);
    } else if (clipMode_ == ClipMode::Cut) {
        success = FileOp::MovePath(clipboardPath_, destPath, overwrite, ec);
    }

    if (!success) {
        wxMessageBox("Failed to paste:\n" + wxString(ec.message()),
                     "Error", wxOK | wxICON_ERROR, this);
        return;
    }

    // clipboard is empty after paste
    clipboardPath_.clear();
    clipMode_ = ClipMode::None;

    SetStatusText("Paste complete. Clipboard is now empty.");
    RefreshFileList(currentPath_);
}
