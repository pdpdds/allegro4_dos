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


#include "Eagle/GuiCode/FileSelectorDialog.hpp"

using std::string;
using std::list;
using std::ostream;
using std::endl;

#include <allegro.h>
#include <allegro/internal/aintern.h>



const unsigned int TOPIC_FOLDER_ITEM = NextFreeTopicId();
const unsigned int TOPIC_FOLDER_TREE = NextFreeTopicId();
const unsigned int TOPIC_FILE_SELECTOR = NextFreeTopicId();



list<File> files;

typedef list<string>::iterator LSIT;
typedef list<Folder>::iterator LFIT;



void clear_file_list() {
   files.clear();
}



int build_file_list(const char* filename , int attrib , void* param) {
   files.push_back(File(filename,attrib));
   return 0;
}



void BuildFileList(const char* path , int in_attrib , int out_attrib , string extension_filter) {
   clear_file_list();
   for_each_file_ex(path , in_attrib , out_attrib , build_file_list , NULL);
   SortFileList(extension_filter);
}



void SortFileList(string extension_filter) {
   list<string> exts = TokenizeString(extension_filter , ';');
   list<File> new_folders;
   list<File> new_files;
   for (list<File>::iterator it = files.begin() ; it != files.end() ; ++it) {
      File& f = *it;
      string s = GetShortFileName(f.file_path);
      if (f.attributes & FA_DIREC) {
         if ((s != ".") && (s != "..")) {
            new_folders.push_back(f);
         }
      }
   }
   for (list<File>::iterator it = files.begin() ; it != files.end() ; ++it) {
      File& f = *it;
      if (!(f.attributes & FA_DIREC)) {
         bool allow = false;
         if (extension_filter == "*") {
            allow = true;
         } else {
            string& s = f.file_path;
            string ext = GetFileExtension(s);
            for (list<string>::iterator sit = exts.begin() ; sit != exts.end() ; ++sit) {
               if (ext == *sit) {allow = true;}
            }
         }
         if (allow) {new_files.push_back(f);}
      }
   }

   /// Sort file list by file extensions
   for (list<File>::iterator it = new_files.begin() ; it != new_files.end() ; ++it) {
      File& f = *it;
      string& s = f.file_path;
      string ext = GetFileExtension(s);
      list<File>::iterator least = it;
      for (list<File>::iterator it2 = it ; it2 != new_files.end() ; ++it2) {
         File& f2 = *it2;
         string& s2 = f2.file_path;
         string ext2 = GetFileExtension(s2);
         if (ext2 < ext) {
            ext = ext2;
            least = it2;
         }
      }
      if (least != it) {
         File temp = *it;
         *it = *least;
         *least = temp;
      }
   }

   files = new_folders;
   for (list<File>::iterator it = new_files.begin() ; it != new_files.end() ; ++it) {
      files.push_back(*it);
   }
}



list<File>& GetFileList() {return files;}



void PrintFileList(ostream& os) {
   os << "File list has " << files.size() << " entries." << endl;
   int i = 0;
   for (list<File>::iterator it = files.begin() ; it != files.end() ; ++it) {
      os << "#" << i << " '" << it->file_path << "'" << endl;
      ++i;
   }
   os << endl;
}



/// ----------------------- File class ---------------------------


File::File(string path , int attrib) :
      file_path(path),
      attributes(attrib)
{}
/*
      FA_NONE           - Exclude files that have any attribute set
      FA_RDONLY         - Directory entries that are unwritable for current user
      FA_HIDDEN         - Hidden flag
      FA_DIREC          - Directories
      FA_SYSTEM         - Files with system flag set (DOS/Windows only)
      FA_LABEL          - Files with volume label flag set (DOS/Windows only)
      FA_ARCH           - Files with archive flag set (DOS/Windows only)
      FA_ALL            - Match all attributes
*/



/// --------------------------------------- Folder class --------------------------------




void Folder::CopyFoldersFromFileList() {
   for (list<File>::iterator it = files.begin() ; it != files.end() ; ++it) {
      string path = it->file_path;
      string name = GetShortName(path);
      string a = ".";
      string b = "..";
      if ((name != a) && (name != b)) {
         sub_folders.push_back(Folder(path , name , level + 1 , this));
      }
   }
}



string Folder::GetShortName(string path) {
   unsigned int i = path.find_last_of(OTHER_PATH_SEPARATOR);
   if (i == string::npos) {
      return path;
   } else if ((i + 1) >= path.size()) {
      return path;
   } else {
      string name = path.substr(i + 1);
      return name;
   }
   return string("");
}



Folder::Folder(string path , string name , int folder_level , Folder* parent_folder) :
      full_path(path),
      short_name(name),
      level(folder_level),
      parent(parent_folder),
      sub_folders(),
      open(false),read(false)
{}



void Folder::ReadFolder() {
   sub_folders.clear();
//   OutputLog() << "Folder::ReadFolder - full_path is " << full_path << endl;
   string search_path = full_path;
   if (search_path.size()) {
      unsigned int index = search_path.size() - 1;
      char c = search_path.at(index);
      if (c != OTHER_PATH_SEPARATOR) {
         search_path.push_back(OTHER_PATH_SEPARATOR);
      }
      search_path.push_back('*');

//      OutputLog() << "Folder::ReadFolder - Search path is '" << search_path << "'" << endl;

      BuildFileList(search_path.c_str() , FA_DIREC , FA_NONE , "*");
      CopyFoldersFromFileList();
   }
   read = true;
}



