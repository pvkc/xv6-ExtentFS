#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"

void writeToNormal();
void writeToLseek();

int main(){

  //int fd = open("test.txt", O_CREATE|O_RDWR);
  int fd = open("test.txt", O_CREATE|O_EXTENT|O_RDWR);
  if (fd <= 0){
    printf(1, "File open failed\n");
    exit();
  }

  char x[1024];
  int i;
  for(i = 0; i < 1024; i++){
      x[i] = 'a';
  }

  for (i = 0; i < 80; i++){
      int size = write(fd, x, 1024);

      if (size < 1024)
      {
        printf(2,"Write failed. Return code %d\n", size);
        exit();
      }
  }

  printf(1, "Wrote 80 * 1024 bytes successfully\n");
  struct stat extFileInfo;
  int rc = fstat(fd, &extFileInfo);
  if (rc != 0)
  {
    printf(2,"fstat error, retruned %d",rc);
    exit();
  }

  printf(1, "File size %d bytes\n", extFileInfo.size);
  i = 0;
  uint tot_blocks = 0;
  while (extFileInfo.extents[i].len){
    tot_blocks += extFileInfo.extents[i].len;
    printf(1,"Extent addr:%d. Extent Len:%d\n", extFileInfo.extents[i].addr, extFileInfo.extents[i].len);
    i++;
    if (i >= 12)
      break;
  }

  if (tot_blocks*512 != extFileInfo.size){
    printf(2, "Something wrong, total blocks don't match size.!!!");
  }

  exit();

}

void writeToLseek(char *fileName, off_t offset){
  off_t off;
  int fd = open(fileName, O_CREATE|O_RDWR|O_EXTENT);

  if (fd <= 0){
    printf(1, "File open failed\n");
    exit();
  }

  char x[1024];
  int i;
  for (i = 0; i < 1024; i++){
    x[i] = 'x';
  }
  
  for (i = 0; i < 1; i++){
    int rc;
    if ((rc = write(fd, x, 1024)) != 1024){
      printf(2, "Normal File, Write failed. Return code %d\n", rc);
      exit();
    }
  }

  //new array to write into file that is offsetted by 16 bytes
  char y[512];
  for(i = 0; i < 512; i++){
    y[i] = 'y';
  }

  if((off = lseek(fd, offset)) == -1) {
    printf(1, "lseek failed, returned value: %d\n", off);
    return;
  } else {
    printf(1, "lseek successful, offset is: %d\n", off);
  }
  
  for (i = 0; i < 1; i++){
    int rc;
    if ((rc = write(fd, y, 512)) != 512){
      printf(2, "Normal File, Write failed. Return code %d\n", rc);
      exit();
    }
  }
}
