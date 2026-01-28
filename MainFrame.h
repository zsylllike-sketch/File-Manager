/*
    Author: Shuyun Zheng
    Date: Jan 26, 2026
    Description: Responsible for the user interface of the file manager
*/
#ifndef MAINFRAME_H
#define MAINFRAME_H
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <filesystem>
#include <vector>

/* The primary app window. Responsible for:
                 - Rendering the current directory path and its entries
                 - Tracking selection-to-path mapping for list rows
                 - Handling clipboard state for copy/cut/paste
                 - Responding to UI events
*/
class MainFrame : public wxFrame {
    public:
    /* Construct main frame and intialization
    * @param title: window title displayed
    */
        MainFrame(const wxString& title);
    private:
        enum {
            ID_Refresh = wxID_HIGHEST + 1,
            ID_NewDir,
            ID_Open,
            ID_Rename,
            ID_Delete,
            ID_Copy,
            ID_Cut,
            ID_Paste,
            ID_About
        };
        enum class ClipMode { None, Copy, Cut };
 
        // UI
        wxTextCtrl* m_pathBar;   // path input bar
        wxListCtrl* m_fileList; // file list

        // State
        std::filesystem::path currentPath_; // Curr working dir shown in UI
        std::vector<std::filesystem::path> rowPaths_; // Store paths for each row
        std::filesystem::path clipboardPath_; // Operation path
        ClipMode clipMode_ = ClipMode::None;

        /* List control col and update the path bar
        */
        void SetupListColumns();
        void UpdatePathUI(); 
        
        /* Resolve the selected row using rowPath
        * @param outPath: output with selected path
        * @return: true if a valid row is selected
        */
        bool TryGetSelectedPath(std::filesystem::path& outPath) const;

        /* If the dest path already exists, ask user to confirm 
        * @param dest: destination path
        * @return: True if overwrite is allowed or dest DNE
        */
        bool ConfirmOverwriteIfExists(const std::filesystem::path& dest);

        // Handling user events
        void OnExit(wxCommandEvent& event);
        void OnRefresh(wxCommandEvent& event);
        void OnPathEnter(wxCommandEvent& event); // Handle path input
        void OnFileActivated(wxListEvent& event); // Handle file/directory activation

        void OnNewDir(wxCommandEvent& event);
        void OnOpen(wxCommandEvent& event);
        void OnRename(wxCommandEvent& event);
        void OnDelete(wxCommandEvent& event);
        void OnCopy(wxCommandEvent& event);
        void OnCut(wxCommandEvent& event);
        void OnPaste(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        /* Read dir entries from provided path and update
        * @param path: dir path to display
        */
        void RefreshFileList(const std::filesystem::path& path);
};



#endif 