void Folder::ReadAll() {
   if (!read) {ReadFolder();}
   for (list<Folder>::iterator it = sub_folders.begin() ; it != sub_folders.end() ; ++it) {
      Folder& f = *it;
      f.ReadAll();
   }
}



void Folder::PrintFolder(std::ostream& os , Indenter indent) {
   os << indent << short_name << endl;
   if (sub_folders.size()) {
      ++indent;
      for (list<Folder>::iterator it = sub_folders.begin() ; it != sub_folders.end() ; ++it) {
         Folder& f = *it;
         f.PrintFolder(os , indent);
      }
   }
}



void Folder::PrintSubFolders(std::ostream& os) {
   for (LFIT it = sub_folders.begin() ; it != sub_folders.end() ; ++it) {
      Folder& f = *it;
      os << f.full_path << endl;
   }
   os << endl;
}



void Folder::Open(bool open_folder) {
   open = open_folder;
   if (open && !read) {
      ReadFolder();
   }
}



void Folder::ToggleOpen() {
   open = !open;
   if (open && !read) {
      ReadFolder();
   }
}



Folder* Folder::FindSubFolderByShortName(string name) {
   if (!read) {ReadFolder();}
   for (LFIT it = sub_folders.begin() ; it != sub_folders.end() ; ++it) {
      Folder& f = *it;
      if (name == f.short_name) {
         return &f;
      }
   }
   return (Folder*)0;
}



Folder* Folder::FindSubFolderByFullPath(string path) {
   if (!read) {ReadFolder();}
   for (LFIT it = sub_folders.begin() ; it != sub_folders.end() ; ++it) {
      Folder& f = *it;
      if (path == f.full_path) {
         return &f;
      }
   }
   for (LFIT it = sub_folders.begin() ; it != sub_folders.end() ; ++it) {
      Folder& f = *it;
      Folder* found = f.FindSubFolderByFullPath(path);
      if (found) {return found;}
   }
   return (Folder*)0;
}



/// ------------------------------- FileSystem class -------------------------------------------



void FileSystem::ReadDrives() {
   drives.clear();
/// _al_drive_exists only exists on Windows and DOS
#if defined(ALLEGRO_WINDOWS) || defined(ALLEGRO_DOS)
   char drive[32];
   char dname[32];
   char d = 'A';
   for (int i = 0 ; i < 26 ; ++i) {
      if (_al_drive_exists(i)) {
         int pos = 0;
         pos += usetc(drive , d);
         pos += usetc(drive + pos , DEVICE_SEPARATOR);
         pos += usetc(drive + pos , OTHER_PATH_SEPARATOR);
         pos += usetc(drive + pos , '\0');
         pos = 0;
         pos += usetc(dname , d);
         pos += usetc(dname + pos , DEVICE_SEPARATOR);
         pos += usetc(dname + pos , '\0');
         drives.push_back(Folder(drive , dname , 0 , (Folder*)0));
      }
      ++d;
   }
   read = true;
#else
   drives.push_back(Folder("/" , "/" , 0 , (Folder*)0));
#endif
}



void FileSystem::ReadAllFolders() {
   if (!read) {ReadDrives();}
   for (list<Folder>::iterator it = drives.begin() ; it != drives.end() ; ++it) {
      Folder& f = *it;
      f.ReadAll();
   }
}



void FileSystem::PrintDriveList(std::ostream& os) {
   os << "Drive list has " << drives.size() << " entries." << endl;
   for (LFIT it = drives.begin() ; it != drives.end() ; ++it) {
      Folder& f = *it;
      os << f.full_path << endl;
   }
   os << endl;
}



void FileSystem::PrintFileSystem(std::ostream& os) {
   Indenter indent(0,3);
   for (list<Folder>::iterator it = drives.begin() ; it != drives.end() ; ++it) {
      Folder& f = *it;
      f.PrintFolder(os , indent);
   }
}


/*
Folder* FileSystem::FindFolder(string path) {
   if (path.size() == 0) {return 0;}

   if (!read) {ReadDrives();}
   // remove trailing slash from path if there is one
   unsigned int ts = path.find_last_of(OTHER_PATH_SEPARATOR);
   if (ts == (path.size() - 1)) {
      path = path.substr(0,ts);
   }


   // compare the drive in path to the drives
   unsigned int index = path.find_first_of(OTHER_PATH_SEPARATOR);
   string drive_path;
   if (index == string::npos) {
      drive_path = path;
   } else {
      drive_path = path.substr(0 , index);
   }
   Folder* target_drive = 0;
   for (LFIT it = drives.begin() ; it != drives.end() ; ++it) {
      Folder& drive = *it;
      if (drive.short_name == path) {
         return &drive;
      }
      if (drive.short_name == drive_path) {
         target_drive = &drive;
      }
   }
//   OutputLog() << "FileSystem::FindFolder : Target drive is " << (target_drive?target_drive->full_path:"null") << endl;
   if (target_drive) {
      return target_drive->FindSubFolderByFullPath(path);
   }
   return (Folder*)0;
}
*/


