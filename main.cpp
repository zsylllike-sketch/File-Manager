/*
    Author: Shuyun Zheng
    Date: Jan 26, 2026
    Description: Entry point of the File Manager application. Defines the
                 wxWidgets application class and initializes the main
                 application window.
*/
#include <wx/wx.h>
#include "MainFrame.h"

class FileManagerApp : public wxApp {
public:
    /* App initialization and start wxWidget
    * @return: true if initialization succeeds
    */
    bool OnInit() override {
        MainFrame* frame = new MainFrame("File Manager");
        frame->Show(true);
        return true;
    }
};
wxIMPLEMENT_APP(FileManagerApp);