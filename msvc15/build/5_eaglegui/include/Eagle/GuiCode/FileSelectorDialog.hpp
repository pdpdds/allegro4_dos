/*
 *
 *     _______       ___       ____      __       _______ 
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\ 
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_ 
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\ 
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_ 
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    EAGLE
 *    Edgar's Allegro Gui Library and Extensions
 *
 *    Copyright 2009-2011 by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#ifndef FileSelectorDialog_H
#define FileSelectorDialog_H


#include <iostream>
#include <string>
#include <list>
#include <cstring>
#include <cstdlib>

#include "Eagle/GuiCode/WidgetBase.hpp"
#include "Eagle/GuiCode/ScrollBarWidget.hpp"
#include "Eagle/GuiCode/ButtonWidget.hpp"
#include "Eagle/GuiCode/ListBox.hpp"
#include "Eagle/GuiCode/TextInput.hpp"
#include "Eagle/StringWork.hpp"



void clear_file_list();

//int (*callback)(const char *filename, int attrib, void *param)
int build_file_list(const char* filename , int attrib , void* param);

void BuildFileList(const char* path , int in_attrib , int out_attrib , std::string extension_filter);
void PrintFileList(std::ostream& os);
void SortFileList(std::string extension_filter);




extern const unsigned int TOPIC_FOLDER_ITEM;

enum FOLDER_ITEM_MSGS {
   FOLDER_ITEM_SELECTED = 0,
   FOLDER_VIEW_TOGGLED = 1
};

extern const unsigned int TOPIC_FOLDER_TREE;

enum FOLDER_TREE_MSGS {
   FOLDER_SELECTED = 0
};

extern const unsigned int TOPIC_FILE_SELECTOR;

enum FILE_SELECTOR_MSGS {
   FILE_OKAYED = 0,
   FILE_CANCELLED = 1
};



class File {
public :
   std::string file_path;
   int attributes;
   
   File(std::string path , int attrib);
   
   
};

std::list<File>& GetFileList();

class Folder {
private :
   void CopyFoldersFromFileList();
   
   std::string GetShortName(std::string path);
   
public :
   std::string full_path;
   std::string short_name;
   int level;
   Folder* parent;
   
   std::list<Folder> sub_folders;
   
   bool open,read;
   
   Folder(std::string path , std::string name , int folder_level , Folder* parent_folder);
   
   void ReadFolder();
   void ReadAll();
   void PrintFolder(std::ostream& os , Indenter indent);
   void PrintSubFolders(std::ostream& os);
   void Open(bool open_folder);
   void ToggleOpen();
   
   Folder* FindSubFolderByShortName(std::string name);
   Folder* FindSubFolderByFullPath(std::string path);

   friend std::ostream& operator<<(std::ostream& os , const Folder& f) {
      os << "Folder : Full path (" << f.full_path << ") , Short name (" << f.short_name << ") , Level (";
      os << f.level << ") , parent (" << f.parent << ") , ";
      os << (f.open?"open":"not open") << " , " << (f.read?"read":"not read");
      os << " , " << f.sub_folders.size() << " sub folders." << std::endl;
      return os;
   }

};


class FileSystem {
public :
   std::list<Folder> drives;
   
   bool read;
   
   void ReadDrives();
   void ReadAllFolders();
   void PrintDriveList(std::ostream& os);
   void PrintFileSystem(std::ostream& os);
   
   
//   Folder* FindFolder(std::string path);// absolute path only
   Folder* SmartFindFolder(std::string path);// absolute path only

   void OpenFolder(std::string path);
   
   friend std::ostream& operator<<(std::ostream& os , const FileSystem& fs);
};


class FolderIconButton : public Button {
private :
   FONT* text_font;
   
public :
   
   FolderIconButton(std::string name , FONT* textfont , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
   
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   void SetFont(FONT* textfont);
};



class FolderItem : public WidgetBase {
private :
   FolderIconButton fbtn;
   Folder* folder;
   FONT* text_font;
   Rectangle text_area;
   
   bool selected;
   
   void ResetPositions();
   
public :
   
   FolderItem(std::string name , FONT* textfont , Folder* target_folder , int x , int y);
   
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual void QueueUserMessage(const WidgetMsg& wmsg);

   virtual void SetWidgetColors(const WidgetColorset& colors);
   virtual void UseWidgetColorset(WidgetColorset* colorset);
   virtual void UseIndividualColorset();
   
   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);


   /// Setters
   void Select(bool is_selected);
   void SetFont(FONT* textfont);

   /// Getters
   Folder* FolderPtr() {return folder;}
   Rectangle TextArea() {return text_area;}
};



class FolderTree : public WidgetBase {
private :
   FileSystem fs;
   std::list<FolderItem*> flist;
   std::string fselect_path;
   
   ScrollBar hscroller;
   ScrollBar vscroller;
   int hscroll;
   int vscroll;
   Rectangle tree_area;
   
   FONT* text_font;
   int fontd,padding;
   
   bool list_rebuilt;
   

   void ClearFolderList();
   void BuildFolderList();
   int AddFolderToList(Folder* f , int x , int y);
   void ReselectFolder();

   void ResetPositions();
   void ResetScrollBars();
  
public :
   
   /// FolderTree must be at least 40+16+2 = 58 in width and height.
   FolderTree(std::string name , FONT* textfont , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
   ~FolderTree();

   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);// may send DIALOG_CLOSE
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual void QueueUserMessage(const WidgetMsg& wmsg);

   virtual void SetWidgetColors(const WidgetColorset& colors);
   virtual void UseWidgetColorset(WidgetColorset* colorset);
   virtual void UseIndividualColorset();

   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

   virtual int AbsMinWidth();
   virtual int AbsMinHeight();
   
   bool SelectFolder(std::string path);
   void SelectDefaultFolder();
   void SetFont(FONT* textfont);
   
   void PrintFileSystem(std::ostream& os);

   /// Getters
   std::string GetSelectedFolder();
   bool ListRebuilt() {return list_rebuilt;}
   
};



// Recommended minimum width and height of 200x125
class FileSelector : public WidgetBase {
private :
   FolderTree ftree;
   ListBox file_list;
   TextInput file_name;
   
   Button okay_btn;
   Button cancel_btn;
   
   FONT* textfont;
   
   bool open_dialog;// true means this is an file opening dialog , false means a file saving dialog
   
   std::string extensions;// semi-colon separated list of file extensions - ex... "bmp;BMP;jpg". To filter none , pass "*".
   std::string selected_files;
   std::list<std::string> files;
   
   void ResetPositions();

   void BuildSelections();


public :
   // minimum recommended dimensions of 200x125, larger for fonts larger than 8x8
   FileSelector(std::string name , FONT* text_font , bool is_open_dialog , bool allow_multiple_selections , 
                std::string okay_button_label , std::string default_folder , std::string default_file_name , std::string file_extensions ,
                const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
   
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual void QueueUserMessage(const WidgetMsg& wmsg);

   virtual void SetWidgetColors(const WidgetColorset& colors);
   virtual void UseWidgetColorset(WidgetColorset* colorset);
   virtual void UseIndividualColorset();
   
   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

   virtual void SetFocusState(bool state);

   virtual int AbsMinWidth();
   virtual int AbsMinHeight();

   /// Getters
   std::string GetSelectedFiles() {return selected_files;}// full path file names separated by commas
   std::list<std::string> GetSelectedFileList() {return files;}// list of full path file names
   
   void PrintFileSystem(std::ostream& os);

};



#endif // FileSelectorDialog_H