Folder* FileSystem::SmartFindFolder(string path) {
   if (!read) {ReadDrives();}
   path = FixFilenameSlashes(path);
   path = RemoveTrailingSlash(path);
   list<string> paths = TokenizeString(path , OTHER_PATH_SEPARATOR);
   if (paths.size() == 0) {return (Folder*)0;}
   list<string>::iterator it = paths.begin();
   Folder* f = 0;
   for (list<Folder>::iterator fit = drives.begin() ; fit != drives.end() ; ++fit) {
      Folder& fl = *fit;
      if (fl.short_name == *it) {
         f = &fl;
         if (paths.size() == 1) {return f;}
         break;
      }
   }
   if (!f) {// drive not found
      return (Folder*)0;
   }
   ++it;
   for ( ; it != paths.end() ; ++it) {
      Folder* sf = f->FindSubFolderByShortName(*it);
      if (!sf) {// sub folder not found
         return (Folder*)0;
      } else {
         f = sf;
         list<string>::iterator next = it;
         ++next;
         if (next == paths.end()) {
            return f;
         }
      }
   }
   return (Folder*)0;
}



void FileSystem::OpenFolder(string path) {
   Folder* f = SmartFindFolder(path);
   while(f) {
      f->Open(true);
      f = f->parent;
   }
}



ostream& operator<<(ostream& os , const FileSystem& fs) {
   os << "Printing file system with operator << , drive " << (fs.read?"has":"has not") << " been read." << endl;
   for (list<Folder>::const_iterator it = fs.drives.begin() ; it != fs.drives.end() ; ++it) {
      const Folder& f = *it;
      os << f;
   }
   os << endl;
   return os;
}




/// ------------------------------------- FolderIconButton class ---------------------------



FolderIconButton::FolderIconButton(string name , FONT* textfont , const Rectangle& position , UINT wflags) :
      Button(name , RECTANGLE_BTN , TOGGLE_BTN , textfont , "" , input_key_press(KEY_NONE) , position , wflags),
      text_font(0)
{
   SetFont(textfont);
}



void FolderIconButton::DisplayOn(BITMAP* bmp , int x , int y) {
   Rectangle r = area;
   r.MoveBy(x,y);
   r.Fill(bmp , (*wcols)[BGCOL]);
   r.Draw(bmp , (*wcols)[FGCOL]);
   const char* ch = (up?"+":"-");
   int tx = r.CX();
   int ty = r.CY() - text_height(text_font)/2;
   textout_centre_ex(bmp , text_font , ch , tx , ty , (*wcols)[TXTCOL] , -1);
}



void FolderIconButton::SetFont(FONT* textfont) {
   ASSERT(textfont);
   text_font = textfont;
   SetRedrawFlag();
}



/// ------------------------------------- FolderItem class ------------------------------------------


void FolderItem::ResetPositions() {
   ASSERT(folder);
   ASSERT(text_font);
   int padding = text_height(text_font)/4;
   int fontd = text_height(text_font) + 2*padding;
   int indent = fontd*folder->level;
   fbtn.SetArea(area.X() + indent , area.Y() , fontd , fontd);
   int textl = text_length(text_font , folder->short_name.c_str()) + 2*padding;
   text_area.SetArea(area.X() + indent + fontd , area.Y() , textl , fontd);
   WidgetBase::SetArea(area.X() , area.Y() , indent + fontd + textl , fontd);
}



FolderItem::FolderItem(string name , FONT* textfont , Folder* target_folder , int x , int y) :
      WidgetBase(name , NULL , Rectangle(x,y,0,0)),
      fbtn((name + string("-fbtn")) , textfont , Rectangle(0,0,0,0)),
      folder(target_folder),
      text_font(textfont),
      text_area(),
      selected(false)
{
   ASSERT(textfont);
   ASSERT(target_folder);
   ResetPositions();
   fbtn.SetParent(this);
   fbtn.UseWidgetColorset(wcols);
   fbtn.SetButtonUpState(!target_folder->open);
}



WidgetMsg FolderItem::Update (double tsec) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg FolderItem::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   FolderTree* ftree = dynamic_cast<FolderTree*>(parent);
   const WidgetMsg wmsg1 = fbtn.CheckInputs(msx,msy);
   retmsg |= wmsg1.Messages();
   if (input_mouse_press(LMB) && text_area.Contains(msx,msy)) {
      WidgetBase::QueueUserMessage(this , TOPIC_FOLDER_ITEM , FOLDER_ITEM_SELECTED);
      if (ftree) {
         ftree->SetBgRedrawFlag();
      } else {
         SetBgRedrawFlag();
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void FolderItem::DisplayOn(BITMAP* bmp , int x , int y) {
   fbtn.DisplayOn(bmp , x , y);
   int padding = text_height(text_font)/4;
   if (selected) {
      Rectangle r = text_area;
      r.MoveBy(x,y);
      r.Fill(bmp , (*wcols)[MGCOL]);
   }
   int tx = x + text_area.X() + padding;
   int ty = y + text_area.CY() - text_height(text_font)/2;
   textout_ex(bmp , text_font , folder->short_name.c_str() , tx , ty , (*wcols)[TXTCOL] , -1);
}



void FolderItem::QueueUserMessage(const WidgetMsg& wmsg) {
   const WidgetMsg btnmsg(&fbtn , TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED);
   if (wmsg == btnmsg) {
      WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_FOLDER_ITEM , FOLDER_VIEW_TOGGLED));
   } else {
      WidgetBase::QueueUserMessage(wmsg);
   }
}



void FolderItem::SetWidgetColors(const WidgetColorset& colors) {
   WidgetBase::SetWidgetColors(colors);
   fbtn.UseWidgetColorset(&wc);
}



void FolderItem::UseWidgetColorset(WidgetColorset* colorset) {
   WidgetBase::UseWidgetColorset(colorset);
   fbtn.UseWidgetColorset(wcols);
}



void FolderItem::UseIndividualColorset() {
   WidgetBase::UseIndividualColorset();
   fbtn.UseWidgetColorset(&wc);
}



