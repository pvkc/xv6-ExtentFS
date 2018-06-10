#define T_DIR  1   // Directory
#define T_FILE 2   // File
#define T_DEV  3   // Device
#define T_EXT_FILE 4 // Extent based file

struct stat {
  short type;  // Type of file
  int dev;     // File system's disk device
  uint ino;    // Inode number
  short nlink; // Number of links to file
  uint size;   // Size of file in bytes
  struct extents_details{
    uint addr:24, // Extent Pointers
         len:8 ;  // Extent Length 
  }extents[12];
};