void FolderItem::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
   ResetPositions();
}



void FolderItem::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   ResetPositions();
}



void FolderItem::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   ResetPositions();
}



void FolderItem::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   ResetPositions();
}




void FolderItem::Select(bool is_selected) {
   selected = is_selected;
   SetBgRedrawFlag();
}



void FolderItem::SetFont(FONT* textfont) {
   ASSERT(textfont);
   text_font = textfont;
   fbtn.SetFont(textfont);
   ResetPositions();
}



/// ------------------------------------- FolderTree class ---------------------------------------------



void FolderTree::ClearFolderList() {
   for (list<FolderItem*>::iterator it = flist.begin() ; it != flist.end() ; ++it) {
      delete *it;
   }
   flist.clear();
   list_rebuilt = true;
}



void FolderTree::BuildFolderList() {
   ClearFolderList();

   int x = area.X() + 1;
   int y = area.Y() + 1;
   for (list<Folder>::iterator it = fs.drives.begin() ; it != fs.drives.end() ; ++it) {
      Folder* f = &(*it);
      y = AddFolderToList(f , x , y);
   }
   ResetScrollBars();
}



int FolderTree::AddFolderToList(Folder* f , int x , int y) {
   if (f) {
      FolderItem* fitem = new FolderItem("fitem" , text_font , f , x , y);
      fitem->SetParent(this);
      fitem->UseWidgetColorset(wcols);
      flist.push_back(fitem);
      y += fontd;
      if (f->open) {
         list<Folder>& sf = f->sub_folders;
         for (list<Folder>::iterator it = sf.begin() ; it != sf.end() ; ++it) {
            y = AddFolderToList(&(*it) , x , y);
         }
      }
      list_rebuilt = true;
   }
   return y;
}



void FolderTree::ReselectFolder() {
   SelectFolder(fselect_path);
}



void FolderTree::ResetPositions() {
   // 1px outline , 40px scrollbar length , 16px scrollbar width
   const int scrw = 16;
   ASSERT(area.W() >= (40 + scrw + 2*1));
   ASSERT(area.H() >= (40 + scrw + 2*1));
   int fx = area.X() + 1;
   int fy = area.Y() + 1;
   tree_area.SetArea(fx,fy,area.W() - (scrw + 2),area.H() - (scrw + 2));
   for (list<FolderItem*>::iterator it = flist.begin() ; it != flist.end() ; ++it) {
      FolderItem* fitem = *it;
      fitem->SetDrawPos(fx , fy);
      fy += fontd;
   }
   hscroller.SetArea(area.X() + 1 , tree_area.BRY() + 1 , area.W() - (scrw + 2) , scrw);
   vscroller.SetArea(tree_area.BRX() + 1 , area.Y() + 1 , scrw , area.H() - (scrw + 2));
}



void FolderTree::ResetScrollBars() {
   int maxlength = 0;
   int maxlines = tree_area.H() / fontd;
   int numlines = flist.size();
   for (list<FolderItem*>::iterator it = flist.begin() ; it != flist.end() ; ++it) {
      FolderItem* fitem = *it;
      int length = fitem->Area().W();
      if (length > maxlength) {maxlength = length;}
   }
//   void SetParameters(int maximum_scroll , int set_scroll , int set_scroll_rate , int set_view_length);
   if (maxlength > tree_area.W()) {
      hscroller.ShowWidget();
      hscroller.SetParameters(maxlength , hscroll , text_length(text_font , " ") , tree_area.W());
   } else {
      hscroller.HideWidget();
      hscroll = 0;
   }
   if (numlines > maxlines) {
      vscroller.ShowWidget();
//      vscroller.SetParameters(maxlines , vscroll , 1 , numlines);
      vscroller.SetParameters(numlines , vscroll , 1 , maxlines);
   } else {
      vscroller.HideWidget();
      vscroll = 0;
   }
}



FolderTree::FolderTree(string name , FONT* textfont , const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      fs(),
      flist(),
      fselect_path(""),
      hscroller(name + string("-hscroller") , 100 , 0 , 1 , 10 , false,Rectangle(0,0,75,75)),
      vscroller(name + string("-vscroller") , 100 , 0 , 1 , 10 , true,Rectangle(0,0,75,75)),
      hscroll(0),
      vscroll(0),
      tree_area(),
      text_font(textfont),
      fontd(0),padding(0),
      list_rebuilt(false)
{
   ASSERT(textfont);

   SetMinDimensions(AbsMinWidth() , AbsMinHeight());

   SetFont(textfont);
   fs.ReadDrives();
   BuildFolderList();
   SelectDefaultFolder();

   ResetPositions();// called by SetFont
   ResetScrollBars();// called by SetFont

   hscroller.SetParent(this);
   vscroller.SetParent(this);
   hscroller.UseWidgetColorset(wcols);
   vscroller.UseWidgetColorset(wcols);
}



FolderTree::~FolderTree() {
   ClearFolderList();
}



WidgetMsg FolderTree::Update (double tsec) {
   UINT retmsg = DIALOG_OKAY;
   WidgetMsg hsmsg = hscroller.Update(tsec);
   WidgetMsg vsmsg = vscroller.Update(tsec);
   retmsg |= hsmsg.Messages();
   retmsg |= vsmsg.Messages();
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



WidgetMsg FolderTree::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   if (tree_area.Contains(msx,msy)) {
      list_rebuilt = false;
      for (list<FolderItem*>::iterator it = flist.begin() ; it != flist.end() ; ++it) {
         if (list_rebuilt) {break;}
         FolderItem* fitem = *it;
         WidgetMsg wmsg = fitem->CheckInputs(msx + hscroll,msy + vscroll*fontd);
         retmsg |= wmsg.Messages();
      }
      if (ms_wdiff) {
         int length = text_length(text_font , " ");
         hscroller.ScrollBy(length*ms_wdiff);
         SetRedrawFlag();
         retmsg |= DIALOG_INPUT_USED | DIALOG_REDRAW_ME;
      }

      if (ms_zdiff) {
         vscroller.ScrollBy(-ms_zdiff);
         SetRedrawFlag();
         retmsg |= DIALOG_INPUT_USED | DIALOG_REDRAW_ME;
      }
   }
   WidgetMsg hsmsg = hscroller.CheckInputs(msx,msy);
   WidgetMsg vsmsg = vscroller.CheckInputs(msx,msy);
   retmsg |= hsmsg.Messages();
   retmsg |= vsmsg.Messages();
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void FolderTree::DisplayOn(BITMAP* bmp , int x , int y) {
   Rectangle r = area;
   r.MoveBy(x,y);
   r.Fill(bmp , (*wcols)[BGCOL]);
   r.Draw(bmp , (*wcols)[FGCOL]);
   Rectangle old_clip = GetClippingRectangle(bmp);
   Rectangle clip = tree_area;
   clip.MoveBy(x,y);
   clip.SetClipRect(bmp);
   for (list<FolderItem*>::iterator it = flist.begin() ; it != flist.end() ; ++it) {
      FolderItem* fitem = *it;
      fitem->DisplayOn(bmp,x - hscroll,y - vscroll*fontd);
   }
   old_clip.SetClipRect(bmp);
   hscroller.DisplayOn(bmp , x , y);
   vscroller.DisplayOn(bmp , x , y);
   ClearRedrawFlag();
}



void FolderTree::QueueUserMessage(const WidgetMsg& wmsg) {
   const WidgetMsg hsmsg(&hscroller , TOPIC_SCROLLBAR , SCROLLBAR_VALUE_CHANGED);
   const WidgetMsg vsmsg(&vscroller , TOPIC_SCROLLBAR , SCROLLBAR_VALUE_CHANGED);
   if (wmsg.IsMessageTopic(TOPIC_FOLDER_ITEM , FOLDER_ITEM_SELECTED)) {
      FolderItem* fitem = dynamic_cast<FolderItem*>(wmsg.From());
//      OutputLog() << "Folder Tree QUM : " << fitem << " , TOPIC_FOLDER_ITEM , FOLDER_SELECTED" << endl;
      if (fitem) {
         Folder* f = fitem->FolderPtr();
         SelectFolder(f->full_path);
      }
   } else if (wmsg.IsMessageTopic(TOPIC_FOLDER_ITEM , FOLDER_VIEW_TOGGLED)) {
      FolderItem* fitem = dynamic_cast<FolderItem*>(wmsg.From());
//      OutputLog() << "Folder Tree QUM : " << fitem << " , TOPIC_FOLDER_ITEM , FOLDER_VIEW_TOGGLED" << endl;
      if (fitem) {
         Folder* f = fitem->FolderPtr();
         f->ToggleOpen();
         SelectFolder(f->full_path);
//         ReselectFolder();
      }
   } else if (wmsg == hsmsg) {
      hscroll = hscroller.GetScroll();
      SetRedrawFlag();
   } else if (wmsg == vsmsg) {
      vscroll = vscroller.GetScroll();
      SetRedrawFlag();
   } else {
      WidgetBase::QueueUserMessage(wmsg);
   }
}



void FolderTree::SetWidgetColors(const WidgetColorset& colors) {
   WidgetBase::SetWidgetColors(colors);
   for (list<FolderItem*>::iterator it = flist.begin() ; it != flist.end() ; ++it) {
      FolderItem* fitem = *it;
      fitem->UseWidgetColorset(wcols);
   }
   hscroller.UseWidgetColorset(wcols);
   vscroller.UseWidgetColorset(wcols);
}



void FolderTree::UseWidgetColorset(WidgetColorset* colorset) {
   WidgetBase::UseWidgetColorset(colorset);
   for (list<FolderItem*>::iterator it = flist.begin() ; it != flist.end() ; ++it) {
      FolderItem* fitem = *it;
      fitem->UseWidgetColorset(wcols);
   }
   hscroller.UseWidgetColorset(wcols);
   vscroller.UseWidgetColorset(wcols);
}



void FolderTree::UseIndividualColorset() {
   WidgetBase::UseIndividualColorset();
   for (list<FolderItem*>::iterator it = flist.begin() ; it != flist.end() ; ++it) {
      FolderItem* fitem = *it;
      fitem->UseWidgetColorset(wcols);
   }
   hscroller.UseWidgetColorset(wcols);
   vscroller.UseWidgetColorset(wcols);
}



void FolderTree::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
   ResetPositions();
}



void FolderTree::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   ResetPositions();
}



void FolderTree::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   ResetPositions();
}



void FolderTree::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   ResetPositions();
}



int FolderTree::AbsMinWidth() {
   return (2 + 16 + hscroller.AbsMinWidth());
}



int FolderTree::AbsMinHeight() {
   return (2 + 16 + vscroller.AbsMinHeight());
}



bool FolderTree::SelectFolder(string path) {
   Folder* f = fs.SmartFindFolder(path);
//   OutputLog() << "Folder Tree::Select Folder - " << (f?"Found":"Did not find") << " folder '" << path << "'" << endl;
   if (f) {
      fselect_path = path;
      if (f->parent) {
         fs.OpenFolder(f->parent->full_path);
      }
      BuildFolderList();
      int vscroll_min = vscroll;
      int view = tree_area.H() / fontd;
      int vscroll_max = vscroll_min + view - 1;
      int hscroll_min = hscroll;
      int hview = tree_area.W();
      int hscroll_max = hscroll_min + hview -1;
      int i = 0;
      for (list<FolderItem*>::iterator it = flist.begin() ; it != flist.end() ; ++it) {
         FolderItem* fitem = *it;
         Folder* fl = fitem->FolderPtr();
         if (fl->full_path == path) {
            (*it)->Select(true);
            // Set the vertical scroll position if necessary
            if ((i < vscroll_min) || (i > vscroll_max)) {
               vscroller.SetScroll(i);// align with top of tree area
            }
            // Set the horizontal scroll position if necessary
            Rectangle t = fitem->TextArea();
            int tx = t.X();
            if ((tx < hscroll_min) || (tx > hscroll_max)) {
               hscroller.SetScroll(tx - tree_area.W()/2);// center horizontally
            }
         } else {
            (*it)->Select(false);
         }
         ++i;
      }
      SetRedrawFlag();
      WidgetBase::QueueUserMessage(this , TOPIC_FOLDER_TREE , FOLDER_SELECTED);
      return true;
   }
   return false;
}



void FolderTree::SelectDefaultFolder() {
   if (!fs.read) {
      fs.ReadDrives();
      BuildFolderList();
   }
   FolderItem* fitem = 0;
   for (list<FolderItem*>::iterator it = flist.begin() ; it != flist.end() ; ++it) {
      if (it == flist.begin()) {
         (*it)->Select(true);
         fitem = *it;
         Folder* f = fitem->FolderPtr();
         ASSERT(f);
         string path = f->full_path;
         fselect_path = RemoveTrailingSlash(path);
      } else {
         (*it)->Select(false);
      }
   }
   WidgetBase::QueueUserMessage(fitem , TOPIC_FOLDER_ITEM , FOLDER_SELECTED);
}



void FolderTree::SetFont(FONT* textfont) {
   ASSERT(textfont);
   text_font = textfont;
   padding = text_height(text_font)/4;
   fontd = text_height(text_font) + 2*padding;
   for (list<FolderItem*>::iterator it = flist.begin() ; it != flist.end() ; ++it) {
      FolderItem* fitem = *it;
      fitem->SetFont(textfont);
   }
   ResetPositions();
   ResetScrollBars();
   SetRedrawFlag();
}



void FolderTree::PrintFileSystem(std::ostream& os) {
   fs.PrintFileSystem(os);
}



string FolderTree::GetSelectedFolder() {
   return fselect_path;
}



/// ------------------------------- File Selector class -----------------------------------


/*
void FileSelector::ResetPositions() {
   int x = area.X() , y = area.Y() , w = area.W() , h = area.H();
   int pad = text_height(textfont)/4;
   int bh = text_height(textfont) + 2*(pad + 1);
   // for an 8x8 font , bh = 14
   int fh = h - 4*bh;
   double fw = (double)(w - 3*bh);
   int nw = w - 4*bh - 2*6*bh;
   int ftx = x + bh             , fty = y + bh        , ftw = (int)(0.4*fw) , fth = fh;
   int flx = x + 2*bh + ftw     , fly = y + bh        , flw = (int)(0.6*fw) , flh = fh;
   int fnx = x + bh             , fny = y + 2*bh + fh , fnw = nw            , fnh = bh;
   int obx = x + fnx + fnw + bh , oby = y + fh + 2*bh , obw = 6*bh          , obh = bh;
   int cbx = x + obx + obw + bh , cby = y + oby       , cbw = obw           , cbh = obh;

   // ASSERT(area.W() >= 187) for an 8x8 font
   // ASSERT(area.H() >= 114) for an 8x8 font
   // To be safe for an 8x8 font, use an area of 200x125 at minimum, increasing the area for
   // larger fonts.
   ASSERT(ftw >= 58);
   ASSERT(fth >= 58);
   ASSERT(flw >= 58);
   ASSERT(flh >= 58);

   // h = fh + 4*bh;
   // ASSERT (h >= (58 + 4*bh)) , (h >= (114)
   // w = fw + 3*bh , fw = ftw / 0.4 = 58/0.4 = 580/4 = 145
   // w = 145 +3*14 = 187
   // ASSERT (w >= 187)

//   int ftx = 0.1*w , fty = 0.1*h , ftw = 0.3*w , fth = 0.6*h;
//   int flx = 0.5*w , fly = 0.1*h , flw = 0.4*w , flh = 0.6*h;
//   int fnx = 0.1*w , fny = 0.8*h , fnw = 0.8*w , fnh = 0.1*h;
//   int obx = 0.6*w , oby = 0.9*h , obw = 0.2*w , obh = 0.1*h;
//   int cbx = 0.8*w , cby = 0.9*h , cbw = 0.2*w , cbh = 0.1*h;


   ftree.SetArea(ftx,fty,ftw,fth);
   file_list.SetArea(flx,fly,flw,flh);
   file_name.SetArea(fnx,fny,fnw,fnh);
   okay_btn.SetArea(obx,oby,obw,obh);
   cancel_btn.SetArea(cbx,cby,cbw,cbh);
}
*/


void FileSelector::ResetPositions() {
   int x = area.X() , y = area.Y() , w = area.W() , h = area.H();

   const int btnw = (w - 3*10)/2;
   const int btnh = 2*text_height(textfont);
   const int lbw = (w - 3*10)/2;
   const int lbh = h - (4*10 + 2*btnh);

   ftree.SetArea(x + 10 , y + 10 , lbw , lbh);
   file_list.SetArea(x + 2*10 + lbw , y + 10 , lbw , lbh);
   file_name.SetArea(x + 10 , y + 2*10 + lbh , w - 2*10 , btnh);
   okay_btn.SetArea(x + 10 , y + 3*10 + lbh + btnh , btnw , btnh);
   cancel_btn.SetArea(x + 2*10 + btnw  ,y + 3*10 + lbh + btnh , btnw , btnh);
}


void FileSelector::BuildSelections() {
   string input = file_name.GetTextValue();
   list<string> flist = TokenizeString(input , ',');
   files.clear();
   selected_files = "";
   string folder = ftree.GetSelectedFolder();
   folder = RemoveTrailingSlash(folder);
   folder.push_back(OTHER_PATH_SEPARATOR);
   for (list<string>::iterator it = flist.begin() ; it != flist.end() ; ++it) {
      string file = folder + *it;
      files.push_back(file);
      selected_files.append(file);
      list<string>::iterator next = it;
      ++next;
      if (next != flist.end()) {
         selected_files.push_back(',');
      }
   }
}



FileSelector::FileSelector(string name , FONT* text_font , bool is_open_dialog , bool multiple_selections_allowed ,
                           string okay_button_label , string default_folder , string default_file_name , string file_extensions ,
                           const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      ftree(name + string("-ftree") , text_font , Rectangle(0,0,75,75)),
      file_list(name + string("-file_list") , text_font , multiple_selections_allowed , Rectangle(0,0,75,75)),
      file_name(name + string("-file_name") , TEXT_INPUT , text_font , 32 , 512 , 0 , 0),
      okay_btn(name + string("-okay_btn") , ROUNDED_BTN , SPRING_BTN , text_font , okay_button_label , input_key_press(KEY_NONE) , Rectangle(0,0,0,0) , DEFAULT_FLAGS | ALLOW_CLOSE),
      cancel_btn(name + string("-cancel_btn") , ROUNDED_BTN , SPRING_BTN , text_font , "Cancel" , input_key_press(KEY_NONE) , Rectangle(0,0,0,0) , DEFAULT_FLAGS | ALLOW_CLOSE),
      textfont(text_font),
      open_dialog(is_open_dialog),
      extensions(file_extensions),
      selected_files(),
      files()
{
   ASSERT(text_font);
   SetMinDimensions(AbsMinWidth() , AbsMinHeight());
   ResetPositions();
   ftree.SetParent(this);
   file_list.SetParent(this);
   file_name.SetParent(this);
   okay_btn.SetParent(this);
   cancel_btn.SetParent(this);
   ftree.UseWidgetColorset(wcols);
   file_list.UseWidgetColorset(wcols);
   file_name.UseWidgetColorset(wcols);
   okay_btn.UseWidgetColorset(wcols);
   cancel_btn.UseWidgetColorset(wcols);

   bool found = ftree.SelectFolder(default_folder);
   if (!found) {
      ftree.SelectDefaultFolder();
   }
   file_name.SetTextValue(default_file_name);
}



WidgetMsg FileSelector::Update (double tsec) {
   UINT retmsg = DIALOG_OKAY;
   WidgetMsg wmsg1 = ftree.Update(tsec);
   WidgetMsg wmsg2 = file_list.Update(tsec);
   WidgetMsg wmsg3 = file_name.Update(tsec);
   WidgetMsg wmsg4 = okay_btn.Update(tsec);
   WidgetMsg wmsg5 = cancel_btn.Update(tsec);
   retmsg |= wmsg1.Messages();
   retmsg |= wmsg2.Messages();
   retmsg |= wmsg3.Messages();
   retmsg |= wmsg4.Messages();
   retmsg |= wmsg5.Messages();
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



WidgetMsg FileSelector::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   WidgetMsg wmsg1 = ftree.CheckInputs(msx,msy);
   WidgetMsg wmsg2 = file_list.CheckInputs(msx,msy);
   WidgetMsg wmsg3 = file_name.CheckInputs(msx,msy);
   WidgetMsg wmsg4 = okay_btn.CheckInputs(msx,msy);
   WidgetMsg wmsg5 = cancel_btn.CheckInputs(msx,msy);
   retmsg |= wmsg1.Messages();
   retmsg |= wmsg2.Messages();
   retmsg |= wmsg3.Messages();
   retmsg |= wmsg4.Messages();
   retmsg |= wmsg5.Messages();
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void FileSelector::DisplayOn(BITMAP* bmp , int x , int y) {
   if (flags & VISIBLE) {
      Rectangle r = area;
      r.MoveBy(x,y);
      //r.Fill(bmp , (*wcols)[BGCOL]);
      ftree.DisplayOn(bmp,x,y);
      file_list.DisplayOn(bmp,x,y);
      file_name.DisplayOn(bmp,x,y);
      okay_btn.DisplayOn(bmp,x,y);
      cancel_btn.DisplayOn(bmp,x,y);
   }
   ClearRedrawFlag();
}



void FileSelector::QueueUserMessage(const WidgetMsg& wmsg) {
//      const WidgetMsg ftmsg =
   // file tree , file list , file name , okay button , cancel button
   const WidgetMsg ftmsg1(&ftree , TOPIC_FOLDER_TREE , FOLDER_SELECTED);
   const WidgetMsg flmsg1(&file_list , TOPIC_LISTBOX , LISTBOX_SELECTION_CHANGED);
   const WidgetMsg flmsg2(&file_list , TOPIC_LISTBOX , LISTBOX_DOUBLE_CLICK_SELECT);
   const WidgetMsg btnmsg1(&okay_btn , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
   const WidgetMsg btnmsg2(&cancel_btn , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
   const WidgetMsg inputmsg1(&file_name , TOPIC_TEXT_INPUT , TEXT_VALUE_ENTERED);

   if (wmsg == ftmsg1) {
//         FolderItem* fitem = dynamic_cast<FolderItem*>(wmsg.From());
      string new_path = ftree.GetSelectedFolder();
      UINT size = new_path.size();
      if (size) {
         char tail = new_path.at(size - 1);
         if (tail != OTHER_PATH_SEPARATOR) {
            new_path.push_back(OTHER_PATH_SEPARATOR);
         }
         new_path.push_back('*');
         BuildFileList(new_path.c_str() , FA_NONE , FA_NONE , extensions);
         file_list.CopyFromFileList();
      } else {
         file_list.ClearList();
      }
      SetRedrawFlag();
   } else if (wmsg == flmsg1) {
      // Listbox selection changed , update file name input box
      file_name.SetTextValue(file_list.GetSelections("" , ","));
      SetRedrawFlag();
   } else if (wmsg == flmsg2) {// list box double click
      string folder = ftree.GetSelectedFolder();
      folder = RemoveTrailingSlash(folder);
      folder.push_back(OTHER_PATH_SEPARATOR);
      string selection = file_list.GetSelections("" , " ");
      string file = folder + selection;
      int attrib = 0;
      if (file_exists(file.c_str() , FA_ALL , &attrib)) {
         if (attrib & FA_DIREC) {
            ftree.SelectFolder(file);
         } else {
            selected_files = file;
            files.clear();
            files.push_back(file);
            WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_FILE_SELECTOR , FILE_OKAYED));
         }
      }
      file_name.SetTextValue(file_list.GetSelections("" , " "));
      SetRedrawFlag();
   } else if (wmsg == btnmsg1) {
      BuildSelections();
      WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_FILE_SELECTOR , FILE_OKAYED));
   } else if (wmsg == btnmsg2) {
      WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_FILE_SELECTOR , FILE_CANCELLED));
   } else if (wmsg == inputmsg1) {
      string selection = file_name.GetTextValue();
      string folder = ftree.GetSelectedFolder();
      folder = RemoveTrailingSlash(folder);
      folder.push_back(OTHER_PATH_SEPARATOR);
      string file = folder + selection;
      if (open_dialog) {
         int attrib = 0;
         if (file_exists(file.c_str() , FA_ALL , &attrib)) {
            if (attrib & FA_DIREC) {
               ftree.SelectFolder(file);
            } else {
               selected_files = file;
               files.clear();
               files.push_back(file);
               WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_FILE_SELECTOR , FILE_OKAYED));
            }
         } else {
            // New file that doesn't exist
            selected_files = file;
            files.clear();
            files.push_back(file);
            WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_FILE_SELECTOR , FILE_OKAYED));
         }
      } else {
         selected_files = file;
         files.clear();
         files.push_back(file);
         WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_FILE_SELECTOR , FILE_OKAYED));
      }
      file_name.SetTextValue("");
      SetRedrawFlag();
   } else {
      WidgetBase::QueueUserMessage(wmsg);
   }


}



void FileSelector::SetWidgetColors(const WidgetColorset& colors) {
   WidgetBase::SetWidgetColors(colors);
   ftree.UseWidgetColorset(&wc);
   file_list.UseWidgetColorset(&wc);
   file_name.UseWidgetColorset(&wc);
   okay_btn.UseWidgetColorset(&wc);
   cancel_btn.UseWidgetColorset(&wc);
}



void FileSelector::UseWidgetColorset(WidgetColorset* colorset) {
   WidgetBase::UseWidgetColorset(colorset);
   ftree.UseWidgetColorset(colorset);
   file_list.UseWidgetColorset(colorset);
   file_name.UseWidgetColorset(colorset);
   okay_btn.UseWidgetColorset(colorset);
   cancel_btn.UseWidgetColorset(colorset);
}



void FileSelector::UseIndividualColorset() {
   WidgetBase::UseIndividualColorset();
   ftree.UseWidgetColorset(&wc);
   file_list.UseWidgetColorset(&wc);
   file_name.UseWidgetColorset(&wc);
   okay_btn.UseWidgetColorset(&wc);
   cancel_btn.UseWidgetColorset(&wc);
}



void FileSelector::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
   ResetPositions();
}



void FileSelector::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   ResetPositions();
}



void FileSelector::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   ResetPositions();
}



void FileSelector::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   ResetPositions();
}



void FileSelector::SetFocusState(bool state) {
   WidgetBase::SetFocusState(state);
   file_name.SetFocusState(state);
}



int FileSelector::AbsMinWidth() {
   return (3*10 + ftree.AbsMinWidth() + file_list.AbsMinWidth());
}



int FileSelector::AbsMinHeight() {
   int ftamh = ftree.AbsMinHeight();
   int flamh = file_list.AbsMinHeight();
   int lbamh = (ftamh > flamh)?ftamh:flamh;
   return (4*10 + 2*2*text_height(textfont) + lbamh);
}



void FileSelector::PrintFileSystem(std::ostream& os) {
   ftree.PrintFileSystem(os);
}










